@ void acl_xor32(vect res, vect a, uint b, size_t len);
@   res = a xor b [32-bit]
@ on entry:
@   r0 = pointer to result
@   r1 = pointer to first operand
@   r2 = second operand
@   r3 = length of input/output arrays in 32-bit words

		.global acl_xor32
		.text
		.arm

dest		.req	r0	@
src1		.req	r1	@
src2		.req	r2	@
len		.req	r3	@
tmp		.req	r12	@

acl_xor32:	ldr	tmp, [src1], #4
		eor	tmp, src2
		str	tmp, [dest], #4
		cmp	dest, src1
		bxeq	lr
		sub	src2, len, #1
		b	acl_mov

		.end
