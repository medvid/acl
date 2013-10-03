@ void acl_bit_set(vect a, uint pos);
@   set bit at position "pos" in array "a"
@ on entry:
@   r0 = pointer to input/output array
@   r1 = position of bit to be set (0 -> lsb)

		.global acl_bit_set
		.text
		.arm

src		.req	r0	@
pos		.req	r1	@
shift		.req	r2	@
tmp		.req	r3	@
mask		.req	r12	@

acl_bit_set:	and	shift, pos, #31
		mov	mask, #1
		mov	mask, mask, lsl shift
		mov	pos, pos, lsr #5
		ldr	tmp, [src, pos, lsl #2]
		orr	tmp, mask
		str	tmp, [src, pos, lsl #2]
		bx	lr

		.end
