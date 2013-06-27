@ void acl_p_mod_sub32(vect res, vect a, uint b, vect m, size_t len);
@   res = (a - b [32-bit] ) mod m

@ void acl_p_mod_sub(vect res, vect a, vect b, vectm, size_t len);
@   res = (a - b) mod m

@ if m == 0, these subtractions work just like ordinary subtractions
@ (they're not modular)

@ on entry:
@   r0 = pointer to result
@   r1 = pointer to first operand
@   r2 = (pointer to) second operand
@   r3 = pointer to m
@   [sp] = length of input/output arrays in 32-bit words

		.global acl_p_mod_sub32
		.global acl_p_mod_sub
		.text
		.arm

dest		.req	r0	@
src1		.req	r1	@
src2		.req	r2	@
emm		.req	r3	@
len		.req	r4	@
cnt		.req	r5	@
tmp1		.req	r6	@
tmp2		.req	r12	@

acl_p_mod_sub32:
		push	{r4-r6}
		ldr	len, [sp, #4*3]
		ldr	tmp1, [src1]
		subs	tmp1, src2
		str	tmp1, [dest]

		mov	cnt, #1
apms_lp0:	ldr	tmp1, [src1, cnt, lsl #2]
		sbcs	tmp1, #0
		str	tmp1, [dest, cnt, lsl #2]
		add	cnt, #1
		teq	cnt, len
		bne	apms_lp0
		bcs	apms_ret
		b	apms_add

acl_p_mod_sub:	push	{r4-r6}
		ldr	len, [sp, #4*3]
		mov	cnt, #0
		msr	cpsr_f, #(1<<29)
apms_lp1:	ldr	tmp1, [src1, cnt, lsl #2]
		ldr	tmp2, [src2, cnt, lsl #2]
		sbcs	tmp1, tmp2
		str	tmp1, [dest, cnt, lsl #2]
		add	cnt, #1
		teq	cnt, len
		bne	apms_lp1
		bcs	apms_ret

		@ add m to result (carry == 0)
apms_add:	teq	emm, #0
		beq	apms_ret
		mov	cnt, #0
apms_lp2:	ldr	tmp1, [dest, cnt, lsl #2]
		ldr	tmp2, [emm, cnt, lsl #2]
		adcs	tmp1, tmp2
		str	tmp1, [dest, cnt, lsl #2]
		add	cnt, #1
		teq	cnt, len
		bne	apms_lp2
		bcc	apms_add

apms_ret:	pop	{r4-r6}
		bx	lr

		.end
