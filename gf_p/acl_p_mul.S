@ void acl_p_mul(vect2 res, vect a, vect b, size_t len);
@   res[2*len] = a[len] * b[len]
@   does not work in-place (res != a, res != b)
@   works only for len >= 3
@ on entry:
@   r0 = pointer to result
@   r1 = pointer to first operand
@   r2 = pointer to second operand
@   r3 = length of input arrays in 32-bit words (output is twice as long)

		.global acl_p_mul
		.text
		.arm

dest		.req	r0	@
src1		.req	r1	@
src2		.req	r2	@
len		.req	r3	@
tmp2		.req	r3	@
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

acl_p_mul:	push	{r4-r11, r14}
		push	{len}
		add	src2, #4
		ldmia	src1!, {pro1, pro2}
		ldmda	src2!, {pro3, pro4}
		umull	tmp1, sum1, pro1, pro3
		str	tmp1, [dest], #4
		mov	sum2, #0
		mov	sum3, #0
		mov	ind, #2
		mov	cnt, #2
		b	apm_entry

		@ first half
apm_h1_lp1:	sub	src1, ind, lsl #2
		add	ind, #1
		add	src2, ind, lsl #2
		mov	cnt, ind
		tst	cnt, #1
		ldrne	pro2, [src1], #4
		ldrne	pro3, [src2], #-4
		bne	apm_h1_entry

apm_h1_lp2:	ldmia	src1!, {pro1, pro2}
		ldmda	src2!, {pro3, pro4}
apm_entry:	umull	tmp1, tmp2, pro1, pro4
		adds	sum1, tmp1
		adcs	sum2, tmp2
		adc	sum3, #0
apm_h1_entry:	umull	tmp1, tmp2, pro2, pro3
		adds	sum1, tmp1
		adcs	sum2, tmp2
		adc	sum3, #0
		subs	cnt, #2
		bhi	apm_h1_lp2

		@ got a diagonal
		str	sum1, [dest], #4
		mov	sum1, sum2
		mov	sum2, sum3
		mov	sum3, #0

		ldr	len, [sp]
		cmp	ind, len
		bne	apm_h1_lp1

		@ second half
apm_h2_lp1:	add	src2, ind, lsl #2
		sub	ind, #1
		sub	src1, ind, lsl #2
		mov	cnt, ind
		tst	cnt, #1
		ldrne	pro2, [src1], #4
		ldrne	pro3, [src2], #-4
		bne	apm_h2_entry

apm_h2_lp2:	ldmia	src1!, {pro1, pro2}
		ldmda	src2!, {pro3, pro4}
		umull	tmp1, tmp2, pro1, pro4
		adds	sum1, tmp1
		adcs	sum2, tmp2
		adc	sum3, #0
apm_h2_entry:	umull	tmp1, tmp2, pro2, pro3
		adds	sum1, tmp1
		adcs	sum2, tmp2
		adc	sum3, #0
		subs	cnt, #2
		bhi	apm_h2_lp2

		@ got a diagonal
		str	sum1, [dest], #4
		mov	sum1, sum2
		mov	sum2, sum3
		mov	sum3, #0

		cmp	ind, #2
		bne	apm_h2_lp1

		umull	tmp1, tmp2, pro2, pro4
		adds	sum1, tmp1
		adc	sum2, tmp2
		stmia	dest, {sum1, sum2}
		pop	{r0, r4-r11, r14}
		bx	lr

		.end
