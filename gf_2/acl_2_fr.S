@ void acl_2_fr(vect res, vect2 a, list data, size_t len);
@   res = a mod (z^exp1 + z^exp2 + ... + z^0)
@         does not work in-place (res != a)
@
@ the exponents are listed (highest exponent mustbe first)
@ in a null-terminated table pointed to by data;
@ the final zero is also considered an exponent
@ - thus all polynomials must end with z^0
@
@ examples of lists:
@     113, 9, 0
@     131, 8, 3, 2, 0
@     163, 7, 6, 3, 0
@     193, 15, 0
@     233, 74, 0
@     239, 158, 0
@     283, 12, 7, 5, 0
@     409, 87, 0
@     571, 10, 5, 2, 0
@
@ on entry:
@   r0 = pointer to result
@   r1 = pointer to input
@   r2 = pointer to exponent table
@   r3 = length of input/output arrays in 32-bit words

		.global acl_2_fr
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

acl_2_fr:	push	{r4-r11, r14}
		push	{tab, len}
		b	a2fr_entry

		@ src ^= dest << exp
a2fr_main_lp:	mov	ind2, dest
		mov	cnt, len
		ldr	tmp1, [tab, #4]!
		mov	ind1, tmp1, lsr #5
		add	ind1, src, ind1, lsl #2
		ands	shift_l, tmp1, #31
		beq	a2fr_x_lp2
		rsb	shift_r, shift_l, #32

		@ xor with shift
		mov	carry, #0
a2fr_x_lp1:	ldmia	ind1, {tmp1, tmp2}
		ldmia	ind2!, {tmp3, tmp4}
		eor	tmp1, carry
		eor	tmp1, tmp3, lsl shift_l
		mov	carry, tmp3, lsr shift_r
		eor	tmp2, carry
		eor	tmp2, tmp4, lsl shift_l
		subs	cnt, #2
		movhs	carry, tmp4, lsr shift_r
		stmhsia	ind1!, {tmp1, tmp2}
		bhi	a2fr_x_lp1
		strlo	tmp1, [ind1], #4

		ldr	tmp1, [ind1]
		eor	tmp1, carry
		str	tmp1, [ind1]
		b	a2fr_main_lp

		@ xor
a2fr_x_lp2:	ldmia	ind1, {tmp1, tmp2}
		ldmia	ind2!, {tmp3, tmp4}
		eor	tmp1, tmp3
		eor	tmp2, tmp4
		subs	cnt, #2
		stmhsia	ind1!, {tmp1, tmp2}
		bhi	a2fr_x_lp2
		strlo	tmp1, [ind1]

		ldr	tmp1, [tab]
		cmp	tmp1, #0
		bne	a2fr_main_lp

		@ dest = src(hi), src(hi) = 0
a2fr_entry:	mov	ind2, dest
		ldr	tab, [sp]
		ldr	tmp1, [tab]
		mov	ind1, tmp1, lsr #5
		add	ind1, src, ind1, lsl #2
		and	shift_r, tmp1, #31
		rsb	shift_l, shift_r, #32
		ldr	tmp1, [ind1]
		mov	tmp2, tmp1, lsl shift_l
		lsr	tmp2, shift_l
		str	tmp2, [ind1], #4
		mov	carry, tmp1, lsr shift_r

		@ move with shift
		mov	tmp3, #0
		mov	tmp4, #0
		mov	cnt, len
a2fr_m_lp1:	ldmia	ind1, {tmp1, tmp2}
		orr	carry, tmp1, lsl shift_l
		str	carry, [ind2], #4
		mov	carry, tmp1, lsr shift_r
		orr	carry, tmp2, lsl shift_l
		subs	cnt, #2
		strhs	carry, [ind2], #4
		movhs	carry, tmp2, lsr shift_r
		stmhsia	ind1!, {tmp3, tmp4}
		bhi	a2fr_m_lp1
		strlo	tmp3, [ind1], #4

a2fr_c_lp1:	ldr	tmp1, [ind2, #-4]!
		cmp	tmp1, #0
		bne	a2fr_main_lp
		subs	len, #1
		bne	a2fr_c_lp1

		@ dest = src(lo)
		pop	{tab, len}
a2fr_d_lp1:	ldmia	src!, {tmp1, tmp2}
		subs	len, #2
		stmhsia	dest!, {tmp1, tmp2}
		bhi	a2fr_d_lp1
		strlo	tmp1, [dest]

		pop	{r4-r11, r14}
		bx	lr

		.end
