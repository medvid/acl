@ void acl_aes_key_en(vect key_out, vect key_in, size_t key_size);
@   expands aes encryption key key_in to key_out
@   assuming a little endian processor
@ on entry:
@   r0 = pointer to expanded key (output)
@   r1 = pointer to key (input)
@   r2 = key size - 4: 128, 6: 192, 8: 256 (use constants ACL_xxx)

		.global acl_aes_key_en
		.text
		.arm

out		.req	r0	@ outputs
in		.req	r1	@ inputs
nk		.req	r2	@ nk
ff		.req	r3	@ mask value
tmp		.req	r4	@ temp
cnt		.req	r5	@ loop counter
sbox		.req	r6	@ pointer to sbox
rcon		.req	r7	@ pointer to rcon
rnd		.req	r8	@ round counter
acc		.req	r9	@ accumulator
st		.req	r12	@ substitution

acl_aes_key_en:
		push	{r4-r9}
		ldr	sbox, =acl_aes_fwd_sbox
		ldr	rcon, =acl_aes_rcon
		mov	ff, #0xff

		@ copy key from in to out
		mov	cnt, nk
aake_lp:	ldr	acc, [in], #4
		str	acc, [out], #4
		subs	cnt, #1
		bne	aake_lp

		@ number of rounds = 3*nk + 28 (cnt==0)
		add	rnd, nk, nk, lsl #1
		add	rnd, #28

		@ rnd mod nk == 0
aake_zero:	ldr	st, [out, #-4]
		ror	st, #8
		and	tmp, ff, st
		ldrb	acc, [sbox, tmp]
		and	tmp, ff, st, lsr #8
		ldrb	tmp, [sbox, tmp]
		orr	acc, tmp, lsl #8
		and	tmp, ff, st, lsr #16
		ldrb	tmp, [sbox, tmp]
		orr	acc, tmp, lsl #16
		mov	tmp, st, lsr #24
		ldrb	tmp, [sbox, tmp]
		orr	acc, tmp, lsl #24
		ldrb	tmp, [rcon], #1
		eor	acc, tmp
		b	aake_drain

		@ nk > 6 ?
aake_cmp_6:	cmp	nk, #6
		bls	aake_drain
		@ (rnd mod nk == 4) and (nk > 6)
		ldr	st, [out, #-4]
		and	tmp, ff, st
		ldrb	acc, [sbox, tmp]
		and	tmp, ff, st, lsr #8
		ldrb	tmp, [sbox, tmp]
		orr	acc, tmp, lsl #8
		and	tmp, ff, st, lsr #16
		ldrb	tmp, [sbox, tmp]
		orr	acc, tmp, lsl #16
		mov	tmp, st, lsr #24
		ldrb	tmp, [sbox, tmp]
		orr	acc, tmp, lsl #24
		b	aake_drain

		@ rnd mod nk == 4 ?
aake_try_4:	cmp	cnt, #4
		beq	aake_cmp_6
		@ rnd mod nk != 4
aake_drain:	ldr	tmp, [out, -nk, lsl #2]
		eor	acc, tmp
		str	acc, [out], #4
		subs	rnd, #1
		beq	aake_done
		add	cnt, #1
		cmp	cnt, nk
		bne	aake_try_4
		mov	cnt, #0
		b	aake_zero

aake_done:	pop	{r4-r9}
		bx	lr
		.end
