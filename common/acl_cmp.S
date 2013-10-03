@ int acl_cmp(vect a, vect b, size_t len);
@   compares two arrays; returns -1 if a<b, 0 if a==b, 1 if a>b
@ on entry:
@   r0 = pointer to first array
@   r1 = pointer to second array
@   r2 = length of input arrays in 32-bit words
@ returns:
@   r0 = result

		.global acl_cmp
		.text
		.arm

src1		.req	r0	@
src2		.req	r1	@
len		.req	r2	@
tmp1		.req	r3	@
tmp2		.req	r12	@

acl_cmp:	sub	len, #1
acmp_lp:	ldr	tmp1, [src1, len, lsl #2]
		ldr	tmp2, [src2, len, lsl #2]
		cmp	tmp1, tmp2
		blo	acmp_less
		bhi	acmp_more
		subs	len, #1
		bhs	acmp_lp
		mov	r0, #0
		bx	lr
acmp_less:	mov	r0, #-1
		bx	lr
acmp_more:	mov	r0, #1
		bx	lr

		.end
