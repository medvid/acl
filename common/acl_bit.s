@ uint acl_bit(vect a, uint pos, size_t len);
@   return value of bit at position "pos" in array "a"
@   returns 0 if pos >= 32*len
@ on entry:
@   r0 = pointer to input array
@   r1 = position of bit to be read (0 -> lsb)
@   r2 = length of input array in 32-bit words
@ returns:
@   r0 = value of bit

		.global acl_bit
		.text
		.arm

src		.req	r0	@
pos		.req	r1	@
len		.req	r2	@
shift		.req	r3	@
tmp		.req	r12	@

acl_bit:	and	shift, pos, #31
		mov	pos, pos, lsr #5
		cmp	pos, len
		bhs	ab_zero
		ldr	tmp, [src, pos, lsl #2]
		mov	tmp, tmp, lsr shift
		and	r0, tmp, #1
		bx	lr

ab_zero:	mov	r0, #0
		bx	lr

		.end
