@ void acl_aes_key_de(vect key_out, vect key_in, size_t key_size);
@   expands aes decryption key key_in to key_out
@   assuming a little endian processor
@ on entry:
@   r0 = pointer to expanded key (output)
@   r1 = pointer to key (input)
@   r2 = key size - 4: 128, 6: 192, 8: 256 (use constants ACL_xxx)

		.global acl_aes_key_de
		.text
		.arm

ptr		.req	r0	@ expanded key
acc		.req	r1	@ accumulator
nk		.req	r2	@ nk
ff		.req	r3	@ mask value
tmp		.req	r4	@ temp
st0		.req	r5	@ temp
st1		.req	r6	@ temp
sbox		.req	r7	@ pointer to sbox
inv_table	.req	r12	@ pointer to inverse table

acl_aes_key_de:
		push	{r0, r2, r14}
		bl	acl_aes_key_en
		pop	{r0, r2, r14}
		push	{r4-r7}
		ldr	sbox, =acl_aes_fwd_sbox
		ldr	inv_table, =acl_aes_inv_table
		mov	ff, #0xff
		lsl	nk, #2
		add	nk, #20
		add	ptr, #16
aakd_lp:	ldr	tmp, [ptr]
		and	st0, ff, tmp
		and	st1, ff, tmp, lsr #8
		ldrb	st0, [sbox, st0]
		ldrb	st1, [sbox, st1]
		ldr	acc, [inv_table, st0, lsl #2]
		ldr	st1, [inv_table, st1, lsl #2]
		eor	acc, st1, ror #24
		and	st0, ff, tmp, lsr #16
		mov	st1, tmp, lsr #24
		ldrb	st0, [sbox, st0]
		ldrb	st1, [sbox, st1]
		ldr	st0, [inv_table, st0, lsl #2]
		ldr	st1, [inv_table, st1, lsl #2]
		eor	acc, st0, ror #16
		eor	acc, st1, ror #8
		str	acc, [ptr], #4
		subs	nk, #1
		bne	aakd_lp
		pop	{r4-r7}
		bx	lr

		.end
