@ uint acl_rev(uint a);
@   return 32-bit int with byte order reversed
@   taken directly from "programming techniques", arm doc. dui 0021a
@ on entry:
@   r0 = input
@ returns:
@   r0 = result

		.global acl_rev
		.text
		.arm

val		.req	r0	@
tmp		.req	r1	@

acl_rev:	eor	tmp, val, val, ror #16
		bic	tmp, #0xff0000
		mov	val, val, ror #8
		eor	val, tmp, lsr #8
		bx	lr

		.end
