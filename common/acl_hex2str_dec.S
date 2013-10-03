@ void acl_hex2str_dec(bytes res, size_t len_r, vect a, size_t len);
@   converts a[len] to a decimal string in res[len_r]
@ on entry:
@   r0 = pointer to result
@   r1 = number of characters in res
@   r2 = pointer to input
@   r3 = length of input array in 32-bit words

		.global acl_hex2str_dec
		.text
		.arm

dest		.req	r0	@
len_d		.req	r1	@
src		.req	r2	@
len		.req	r3	@
cnt		.req	r4	@
tmp		.req	r5	@
ind_s		.req	r6	@
ind_d		.req	r7	@
carry		.req	r12	@

acl_hex2str_dec:
		push	{r4-r7}

		@ clear accumulator
		mov	cnt, len_d
		mov	tmp, #0
ah2sd_init_lp1:	subs	cnt, #1
		strb	tmp, [dest, cnt]
		bne	ah2sd_init_lp1

		mov	ind_s, len, lsl #3
		sub	ind_s, #1
		b	ah2sd_entry

		@ mul by 16 and adjust
ah2sd_main_lp:	sub	ind_d, len_d, #1
		mov	carry, #0
ah2sd_adj_lp1:	ldrb	tmp, [dest, ind_d]
		add	tmp, carry, tmp, lsl #4
		mov	carry, #0
		mov	cnt, #16
ah2sd_adj_lp2:	cmp	tmp, #10*16
		subhs	tmp, #10*16
		addhs	carry, cnt
		lsrs	cnt, #1
		lslne	tmp, #1
		bne	ah2sd_adj_lp2
		lsr	tmp, #4
		strb	tmp, [dest, ind_d]
		subs	ind_d, #1
		bhs	ah2sd_adj_lp1

		@ fetch next nibble (msb first)
ah2sd_entry:	ldrb	carry, [src, ind_s, lsr #1]
		tst	ind_s, #1
		lsrne	carry, #4
		and	carry, #0x0f

		@ add it to the lsb
		sub	ind_d, len_d, #1
ah2sd_adj_lp3:	ldrb	tmp, [dest, ind_d]
		add	tmp, carry
		mov	carry, #0
ah2sd_adj_lp4:	cmp	tmp, #10
		subhs	tmp, #10
		addhs	carry, #1
		bhi	ah2sd_adj_lp4
		strb	tmp, [dest, ind_d]
		cmp	carry, #0
		beq	ah2sd_adj_done
		subs	ind_d, #1
		bhs	ah2sd_adj_lp3

ah2sd_adj_done:	subs	ind_s, #1
		bhs	ah2sd_main_lp
		@ convert to characters
		mov	cnt, #0
		mov	carry, #' '
ah2sd_end_lp1:	ldrb	tmp, [dest, cnt]
		cmp	tmp, #0
		bne	ah2sd_end_ent
		strb	carry, [dest, cnt]
		add	cnt, #1
		cmp	cnt, len_d
		bne	ah2sd_end_lp1

		@ entire number is zero
		sub	cnt, #1
		mov	tmp, #'0'
		strb	tmp, [dest, cnt]
		b	ah2sd_done

ah2sd_end_lp2:	ldrb	tmp, [dest, cnt]
ah2sd_end_ent:	add	tmp, #'0'
		strb	tmp, [dest, cnt]
		add	cnt, #1
		cmp	cnt, len_d
		bne	ah2sd_end_lp2

ah2sd_done:	pop	{r4-r7}
		bx	lr

		.end
