@ void acl_hex2str_le(bytes res, vect a, size_t len);
@   converts little-endian number in a to string(hex chars)[len]
@ on entry:
@   r0 = pointer to result string
@   r1 = pointer to input
@   r2 = length of result string in bytes

		.global acl_hex2str_le
		.text
		.arm

dest		.req	r0	@
src		.req	r1	@
len		.req	r2	@
tmp		.req	r3	@
acc		.req	r12	@

acl_hex2str_le:	ldrb	acc, [src], #1
		subs	len, #1
		bxmi	lr
		and	tmp, acc, #0x0f
		cmp	tmp, #10
		addlo	tmp, #'0'
		addhs	tmp, #'A' - 10
		strb	tmp, [dest, len]
		subs	len, #1
		bxmi	lr
		mov	tmp, acc, lsr #4
		cmp	tmp, #10
		addlo	tmp, #'0'
		addhs	tmp, #'A' - 10
		strb	tmp, [dest, len]
		b	acl_hex2str_le

		.end
