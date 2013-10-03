BORDER = 512

@ void acl_p_fr(vect res, vect2 a, list data, size_t len);
@   res = a mod (2^exp1 +- 2^exp2 +- ... +- 2^0)
@         does not work in-place (res != a)
@
@ the terms are listed in a table pointed to by data:
@ - the first entry must be the highest exponent
@ - for a negative term (-2^exp), list the exponent (must be <= BORDER)
@ - for a positive term (+2^exp), list the ones' complement of the exponent
@
@ the table is terminated:
@ - by a zero, this is also considered a term (-2^0)
@ - by a value BORDER < val < 0x80000000, each ofthe 32 bits that is '1'
@   encodes a term in the form -2^(bit position)
@
@ examples of lists:
@     128, 1, 0
@     128, 97, 0
@     160, 32, 0x538d             @ 14, 12, 9, 8,7, 3, 2, 0
@     160, 31, 0
@     192, 32, 0x11c9             @ 12, 8, 7, 6, 3, 0
@     192, 64, 0
@     224, 32, 0x1a93             @ 12, 11, 9, 7,4, 1, 0
@     224, 96, ~0
@     256, 32, 0x03d1             @ 9, 8, 7, 6, 4, 0
@     256, 224, ~192, ~96, 0
@     384, 128, 96, ~32, 0
@     521, 0
@
@ on entry:
@ r0 = pointer to result
@ r1 = pointer to input
@ r2 = pointer to exponent table
@ r3 = length of input/output arrays in 32-bit words

		.global acl_p_fr
		.text
		.arm

dest		.req	r0	@
src		.req	r1	@
tab		.req	r2	@
len		.req	r3	@
carry		.req	r4	@
tmp1		.req	r5	@
tmp2		.req	r6	@
ind1		.req	r7	@
ind2		.req	r8	@
shift_r		.req	r9	@
shift_l		.req	r10	@
tmp3		.req	r11	@
tmp4		.req	r12	@
cnt		.req	r14	@

acl_p_fr:	push	{r4-r11, r14}
		push	{tab, len}
		b	apfr_again

		@ get exponent
