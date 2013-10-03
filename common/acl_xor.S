@ void acl_xor(vect res, vect a, vect b, size_t len);
@   res = a xor b
@ on entry:
@   r0 = pointer to result
@   r1 = pointer to first operand
@   r2 = pointer to second operand
@   r3 = length of input/output arrays in 32-bit words

		.global acl_xor
		.text
		.arm

dest		.req	r0	@
src1		.req	r1	@
src2		.req	r2	@
len		.req	r3	@
tmp1		.req	r4	@
tmp2		.req	r5	@
tmp3		.req	r6	@
tmp4		.req	r12	@

acl_xor:	push	{r4-r6}
ax_lp1:		ldmia	src1!, {tmp1, tmp2}
		ldmia	src2!, {tmp3, tmp4}
		eor	tmp1, tmp3
		eor	tmp2, tmp4
		subs	len, #2
		stmhsia	dest!, {tmp1, tmp2}
		bhi	ax_lp1
		strlo	tmp1, [dest]
		pop	{r4-r6}
		bx	lr

		.end
