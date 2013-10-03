@ uint acl_p_mod_add32(vect res, vect a, uint b, vect m, size_t len);
@   res = (a + b [32-bit] ) mod m, returns number of subtractions of m
@ uint acl_p_mod_add(vect res, vect a, vect b, vectm, size_t len);
@   res = (a + b) mod m, returns number of subtractions of m

@ if m == 0, these additions work just like ordinary additions
@ (they're not modular)

@ on entry:
@   r0 = pointer to result
@   r1 = pointer to first operand
@   r2 = (pointer to) second operand
@   r3 = pointer to m
@   [sp] = length of input/output arrays in 32-bit words

		.global acl_p_mod_add32
		.global acl_p_mod_add
		.text
		.arm

dest		.req	r0	@
src1		.req	r1	@
carry		.req	r1	@
src2		.req	r2	@
emm		.req	r3	@
len		.req	r4	@
cnt		.req	r5	@
tmp1		.req	r6	@
tmp2		.req	r7	@
total		.req	r12	@

acl_p_mod_add32:
		push	{r4-r7}
		mov	total, #0
		ldr	len, [sp, #4*4]
		ldr	tmp1, [src1]
		adds	tmp1, src2
		str	tmp1, [dest]

		mov	cnt, #1
apma_lp0:	ldr	tmp1, [src1, cnt, lsl #2]
		adcs	tmp1, #0
		str	tmp1, [dest, cnt, lsl #2]
		add	cnt, #1
		teq	cnt, len
		bne	apma_lp0
		b	apma_entry

acl_p_mod_add:	push	{r4-r7}
		mov	total, #0
		ldr	len, [sp, #4*4]
		mov	cnt, #0
		msr	cpsr_f, #0
apma_lp1:	ldr	tmp1, [src1, cnt, lsl #2]
		ldr	tmp2, [src2, cnt, lsl #2]
		adcs	tmp1, tmp2
		str	tmp1, [dest, cnt, lsl #2]
		add	cnt, #1
		teq	cnt, len
		bne	apma_lp1
apma_entry:	mov	carry, #1
		bcs	apma_subtract
		mov	carry, #0

		@ a > m ?
apma_cmp:	cmp	emm, #0
		beq	apma_ret
		sub	cnt, len, #1
apma_lp2:	ldr	tmp1, [dest, cnt, lsl #2]
		ldr	tmp2, [emm, cnt, lsl #2]
		cmp	tmp1, tmp2
		blo	apma_ret
		bhi	apma_subtract
		subs	cnt, #1
		bhs	apma_lp2

		@ a = a - m
apma_subtract:	add	total, #1
		mov	cnt, #0
		msr	cpsr_f, #(1<<29)
apma_lp4:	ldr	tmp1, [dest, cnt, lsl #2]
		ldr	tmp2, [emm, cnt, lsl #2]
		sbcs	tmp1, tmp2
		str	tmp1, [dest, cnt, lsl #2]
		add	cnt, #1
		teq	cnt, len
		bne	apma_lp4
		sbcs	carry, #0
		bne	apma_subtract

apma_ret:	mov	r0, total
		pop	{r4-r7}
		bx	lr

		.end
