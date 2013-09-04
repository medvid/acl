@ void acl_str2hex_be(vect res, bytes str, size_t len);
@   converts string(hex chars) to big-endian number in res[len]
@ on entry:
@   r0 = pointer to result
@   r1 = pointer to input string
@   r2 = length of result array in 32-bit words

		.global acl_str2hex_be
		.text
		.arm

dest		.req	r0	@
src		.req	r1	@
len		.req	r2	@
cnt		.req	r3	@
tmp		.req	r4	@
acc		.req	r5	@
ind		.req	r12	@

asc2hex:	subs	tmp, #'0'
		cmp	tmp, #10
		movlo	pc, lr
		subs	tmp, #'A' - '0'
		cmp	tmp, #6
		addlo	tmp, #10
		movlo	pc, lr
		sub	tmp, #'a' - 'A' - 10
		mov	pc, lr

acl_str2hex_be:	push	{r4-r5, r14}
		mov	cnt, #0
		mov	ind, #0
as2hb_main_lp:	mov	acc, #0
		ldrb	tmp, [src, cnt]
		cmp	tmp, #0
		beq	as2hb_str_end
		bl	asc2hex
		mov	acc, tmp, lsl #4
		add	cnt, #1

		ldrb	tmp, [src, cnt]
		cmp	tmp, #0
		beq	as2hb_str_end
		bl	asc2hex
		orr	acc, tmp
		add	cnt, #1

as2hb_str_end:	strb	acc, [dest, ind]
		add	ind, #1
		cmp	ind, len, lsl #2
		bne	as2hb_main_lp

		pop	{r4-r5, r14}
		bx	lr

		.end
