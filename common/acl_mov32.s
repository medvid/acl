@ void acl_mov32(vect res, uint val, size_t len);
@   initializes the array "res" to zero, except thelowest 32-bit word,
@   which is set to the 32-bit constant "val"
@ on entry:
@   r0 = pointer to result array
@   r1 = value
@   r2 = length of result array in 32-bit words

		.global acl_mov32
		.text
		.arm

dest		.req	r0	@
val		.req	r1	@
len		.req	r2	@
tmp1		.req	r3	@
tmp2		.req	r12	@

acl_mov32:	str	val, [dest], #4
		sub	len, #1
		mov	tmp1, #0
		mov	tmp2, #0
am32_lp1:	subs	len, #2
		stmhsia	dest!, {tmp1, tmp2}
		bhi	am32_lp1
		strlo	tmp1, [dest]
		bx	lr

		.end
