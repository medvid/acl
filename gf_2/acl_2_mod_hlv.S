@ void acl_2_mod_hlv(vect a, uint k, vect poly, size_t len);
@   k times: if a mod z == 1 then a = (a + poly)/z mod poly
@                            else a = (a/z) mod poly
@   poly mod z must be equal to 1
@ on entry:
@   r0 = pointer to input/result
@   r1 = number of times to halve
@   r2 = pointer to reduction polynomial
@   r3 = length of input/output arrays in 32-bit words

		.global acl_2_mod_hlv
		.text
		.arm

dest		.req	r0	@
kay		.req	r1	@
poly		.req	r2	@
len		.req	r3	@
tmp1		.req	r4	@
tmp2		.req	r5	@
cnt		.req	r12	@

acl_2_mod_hlv:	push	{r4-r5}

a2mh_again:	ldr	tmp1, [dest]
		mov	cnt, len
		msr	cpsr_f, #0
		tst	tmp1, #1
		beq	a2mh_lp2

		@ a = (a + poly)/z
a2mh_lp1:	sub	cnt, #1
		ldr	tmp1, [dest, cnt, lsl #2]
		ldr	tmp2, [poly, cnt, lsl #2]
		eor	tmp1, tmp2
		movs	tmp1, tmp1, rrx
		str	tmp1, [dest, cnt, lsl #2]
		teq	cnt, #0
		bne	a2mh_lp1
		subs	kay, #1
		bne	a2mh_again
		b	a2mh_ret

		@ a = a/z
a2mh_lp2:	sub	cnt, #1
		ldr	tmp1, [dest, cnt, lsl #2]
		movs	tmp1, tmp1, rrx
		str	tmp1, [dest, cnt, lsl #2]
		teq	cnt, #0
		bne	a2mh_lp2
		subs	kay, #1
		bne	a2mh_again
a2mh_ret:	pop	{r4-r5}
		bx	lr

		.end
