@ bool_t acl_zero(vect a, size_t len);
@   returns true if array is zero, false otherwise
@ on entry:
@   r0 = pointer to input array
@   r1 = length of input array in 32-bit words
@ returns:
@   r0 = result

		.global acl_zero
		.text
		.arm

src		.req	r0	@
len		.req	r1	@
tmp		.req	r2	@

acl_zero:	ldr	tmp, [src], #4
		cmp	tmp, #0
		bne	azro_nope
		subs	len, #1
		bne	acl_zero
		mov	r0, #-1
		bx	lr
azro_nope:	mov	r0, #0
		bx	lr

		.end
