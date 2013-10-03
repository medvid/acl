@ void acl_2_sqr(vect2 res, vect a, size_t len);
@   res[2*len] = a[len] * a[len] over gf(2^m)
@   does not work in-place (res != a)
@ on entry:
@   r0 = pointer to result
@   r1 = pointer to input
@   r2 = length of input array in 32-bit words (output is twice as long)

		.global acl_2_sqr
		.text
		.arm

dest		.req	r0	@
src		.req	r1	@
len		.req	r2	@
tab		.req	r3	@
mask		.req	r4	@
pro		.req	r5	@
tmp		.req	r6	@
res		.req	r12	@

a2s_table:	.byte	0x00, 0x01, 0x04, 0x05
		.byte	0x10, 0x11, 0x14, 0x15
		.byte	0x40, 0x41, 0x44, 0x45
		.byte	0x50, 0x51, 0x54, 0x55

acl_2_sqr:	push	{r4-r6}
		adr	tab, a2s_table
		mov	mask, #0xf

a2s_lp:		ldr	pro, [src], #4
		and	tmp, mask, pro
		ldrb	res, [tab, tmp]
		and	tmp, mask, pro, lsr #4
		ldrb	tmp, [tab, tmp]
		eor	res, tmp, lsl #8
		and	tmp, mask, pro, lsr #8
		ldrb	tmp, [tab, tmp]
		eor	res, tmp, lsl #16
		and	tmp, mask, pro, lsr #12
		ldrb	tmp, [tab, tmp]
		eor	res, tmp, lsl #24
		str	res, [dest], #4

		and	tmp, mask, pro, lsr #16
		ldrb	res, [tab, tmp]
		and	tmp, mask, pro, lsr #20
		ldrb	tmp, [tab, tmp]
		eor	res, tmp, lsl #8
		and	tmp, mask, pro, lsr #24
		ldrb	tmp, [tab, tmp]
		eor	res, tmp, lsl #16
		and	tmp, mask, pro, lsr #28
		ldrb	tmp, [tab, tmp]
		eor	res, tmp, lsl #24
		str	res, [dest], #4
		subs	len, #1
		bne	a2s_lp

		pop	{r4-r6}
		bx	lr

		.end
