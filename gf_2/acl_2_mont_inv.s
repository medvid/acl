@ int acl_2_mont_inv(vect res, vect a, vect poly, vect3 tmp, size_t len);
@   res = (a^-1) * (z^k) mod poly (poly mod z == 1)
@   returns 0 if a is non-invertible, k otherwise
@   a != 0 and a != 1
@ on entry:
@   r0 = pointer to result
@   r1 = pointer to a
@   r2 = pointer to reduction polynomial
@   r3 = pointer to temporary array (size: 3*len ints)
@   [sp] = length of input/output arrays in 32-bit words

		.global acl_2_mont_inv
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

acl_2_mont_inv:	push	{r4-r11}
		mov	swap, #0
		ldr	len_u, [sp, #4*8]
		add	vv, uu, len_u, lsl #2
		add	x2, vv, len_u, lsl #2

		@ initialization
		mov	cnt, len_u
		mov	kay, #0
a2mi_init_lp1:	subs	cnt, #1
		ldr	tmp1, [aa, cnt, lsl #2]
		str	tmp1, [uu, cnt, lsl #2]
		ldr	tmp2, [mm, cnt, lsl #2]
		str	tmp2, [vv, cnt, lsl #2]
		str	kay, [x1, cnt, lsl #2]
		str	kay, [x2, cnt, lsl #2]
		bne	a2mi_init_lp1

		mov	cnt, #1
		str	cnt, [x1]
		mov	len_x, #1

		tst	tmp1, #1
		bne	a2mi_compare
		b	a2mi_u_again

a2mi_v_bigger:	mov	tmp1, uu
		mov	uu, vv
		mov	vv, tmp1
		mov	tmp1, x1
		mov	x1, x2
		mov	x2, tmp1
		eor	swap, #1

		@ u = u + v
a2mi_u_bigger:	mov	cnt, len_u
a2mi_u_lp1:	subs	cnt, #1
		ldr	tmp1, [uu, cnt, lsl #2]
		ldr	tmp2, [vv, cnt, lsl #2]
		eor	tmp1, tmp2
		str	tmp1, [uu, cnt, lsl #2]
		bne	a2mi_u_lp1

		@ x1 = x1 + x2
		mov	cnt, len_x
a2mi_u_lp2:	subs	cnt, #1
		ldr	tmp1, [x1, cnt, lsl #2]
		ldr	tmp2, [x2, cnt, lsl #2]
		eor	tmp1, tmp2
		str	tmp1, [x1, cnt, lsl #2]
		bne	a2mi_u_lp2

		@ count trailing zeroes of u
a2mi_u_again:	ldr	tmp1, [uu]
		mov	shift_r, #0
		msr	cpsr_f, #(1<<29)
a2mi_u_lp3:	movs	tmp1, tmp1, rrx
		addcc	shift_r, #1
		bcc	a2mi_u_lp3
		rsb	shift_l, shift_r, #32
		add	kay, shift_r

		@ right shift u
		add	uu, len_u, lsl #2
		mov	cnt, len_u
		mov	tmp2, #0
a2mi_u_lp4:	ldr	tmp1, [uu, #-4]
		orr	tmp2, tmp1, lsr shift_r
		str	tmp2, [uu, #-4]!
		mov	tmp2, tmp1, lsl shift_l
		subs	cnt, #1
		bne	a2mi_u_lp4

		@ left shift x2
		mov	cnt, len_x
		mov	tmp2, #0
a2mi_u_lp5:	ldr	tmp1, [x2]
		orr	tmp2, tmp1, lsl shift_r
		str	tmp2, [x2], #4
		mov	tmp2, tmp1, lsr shift_l
		subs	cnt, #1
		bne	a2mi_u_lp5
		cmp	tmp2, #0
		addne	len_x, #1
		strne	tmp2, [x2], #4
		sub	x2, len_x, lsl #2

		@ shifted by 32 bits?
		cmp	shift_l, #0
		beq	a2mi_u_again

		@ u == 1 ?
a2mi_compare:	ldr	tmp1, [uu]
		cmp	tmp1, #1
		beq	a2mi_cmp_one

		@ compare u and v
a2mi_not_one:	mov	shift_r, #0
		subs	cnt, len_u, #1
a2mi_cmp_lp1:	ldr	tmp1, [uu, cnt, lsl #2]
		ldr	tmp2, [vv, cnt, lsl #2]
		cmp	tmp1, tmp2
		bhi	a2mi_u_bigger
		blo	a2mi_v_bigger
		orrs	shift_r, tmp1
		subeq	len_u, #1
		subs	cnt, #1
		bhs	a2mi_cmp_lp1

a2mi_not_inv:	mov	r0, #-1
		pop	{r4-r11}
		bx	lr

		@ u == 1 ?
a2mi_cmp_one:	mov	cnt, len_u
a2mi_cmp_lp2:	subs	cnt, #1
		beq	a2mi_done1
		ldr	tmp1, [uu, cnt, lsl #2]
		cmp	tmp1, #0
		bne	a2mi_not_one
		b	a2mi_cmp_lp2

a2mi_done1:	teq	swap, #0
		beq	a2mi_done2

		@ x2 = x1
		ldr	cnt, [sp, #4*8]
a2mi_mov_lp1:	subs	cnt, #1
		ldr	tmp1, [x1, cnt, lsl #2]
		str	tmp1, [x2, cnt, lsl #2]
		bne	a2mi_mov_lp1

a2mi_done2:	mov	r0, kay
		pop	{r4-r11}
		bx	lr

		.end
