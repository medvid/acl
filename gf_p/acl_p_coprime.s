@ bool_t acl_p_coprime(vect a, vect b, vect2 tmp, size_t len);
@   returns true if gcd(a,b) == 1
@ on entry:
@   r0 = pointer to a
@   r1 = pointer to b
@   r2 = pointer to temporary array (2 x len 32-bitwords)
@   r3 = length of input/output arrays in 32-bit words

		.global acl_p_coprime
		.text
		.arm

aa		.req	r0	@
shift_r		.req	r0	@
bb		.req	r1	@
shift_l		.req	r1	@
uu		.req	r2	@
len		.req	r3	@
vv		.req	r4	@
tmp1		.req	r5	@
tmp2		.req	r6	@
cnt		.req	r12	@

acl_p_coprime:	push	{r4-r6}
		add	vv, uu, len, lsl #2

		@ initialization
		sub	cnt, len, #1
apco_init_lp:	ldr	tmp1, [aa, cnt, lsl #2]
		str	tmp1, [uu, cnt, lsl #2]
		ldr	tmp2, [bb, cnt, lsl #2]
		str	tmp2, [vv, cnt, lsl #2]
		subs	cnt, #1
		bhs	apco_init_lp

		@ if both u and v are even
		orr	cnt, tmp1, tmp2
		tst	cnt, #1
		beq	apco_no

		@ if both u and v are odd
		and	cnt, tmp1, tmp2
		tst	cnt, #1
		bne	apco_compare

		@ if u is even and v is odd
		tst	tmp1, #1
		beq	apco_u_again

		@ if u is odd and v is even
		mov	tmp1, uu
		mov	uu, vv
		mov	vv, tmp1
		b	apco_u_again

		@ swap u and v
apco_v_bigger:	mov	tmp1, uu
		mov	uu, vv
		mov	vv, tmp1

		@ u = u - v
apco_u_bigger:	mov	cnt, #0
		msr	cpsr_f, #(1<<29)
apco_u_lp1:	ldr	tmp1, [uu, cnt, lsl #2]
		ldr	tmp2, [vv, cnt, lsl #2]
		sbcs	tmp1, tmp2
		str	tmp1, [uu, cnt, lsl #2]
		add	cnt, #1
		teq	cnt, len
		bne	apco_u_lp1

		@ count trailing zeroes of u
apco_u_again:	ldr	tmp1, [uu]
		mov	shift_r, #0
		msr	cpsr_f, #(1<<29)
apco_u_lp2:	movs	tmp1, tmp1, rrx
		addcc	shift_r, #1
		bcc	apco_u_lp2
		rsb	shift_l, shift_r, #32

		@ right shift u
		add	uu, len, lsl #2
		mov	cnt, len
		mov	tmp2, #0
apco_u_lp3:	ldr	tmp1, [uu, #-4]
		orr	tmp2, tmp1, lsr shift_r
		str	tmp2, [uu, #-4]!
		mov	tmp2, tmp1, lsl shift_l
		subs	cnt, #1
		bne	apco_u_lp3

		@ shifted by 32 bits?
		cmp	shift_r, #32
		beq	apco_u_again

		@ compare u and v
apco_compare:	mov	shift_r, #0
		subs	cnt, len, #1
apco_cmp_lp1:	ldr	tmp1, [uu, cnt, lsl #2]
		ldr	tmp2, [vv, cnt, lsl #2]
		cmp	tmp1, tmp2
		bhi	apco_u_bigger
		blo	apco_v_bigger
		orrs	shift_r, tmp1
		subeq	len, #1
		subs	cnt, #1
		bhs	apco_cmp_lp1

		@ u (== v) == 1 ?
		cmp	len, #1
		bne	apco_no
		cmp	tmp1, #1
		bne	apco_no
		mov	r0, #-1
		pop	{r4-r6}
		bx	lr

apco_no:	mov	r0, #0
		pop	{r4-r6}
		bx	lr

		.end
