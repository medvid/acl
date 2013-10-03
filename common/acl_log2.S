@ int acl_log2(vect a, size_t len);
@   returns the position of the most significant non-zero bit
@   -1 -> all bits are zero, 0 -> lsb, ...
@ on entry:
@   r0 = pointer to input array
@   r1 = length of input array in 32-bit words
@ returns:
@   r0 = result

		.global acl_log2
		.text
		.arm

src		.req	r0	@
len		.req	r1	@
tmp		.req	r2	@

acl_log2:	lsl	len, #5
al2_lp1:	subs	len, #32
		blo	al2_zero
		ldr	tmp, [src, len, lsr #3]
		cmp	tmp, #0
		beq	al2_lp1
al2_lp2:	subpl	len, #1
		addpls	tmp, tmp
		bpl	al2_lp2
al2_zero:	add	r0, len, #31
		bx	lr

		.end
