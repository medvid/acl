@ void acl_p_mont_red(vect res, vect2 a, vect m, uint m_inv, size_t len);
@   res = a*r^(-1) mod m
@   does not work in-place (res != a)
@ on entry:
@   r0 = pointer to result
@   r1 = pointer to input
@   r2 = pointer to modulus
@   r3 = -m^-1 mod 2^32
@   [sp] = length of input/output arrays in 32-bit words

		.global acl_p_mont_red
		.text
		.arm

dest		.req	r0	@
src		.req	r1	@
emm		.req	r2	@
minv		.req	r3	@
tmp2		.req	r3	@
len		.req	r3	@
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

acl_p_mont_red:	push {r4-r11, r14}
		push	{minv}

		mov	sum1, #0
		mov	sum2, #0
		mov	sum3, #0
		mov	ind, #1
		b	apmr_entry

		@ first half
apmr_h1_lp1:	sub	dest, ind, lsl #2
		mov	cnt, ind
		add	ind, #1
		add	emm, ind, lsl #2
		tst	cnt, #1
		ldrne	pro2, [dest], #4
		ldrne	pro3, [emm], #-4
		bne	apmr_h1_entry

apmr_h1_lp2:	ldmia	dest!, {pro1, pro2}
		ldmda	emm!, {pro3, pro4}
		umull	tmp1, tmp2, pro1, pro4
		adds	sum1, tmp1
		adcs	sum2, tmp2
		adc	sum3, #0
apmr_h1_entry:	umull	tmp1, tmp2, pro2, pro3
		adds	sum1, tmp1
		adcs	sum2, tmp2
		adc	sum3, #0
		subs	cnt, #2
		bhi	apmr_h1_lp2

		@ calculate next q
apmr_entry:	ldr	tmp1, [src], #4
		adds	sum1, tmp1
		adcs	sum2, #0
		adc	sum3, #0
		ldr	minv, [sp]
		mul	pro1, sum1, minv
		str	pro1, [dest], #4
		ldr	pro2, [emm], #-4
		umull	tmp1, tmp2, pro1, pro2
		adds	sum1, tmp1
		adcs	sum1, sum2, tmp2
		adc	sum2, sum3, #0
		mov	sum3, #0
		ldr	len, [sp, #4*10]
		cmp	ind, len
		bne	apmr_h1_lp1

		@ second half
		sub	dest, #4
		add	emm, #8
		sub	ind, #1
		mov	cnt, ind
		b	apmr_h2_entry1

apmr_h2_lp1:	ldr	tmp1, [src], #4
		adds	sum1, tmp1
		str	sum1, [dest]
		adcs	sum1, sum2, #0
		adc	sum2, sum3, #0
		mov	sum3, #0

		add	dest, ind, lsl #2
		sub	ind, #1
		mov	cnt, ind
		sub	emm, ind, lsl #2
apmr_h2_entry1:	tst	cnt, #1
		ldrne	pro2, [dest], #-4
		ldrne	pro3, [emm], #4
		bne	apmr_h2_entry2

apmr_h2_lp2:	ldmda	dest!, {pro1, pro2}
		ldmia	emm!, {pro3, pro4}
		umull	tmp1, tmp2, pro1, pro4
		adds	sum1, tmp1
		adcs	sum2, tmp2
		adc	sum3, #0
apmr_h2_entry2:	umull	tmp1, tmp2, pro2, pro3
		adds	sum1, tmp1
		adcs	sum2, tmp2
		adc	sum3, #0
		subs	cnt, #2
		bhi	apmr_h2_lp2

		@ finished yet?
		cmp	ind, #1
		bne	apmr_h2_lp1

		ldmia	src, {pro1, pro2}
		adds	sum1, pro1
		adcs	sum2, pro2
		adc	sum3, #0
		stmia	dest!, {sum1, sum2}
		ldr	len, [sp, #4*10]
		sub	dest, len, lsl #2
		sub	emm, len, lsl #2

		@ is there a carry?
		cmp	sum3, #0
		bne	apmr_subtract

		@ result > m ?
		sub	cnt, len, #1
apmr_cmp_lp:	ldr	pro1, [dest, cnt, lsl #2]
		ldr	pro2, [emm, cnt, lsl #2]
		cmp	pro1, pro2
		blo	apmr_ret
		bhi	apmr_subtract
		subs	cnt, #1
		bhs	apmr_cmp_lp

		@ result = result - m
apmr_subtract:	msr	cpsr_f, #(1<<29)
apmr_sub_lp:	ldr	pro1, [dest]
		ldr	pro2, [emm], #4
		sbcs	pro1, pro2
		str	pro1, [dest], #4
		sub	len, #1
		teq	len, #0
		bne	apmr_sub_lp

apmr_ret:	pop	{r0, r4-r11, r14}
		bx	lr

		.end
