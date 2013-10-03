@ void acl_str2hex_le(vect res, size_t len, bytes str, size_t len_s);
@   converts string(hex chars) to little-endian number in res[len]
@ on entry:
@   r0 = pointer to result
@   r1 = length of result array in 32-bit words
@   r2 = pointer to input string
@   r3 = length of input string in bytes, if 0 -> null terminated

		.global acl_str2hex_le
		.text
		.arm

dest		.req	r0	@
len		.req	r1	@
src		.req	r2	@
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

acl_str2hex_le:	push	{r4-r5, r14}

		@ find end of string
		cmp	cnt, #0
		bne	as2hl_skip
as2hl_init_lp:	ldrb	tmp, [src, cnt]
		cmp	tmp, #0
		addne	cnt, #1
		bne	as2hl_init_lp

as2hl_skip:	mov	ind, #0
as2hl_main_lp:	mov	acc, #0
		subs	cnt, #1
		bmi	as2hl_str_end
		ldrb	tmp, [src, cnt]
		bl	asc2hex
		mov	acc, tmp

		subs	cnt, #1
		bmi	as2hl_str_end
		ldrb	tmp, [src, cnt]
		bl	asc2hex
		orr	acc, tmp, lsl #4

as2hl_str_end:	strb	acc, [dest, ind]
		add	ind, #1
		cmp	ind, len, lsl #2
		bne	as2hl_main_lp

		pop	{r4-r5, r14}
		bx	lr

		.end
