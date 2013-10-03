@ void acl_rsh(vect a, uint k, size_t len);
@   a = a >> k (right shift by k bits)
@ on entry:
@   r0 = pointer to input/output
@   r1 = number of bits to shift by
@   r2 = length of input/output array in 32-bit words

		.global acl_rsh
		.text
		.arm

dest		.req	r0	@
kay		.req	r1	@
len		.req	r2	@
tmp1		.req	r3	@
tmp2		.req	r4	@
cnt		.req	r5	@
shift_r		.req	r6	@
shift_l		.req	r12	@

acl_rsh:	push	{r4-r6}
ar_lp1:		add	dest, len, lsl #2
		mov	cnt, len
		mov	tmp2, #0
		mov	shift_r, kay
		rsbs	shift_l, shift_r, #32
		movmi	shift_r, #32
		movmi	shift_l, #0

ar_lp2:		ldr	tmp1, [dest, #-4]
		orr	tmp2, tmp1, lsr shift_r
		str	tmp2, [dest, #-4]!
		mov	tmp2, tmp1, lsl shift_l
		subs	cnt, #1
		bne	ar_lp2
		subs	kay, shift_r
		bne	ar_lp1

		pop	{r4-r6}
		bx	lr

		.end
