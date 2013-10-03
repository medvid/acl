@ void acl_p_mod_hlv(vect a, uint k, vect m, size_tlen);
@   k times: a = (a + m)/2 mod m or res = (a/2) mod m
@ on entry:
@   r0 = pointer to input/result
@   r1 = k
@   r2 = pointer to m
@   r3 = length of input/output arrays in 32-bit words

		.global acl_p_mod_hlv
		.text
		.arm

dest		.req	r0	@
kay		.req	r1	@
emm		.req	r2	@
len		.req	r3	@
tmp1		.req	r4	@
tmp2		.req	r5	@
cnt		.req	r12	@

acl_p_mod_hlv:	push	{r4-r5}
		ldr	tmp1, [dest]

apmh_again:	mov	cnt, len
		msr	cpsr_f, #0
		tst	tmp1, #1
		beq	apmh_lp2

		@ a = a + m
		mov	cnt, #0
apmh_lp1:	ldr	tmp1, [dest, cnt, lsl #2]
		ldr	tmp2, [emm, cnt, lsl #2]
		adcs	tmp1, tmp2
		str	tmp1, [dest, cnt, lsl #2]
		add	cnt, #1
		teq	cnt, len
		bne	apmh_lp1

		@ a = a/2
apmh_lp2:	sub	cnt, #1
		ldr	tmp1, [dest, cnt, lsl #2]
		movs	tmp1, tmp1, rrx
		str	tmp1, [dest, cnt, lsl #2]
		teq	cnt, #0
		bne	apmh_lp2

		subs	kay, #1
		bne	apmh_again
		pop	{r4-r5}
		bx	lr

		.end
