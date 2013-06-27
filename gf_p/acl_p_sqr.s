@ void acl_p_sqr(vect2 res, vect a, size_t len);
@   res[2*len] = a[len] * a[len]
@   does not work in-place (res != a)
@   works only for len > 4
@ on entry:
@   r0 = pointer to result
@   r1 = pointer to input
@   r2 = length of input array in 32-bit words (output is twice as long)

		.global acl_p_sqr
		.text
		.arm

dest		.req	r0	@
src1		.req	r1	@
len		.req	r2	@
tmp2		.req	r2	@
src2		.req	r3	@
ind		.req	r4	@
pro1		.req	r5	@
pro2		.req	r6	@
pro3		.req	r7	@
pro4		.req	r8	@
sum1		.req	r9	@
sum2		.req	r10	@
sum3		.req	r11	@
tmp1		.req	r12	@
cnt		.req	r14	@

acl_p_sqr:	push	{r4-r11, r14}
		push	{len}
		mov	src2, src1
		ldr	pro1, [src2], #4
		umull	tmp1, sum1, pro1, pro1
		movs	sum1, sum1, lsr #1
		mov	tmp1, tmp1, rrx
		str	tmp1, [dest], #4
		mov	sum2, #0
		mov	sum3, #0
		mov	ind, #2
		mov	cnt, #1
		b	aps_entry

		@ first quarter
aps_h1_lp1:	mov	tmp1, ind, lsr #1
		sub	src1, tmp1, lsl #2
		add	tmp1, #1
		add	src2, tmp1, lsl #2
		add	ind, #1
		mov	cnt, ind, lsr #1
aps_entry:	tst	cnt, #1
		ldrne	pro2, [src1], #4
		ldrne	pro3, [src2], #-4
		bne	aps_h1_entry

aps_h1_lp2:	ldmia	src1!, {pro1, pro2}
		ldmda	src2!, {pro3, pro4}
		umull	tmp1, tmp2, pro1, pro4
		adds	sum1, tmp1
		adcs	sum2, tmp2
		adc	sum3, #0
aps_h1_entry:	umull	tmp1, tmp2, pro2, pro3
		adds	sum1, tmp1
		adcs	sum2, tmp2
		adc	sum3, #0
		subs	cnt, #2
		bhi	aps_h1_lp2

		@ got half a diagonal
		tst	ind, #1
		beq	aps_h1_skip
		str	sum1, [dest], #4
		mov	sum1, sum2
		mov	sum2, sum3
		mov	sum3, #0
		ldr	len, [sp]
		cmp	ind, len
		bne	aps_h1_lp1
		b	aps_h2_lp1

		@ add center/2
aps_h1_skip:	umull	tmp1, tmp2, pro3, pro3
		movs	tmp2, tmp2, lsr #1
		movs	tmp1, tmp1, rrx
		addcss	sum1, #0x80000000
		str	sum1, [dest], #4
		adcs	sum1, sum2, tmp1
		adc	sum2, sum3, tmp2
		mov	sum3, #0
		ldr	len, [sp]
		cmp	ind, len
		bne	aps_h1_lp1

		@ second quarter
aps_h2_lp1:	mov	tmp1, ind, lsr #1
		add	src2, tmp1, lsl #2
		sub	tmp1, #1
		sub	src1, tmp1, lsl #2
		sub	ind, #1
		mov	cnt, ind, lsr #1
		tst	cnt, #1
		ldrne	pro2, [src1], #4
		ldrne	pro3, [src2], #-4
		bne	aps_h2_entry

aps_h2_lp2:	ldmia	src1!, {pro1, pro2}
		ldmda	src2!, {pro3, pro4}
		umull	tmp1, tmp2, pro1, pro4
		adds	sum1, tmp1
		adcs	sum2, tmp2
		adc	sum3, #0
aps_h2_entry:	umull	tmp1, tmp2, pro2, pro3
		adds	sum1, tmp1
		adcs	sum2, tmp2
		adc	sum3, #0
		subs	cnt, #2
		bhi	aps_h2_lp2

		@ got half a diagonal
		tst	ind, #1
		beq	aps_h2_skip
		str	sum1, [dest], #4
		mov	sum1, sum2
		mov	sum2, sum3
		mov	sum3, #0
		b	aps_h2_lp1

		@ add center/2
aps_h2_skip:	umull	tmp1, tmp2, pro3, pro3
		movs	tmp2, tmp2, lsr #1
		movs	tmp1, tmp1, rrx
		addcss	sum1, #0x80000000
		str	sum1, [dest], #4
		adcs	sum1, sum2, tmp1
		adc	sum2, sum3, tmp2
		mov	sum3, #0
		cmp	ind, #2
		bne	aps_h2_lp1

		stmia	dest!, {sum1, sum2}
		ldr	len, [sp]
		sub	dest, len, lsl #3
		add	src1, #4
		sub	src1, len, lsl #2
		mov	cnt, len
		ldr	pro1, [src1]
		movs	pro1, pro1, lsr #1

		@ multiply by 2
aps_sh_lp:	ldmia	dest, {sum1, sum2}
		adcs	sum1, sum1
		adcs	sum2, sum2
		stmia	dest!, {sum1, sum2}
		sub	cnt, #1
		teq	cnt, #0
		bne	aps_sh_lp

		pop	{r0, r4-r11, r14}
		bx	lr

		.end
