@ void acl_aes_ecb_de(vect4 out, vect4 in, vect exp_key, size_t key_size);
@   decrypt 16 bytes in ecb mode (little endian)
@ on entry:
@   r0 = pointer to 16 output data bytes
@   r1 = pointer to 16 input data bytes
@   r2 = pointer to already expanded key
@   r3 = key size - 4: 128, 6: 192, 8: 256 (use constants ACL_xxx)

		.global acl_aes_ecb_de
		.text
		.arm

out		.req	r0	@ outputs
in		.req	r1	@ inputs
key		.req	r2	@ expanded key
nk		.req	r3	@ key size
st0		.req	r4	@ aes state word 0
st1		.req	r5	@ aes state word 1
st2		.req	r6	@ aes state word 2
st3		.req	r7	@ aes state word 3

acl_aes_ecb_de:
		push	{r4-r11, r14}
		push	{out}
		ldm	in, {st0, st1, st2, st3}
		bl	acl_aes_de
		pop	{out}
		stm	out, {st0, st1, st2, st3}
		pop	{r4-r11, r14}
		bx	lr

		.end
