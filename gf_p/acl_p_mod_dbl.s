@ uint acl_p_mod_dbl(vect a, uint k, vect m, size_tlen);
@   a = (2^k)*a mod m
@   returns number of times it had to subtract m
@ on entry:
@   r0 = pointer to input/result
@   r1 = number of times to double
@   r2 = pointer to m
@   r3 = length of input/output arrays in 32-bit words

		.global acl_p_mod_dbl
		.text
		.arm

dest		.req	r0	@
kay		.req	r1	@
emm		.req	r2	@
len		.req	r3	@
carry		.req	r4	@
tmp1		.req	r5	@
tmp2		.req	r6	@
total		.req	r7	@
cnt		.req	r12	@

acl_p_mod_dbl:	push	{r4-r7}
		mov	total, #0

		@ a = 2 * a
apmd_again:	mov	cnt, #0
		msr	cpsr_f, #0
apmd_lp1:	ldr	tmp1, [dest, cnt, lsl #2]
		adcs	tmp1, tmp1
		str	tmp1, [dest, cnt, lsl #2]
		add	cnt, #1
		teq	cnt, len
		bne	apmd_lp1
		mov	carry, #1
		bcs	apmd_subtract
		mov	carry, #0

		@ a > m ?
		sub	cnt, len, #1
apmd_lp2:	ldr	tmp1, [dest, cnt, lsl #2]
		ldr	tmp2, [emm, cnt, lsl #2]
		cmp	tmp1, tmp2
		blo	apmd_next
		bhi	apmd_subtract
		subs	cnt, #1
		bhs	apmd_lp2

		@ a = a - m
apmd_subtract:	add	total, #1
		mov	cnt, #0
		msr	cpsr_f, #(1<<29)
apmd_lp3:	ldr	tmp1, [dest, cnt, lsl #2]
		ldr	tmp2, [emm, cnt, lsl #2]
		sbcs	tmp1, tmp2
		str	tmp1, [dest, cnt, lsl #2]
		add	cnt, #1
		teq	cnt, len
		bne	apmd_lp3
		sbcs	carry, #0
		bne	apmd_subtract

apmd_next:	subs	kay, #1
		bne	apmd_again
		mov	r0, total
		pop	{r4-r7}
		bx	lr

		.end
