@ void acl_2_mul(vect2 res, vect a, vect b, size_t len);
@   res[2*len] = a[len] * b[len] over gf(2^m)
@   does not work in-place (res != a, res != b)
@ on entry:
@   r0 = pointer to result
@   r1 = pointer to first operand
@   r2 = pointer to second operand
@   r3 = length of input arrays in 32-bit words (output is twice as long)

		.global acl_2_mul
		.text
		.arm

dest		.req	r0	@
src1		.req	r1	@
src2		.req	r2	@
len		.req	r3	@
ind		.req	r4	@
pro1		.req	r5	@
pro2		.req	r6	@
res1		.req	r7	@
res2		.req	r8	@
sum1		.req	r9	@
sum2		.req	r10	@
cnt		.req	r12	@

acl_2_mul:	push	{r4-r10}
		mov	sum1, #0
		mov	sum2, #0
		mov	ind, #1
		mov	cnt, #1
		b	a2m_h1_lp2

		@ first half
a2m_h1_lp1:	sub	src1, ind, lsl #2
		add	ind, #1
		add	src2, ind, lsl #2
		mov	cnt, ind

a2m_h1_lp2:	ldr	pro1, [src1], #4
		ldr	pro2, [src2], #-4
		mov	res1, #0
		mvn	res2, #1

a2m_h1_lp3:	adds	pro1, pro1
		eorcs	res1, pro2
		adds	res1, res1
		adc	res2, res2

		adds	pro1, pro1
		eorcs	res1, pro2
		adds	res1, res1
		adc	res2, res2

		adds	pro1, pro1
		eorcs	res1, pro2
		adds	res1, res1
		adc	res2, res2

		adds	pro1, pro1
		eorcs	res1, pro2
		adds	res1, res1
		adc	res2, res2

		adds	pro1, pro1
		eorcs	res1, pro2
		adds	res1, res1
		adc	res2, res2

		adds	pro1, pro1
		eorcs	res1, pro2
		adds	res1, res1
		adc	res2, res2

		adds	pro1, pro1
		eorcs	res1, pro2
		adds	res1, res1
		adc	res2, res2

		adds	pro1, pro1
		eorcs	res1, pro2
		adds	res1, res1
		adcs	res2, res2
		bcs	a2m_h1_lp3

		eors	sum2, res2, rrx
		eor	sum1, res1, rrx
		subs	cnt, #1
		bne	a2m_h1_lp2

		@ got a diagonal
		str	sum1, [dest], #4
		mov	sum1, sum2
		mov	sum2, #0
		cmp	ind, len
		bne	a2m_h1_lp1

		@ second half
a2m_h2_lp1:	add	src2, ind, lsl #2
		sub	ind, #1
		sub	src1, ind, lsl #2
		mov	cnt, ind

a2m_h2_lp2:	ldr	pro1, [src1], #4
		ldr	pro2, [src2], #-4
		mov	res1, #0
		mvn	res2, #1

a2m_h2_lp3:	adds	pro1, pro1
		eorcs	res1, pro2
		adds	res1, res1
		adc	res2, res2

		adds	pro1, pro1
		eorcs	res1, pro2
		adds	res1, res1
		adc	res2, res2

		adds	pro1, pro1
		eorcs	res1, pro2
		adds	res1, res1
		adc	res2, res2

		adds	pro1, pro1
		eorcs	res1, pro2
		adds	res1, res1
		adc	res2, res2

		adds	pro1, pro1
		eorcs	res1, pro2
		adds	res1, res1
		adc	res2, res2

		adds	pro1, pro1
		eorcs	res1, pro2
		adds	res1, res1
		adc	res2, res2

		adds	pro1, pro1
		eorcs	res1, pro2
		adds	res1, res1
		adc	res2, res2

		adds	pro1, pro1
		eorcs	res1, pro2
		adds	res1, res1
		adcs	res2, res2
		bcs	a2m_h2_lp3

		eors	sum2, res2, rrx
		eor	sum1, res1, rrx
		subs	cnt, #1
		bne	a2m_h2_lp2

		@ got a diagonal
		str	sum1, [dest], #4
		mov	sum1, sum2
		mov	sum2, #0
		cmp	ind, #1
		bne	a2m_h2_lp1

		str	sum1, [dest]
		pop	{r4-r10}
		bx	lr

		.end
