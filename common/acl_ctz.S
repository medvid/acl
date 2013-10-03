@ uint acl_ctz(vect a, size_t len);
@   count trailing zeroes; if whole number is zero,returns 32*len
@ on entry:
@   r0 = pointer to input array
@   r1 = length of input array in 32-bit words
@ returns:
@   r0 = result

		.global acl_ctz
		.text
		.arm

src		.req	r0	@
len		.req	r1	@
tmp		.req	r2	@
res		.req	r3	@

acl_ctz:	mov	res, #0
actz_lp1:	ldr	tmp, [src], #4
		cmp	tmp, #0
		bne	actz_lp2
		add	res, #32
		subs	len, #1
		bne	actz_lp1
		bx	lr
actz_lp2:	movs	tmp, tmp, rrx
		addcc	res, #1
		bcc	actz_lp2
		mov	r0, res
		bx	lr

		.end
