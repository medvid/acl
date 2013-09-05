@ void acl_rev_bytes(vect a, size_t len);
@   reverse byte order in array
@ on entry:
@   r0 = input
@   r1 = input length
@ returns:
@   r0 = result

		.global acl_rev_bytes
		.text
		.arm

src		.req	r0	@
len		.req	r1	@
val		.req	r2	@
tmp		.req	r3	@

acl_rev_bytes:	cmp	len, #0
		beq	arb_end
		sub	len, #1
		ldr	val, [src, len, LSL #2]
		eor	tmp, val, val, ror #16
		bic	tmp, #0xff0000
		mov	val, val, ror #8
		eor	val, tmp, lsr #8
		str	val, [src, len, LSL #2]
		b	acl_rev_bytes
arb_end:
		bx	lr

		.end
