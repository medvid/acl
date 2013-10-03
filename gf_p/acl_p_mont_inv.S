@ int acl_p_mont_inv(vect res, vect a, vect m, vect3 tmp, size_t len);
@   res = +- (a^-1) * (2^k) mod m (m must be odd)
@   returns 0 if a is non-invertible, +- k otherwise
@   a != 0 and a != 1
@ on entry:
@   r0 = pointer to result
@   r1 = pointer to a
@   r2 = pointer to m
@   r3 = pointer to temporary array (size: 3*len ints)
@   [sp] = length of input/output arrays in 32-bit words

		.global acl_p_mont_inv
		.text
		.arm

x1		.req	r0	@
aa		.req	r1	@
shift_r		.req	r1	@
mm		.req	r2	@
shift_l		.req	r2	@
uu		.req	r3	@
vv		.req	r4	@
x2		.req	r5	@
cnt		.req	r6	@
kay		.req	r7	@
len_x		.req	r8	@
len_u		.req	r9	@
tmp1		.req	r10	@
tmp2		.req	r11	@
swap		.req	r12	@

acl_p_mont_inv:	push	{r4-r11}
		mov	swap, #0
		ldr	len_u, [sp, #4*8]
		add	vv, uu, len_u, lsl #2
		add	x2, vv, len_u, lsl #2

		@ initialization
apmi_init:	mov	cnt, len_u
		mov	kay, #0
apmi_init_lp1:	subs	cnt, #1
		ldr	tmp1, [aa, cnt, lsl #2]
		str	tmp1, [uu, cnt, lsl #2]
		ldr	tmp2, [mm, cnt, lsl #2]
		str	tmp2, [vv, cnt, lsl #2]
		str	kay, [x1, cnt, lsl #2]
		str	kay, [x2, cnt, lsl #2]
		bne	apmi_init_lp1

		mov	cnt, #1
		str	cnt, [x1]
		mov	len_x, #1

		tst	tmp1, #1
		bne	apmi_compare
		b	apmi_u_again

apmi_v_bigger:	mov	tmp1, uu
		mov	uu, vv
		mov	vv, tmp1
		mov	tmp1, x1
		mov	x1, x2
		mov	x2, tmp1
		eor	swap, #1

		@ u = u - v
apmi_u_bigger:	mov	cnt, #0
		msr	cpsr_f, #(1<<29)
apmi_u_lp1:	ldr	tmp1, [uu, cnt, lsl #2]
		ldr	tmp2, [vv, cnt, lsl #2]
		sbcs	tmp1, tmp2
		str	tmp1, [uu, cnt, lsl #2]
		add	cnt, #1
		teq	cnt, len_u
		bne	apmi_u_lp1

		@ x1 = x1 + x2
		mov	cnt, #0
		msr	cpsr_f, #0
apmi_u_lp2:	ldr	tmp1, [x1, cnt, lsl #2]
		ldr	tmp2, [x2, cnt, lsl #2]
		adcs	tmp1, tmp2
		str	tmp1, [x1, cnt, lsl #2]
		add	cnt, #1
		teq	cnt, len_x
		bne	apmi_u_lp2

		@ make x1 and x2 longer?
		mov	tmp1, #1
		strcs	tmp1, [x1, len_x, lsl #2]
		addcs	len_x, #1

		@ count trailing zeroes of u
apmi_u_again:	ldr	tmp1, [uu]
		mov	shift_r, #0
		msr	cpsr_f, #(1<<29)
apmi_u_lp3:	movs	tmp1, tmp1, rrx
		addcc	shift_r, #1
		bcc	apmi_u_lp3
		rsb	shift_l, shift_r, #32
		add	kay, shift_r

		@ right shift u
		add	uu, len_u, lsl #2
		mov	cnt, len_u
		mov	tmp2, #0
apmi_u_lp4:	ldr	tmp1, [uu, #-4]
		orr	tmp2, tmp1, lsr shift_r
		str	tmp2, [uu, #-4]!
		mov	tmp2, tmp1, lsl shift_l
		subs	cnt, #1
		bne	apmi_u_lp4

		@ left shift x2
		mov	cnt, len_x
		mov	tmp2, #0
apmi_u_lp5:	ldr	tmp1, [x2]
		orr	tmp2, tmp1, lsl shift_r
		str	tmp2, [x2], #4
		mov	tmp2, tmp1, lsr shift_l
		subs	cnt, #1
		bne	apmi_u_lp5
		cmp	tmp2, #0
		addne	len_x, #1
		strne	tmp2, [x2], #4
		sub	x2, len_x, lsl #2

		@ shifted by 32 bits?
		cmp	shift_r, #32
		beq	apmi_u_again

		@ compare u and v
apmi_compare:	mov	shift_r, #0
		subs	cnt, len_u, #1
apmi_cmp_lp1:	ldr	tmp1, [uu, cnt, lsl #2]
		ldr	tmp2, [vv, cnt, lsl #2]
		cmp	tmp1, tmp2
		bhi	apmi_u_bigger
		blo	apmi_v_bigger
		orrs	shift_r, tmp1
		subeq	len_u, #1
		subs	cnt, #1
		bhs	apmi_cmp_lp1

		@ u (== v) == 1 ?
		cmp	len_u, #1
		bne	apmi_not_inv
		cmp	tmp1, #1
		bne	apmi_not_inv
		cmp	swap, #0
		beq	apmi_done

		ldr	cnt, [sp, #4*8]
apmi_mov_lp1:	subs	cnt, #1
		ldr	tmp1, [x1, cnt, lsl #2]
		str	tmp1, [x2, cnt, lsl #2]
		bne	apmi_mov_lp1
		rsb	kay, kay, #0

apmi_done:	mov	r0, kay
		pop	{r4-r11}
		bx	lr

apmi_not_inv:	mov	r0, #0
		pop	{r4-r11}
		bx	lr

		.end