apfr_main_lp:	mov	ind2, dest
		mov	cnt, len
		ldr	tmp1, [tab, #4]!
		cmp	tmp1, #0
		bmi	apfr_subtract
		cmp	tmp1, #BORDER
		bhi	apfr_mul

		@ src += dest << exp
		mov	ind1, tmp1, lsr #5
		add	ind1, src, ind1, lsl #2
		ands	shift_l, tmp1, #31
		beq	apfr_a_skip1
		rsb	shift_r, shift_l, #32

		@ add with shift
		msr	cpsr_f, #0
		mov	carry, #0
		tst	cnt, #1
		beq	apfr_a_lp1
		ldr	tmp1, [ind1]
		ldr	tmp3, [ind2], #4
		adcs	tmp1, tmp3, lsl shift_l
		mov	carry, tmp3, lsr shift_r
		str	tmp1, [ind1], #4
		sub	cnt, #1
		teq	cnt, #0
		beq	apfr_a_lp2
apfr_a_lp1:	ldmia	ind1, {tmp1, tmp2}
		ldmia	ind2!, {tmp3, tmp4}
		orr	carry, tmp3, lsl shift_l
		adcs	tmp1, carry
		mov	carry, tmp3, lsr shift_r
		orr	carry, tmp4, lsl shift_l
		adcs	tmp2, carry
		mov	carry, tmp4, lsr shift_r
		stmia	ind1!, {tmp1, tmp2}
		sub	cnt, #2
		teq	cnt, #0
		bne	apfr_a_lp1

		@ propagate carry
apfr_a_lp2:	ldr	tmp1, [ind1]
		adcs	tmp1, carry
		str	tmp1, [ind1], #4
		mov	carry, #0
		bcs	apfr_a_lp2
		b	apfr_main_lp

		@ add
apfr_a_skip1:	msr	cpsr_f, #0
		tst	cnt, #1
		beq	apfr_a_lp3
		ldr	tmp1, [ind1]
		ldr	tmp3, [ind2], #4
		adcs	tmp1, tmp3
		str	tmp1, [ind1], #4
		sub	cnt, #1
		teq	cnt, #0
		beq	apfr_a_lp3_d
apfr_a_lp3:	ldmia	ind1, {tmp1, tmp2}
		ldmia	ind2!, {tmp3, tmp4}
		adcs	tmp1, tmp3
		adcs	tmp2, tmp4
		stmia	ind1!, {tmp1, tmp2}
		sub	cnt, #2
		teq	cnt, #0
		bne	apfr_a_lp3
apfr_a_lp3_d:	bcc	apfr_a_skip2

		@ propagate carry
apfr_a_lp4:	ldr	tmp1, [ind1]
		adcs	tmp1, #0
		str	tmp1, [ind1], #4
		bcs	apfr_a_lp4
apfr_a_skip2:	ldr	tmp1, [tab]
		cmp	tmp1, #0
		bne	apfr_main_lp
		b	apfr_again
		@ src -= dest << exp
apfr_subtract:	mvn	tmp2, tmp1
		cmp	tmp2, #BORDER
		bhi	apfr_mul
		mov	ind1, tmp2, lsr #5
		add	ind1, src, ind1, lsl #2
		ands	shift_l, tmp2, #31
		beq	apfr_s_skip1
		rsb	shift_r, shift_l, #32

		@ subtract with shift
		msr	cpsr_f, #(1<<29)
		mov	carry, #0
		tst	cnt, #1
		beq	apfr_s_lp1
		ldr	tmp1, [ind1]
		ldr	tmp3, [ind2], #4
		sbcs	tmp1, tmp3, lsl shift_l
		mov	carry, tmp3, lsr shift_r
		str	tmp1, [ind1], #4
		sub	cnt, #1
		teq	cnt, #0
		beq	apfr_s_lp2
apfr_s_lp1:	ldmia	ind1, {tmp1, tmp2}
		ldmia	ind2!, {tmp3, tmp4}
		orr	carry, tmp3, lsl shift_l
		sbcs	tmp1, carry
		mov	carry, tmp3, lsr shift_r
		orr	carry, tmp4, lsl shift_l
		sbcs	tmp2, carry
		mov	carry, tmp4, lsr shift_r
		stmia	ind1!, {tmp1, tmp2}
		sub	cnt, #2
		teq	cnt, #0
		bne	apfr_s_lp1

		@ propagate borrow
apfr_s_lp2:	ldr	tmp1, [ind1]
		sbcs	tmp1, carry
		str	tmp1, [ind1], #4
		mov	carry, #0
		bcc	apfr_s_lp2
		b	apfr_main_lp

		@ subtract
apfr_s_skip1:	msr	cpsr_f, #(1<<29)
		tst	cnt, #1
		beq	apfr_s_lp3
		ldr	tmp1, [ind1]
		ldr	tmp3, [ind2], #4
		sbcs	tmp1, tmp3
		str	tmp1, [ind1], #4
		sub	cnt, #1
		teq	cnt, #0
		beq	apfr_s_lp3_d
apfr_s_lp3:	ldmia	ind1, {tmp1, tmp2}
		ldmia	ind2!, {tmp3, tmp4}
		sbcs	tmp1, tmp3
		sbcs	tmp2, tmp4
		stmia	ind1!, {tmp1, tmp2}
		sub	cnt, #2
		teq	cnt, #0
		bne	apfr_s_lp3
apfr_s_lp3_d:	bcs	apfr_s_skip2

		@ propagate borrow
apfr_s_lp4:	ldr	tmp1, [ind1]
		sbcs	tmp1, #0
		str	tmp1, [ind1], #4
		bcc	apfr_s_lp4
apfr_s_skip2:	ldr	tmp1, [tab]
		cmp	tmp1, #~0
		bne	apfr_main_lp
		b	apfr_again

		@ src += dest * exp
apfr_mul:	mov	ind1, src
		mov	tab, tmp1
		mov	carry, #0
		tst	cnt, #1
		beq	apfr_u_lp1
		ldr	tmp1, [ind1]
		ldr	tmp3, [ind2], #4

		umull	shift_l, shift_r, tmp3, tab
		adds	tmp1, shift_l
		adc	carry, shift_r, #0

		str	tmp1, [ind1], #4
		sub	cnt, #1
		teq	cnt, #0
		beq	apfr_u_lp1_d

apfr_u_lp1:	ldmia	ind1, {tmp1, tmp2}
		ldmia	ind2!, {tmp3, tmp4}
		umull	shift_l, shift_r, tmp3, tab
		adds	shift_l, carry
		adc	shift_r, #0
		adds	tmp1, shift_l
		adc	carry, shift_r, #0

		umull	shift_l, shift_r, tmp4, tab
		adds	shift_l, carry
		adc	shift_r, #0
		adds	tmp2, shift_l
		adc	carry, shift_r, #0

		stmia	ind1!, {tmp1, tmp2}
		subs	cnt, #2
		bne	apfr_u_lp1

apfr_u_lp1_d:	ldr	tmp1, [ind1]
		adds	tmp1, carry
		str	tmp1, [ind1], #4
		bcc	apfr_again

		@ propagate carry
apfr_u_lp2:	ldr	tmp1, [ind1]
		adcs	tmp1, #0
		str	tmp1, [ind1], #4
		bcs	apfr_u_lp2

		@ dest = src(hi), src(hi) = 0
apfr_again:	mov	tmp3, #0
		mov	tmp4, #0
		mov	cnt, len
		mov	ind2, dest
		ldr	tab, [sp]
		ldr	tmp1, [tab]
		mov	ind1, tmp1, lsr #5
		add	ind1, src, ind1, lsl #2
		ands	shift_r, tmp1, #31
		beq	apfr_m_lp2
		rsb	shift_l, shift_r, #32
		ldr	tmp1, [ind1]
		mov	tmp2, tmp1, lsl shift_l
		lsr	tmp2, shift_l
		str	tmp2, [ind1], #4
		mov	carry, tmp1, lsr shift_r

		@ move with shift
apfr_m_lp1:	ldmia	ind1, {tmp1, tmp2}
		orr	carry, tmp1, lsl shift_l
		str	carry, [ind2], #4
		mov	carry, tmp1, lsr shift_r
		orr	carry, tmp2, lsl shift_l
		subs	cnt, #2
		strhs	carry, [ind2], #4
		movhs	carry, tmp2, lsr shift_r
		stmhsia	ind1!, {tmp3, tmp4}
		bhi	apfr_m_lp1
		strlo	tmp3, [ind1]
		b	apfr_c_lp1

		@ move
apfr_m_lp2:	ldmia	ind1, {tmp1, tmp2}
		subs	cnt, #2
		stmhsia	ind2!, {tmp1, tmp2}
		stmhsia	ind1!, {tmp3, tmp4}
		bhi	apfr_m_lp2
		strlo	tmp1, [ind2], #4
		strlo	tmp3, [ind1]

apfr_c_lp1:	ldr	tmp1, [ind2, #-4]!
		cmp	tmp1, #0
		bne	apfr_main_lp
		subs	len, #1
		bne	apfr_c_lp1

		@ dest = src(lo)
		pop	{tab, len}
apfr_d_lp1:	ldmia	src!, {tmp1, tmp2}
		subs	len, #2
		stmhsia	dest!, {tmp1, tmp2}
		bhi	apfr_d_lp1
		strlo	tmp1, [dest]

		pop	{r4-r11, r14}
		bx	lr

		.end
