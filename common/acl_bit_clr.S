@ void acl_bit_clr(vect a, uint pos);
@   clear bit at position "pos" in array "a"
@ on entry:
@   r0 = pointer to input/output array
@   r1 = position of bit to be cleared (0 -> lsb)

		.global acl_bit_clr
		.text
		.arm

src		.req	r0	@
pos		.req	r1	@
shift		.req	r2	@
tmp		.req	r3	@
mask		.req	r12	@

acl_bit_clr:	and	shift, pos, #31
		mov	mask, #1
		mov	mask, mask, lsl shift
		mov	pos, pos, lsr #5
		ldr	tmp, [src, pos, lsl #2]
		bic	tmp, mask
		str	tmp, [src, pos, lsl #2]
		bx	lr

		.end
