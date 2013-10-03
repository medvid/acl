@ void acl_aes_cbc_de(vect4 out, vect4 in, vect exp_key, \
@                     size_t key_size, vect4 state);
@   decrypt 16 bytes in cbc mode (little endian)
@ on entry:
@   r0 = pointer to 16 output data bytes
@   r1 = pointer to 16 input data bytes
@   r2 = pointer to already expanded key
@   r3 = key size - 4: 128, 6: 192, 8: 256 (use constants ACL_xxx)
@   [sp] = pointer to 16 state bytes

		.global acl_aes_cbc_de
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
hlp0		.req	r8	@
hlp1		.req	r9	@
hlp2		.req	r10	@
hlp3		.req	r11	@
state		.req	r12	@ pointer to state

acl_aes_cbc_de:
		ldr	state, [sp]
		push	{r4-r11, r14}
		ldm	state, {hlp0, hlp1, hlp2, hlp3}
		push	{out, hlp0, hlp1, hlp2, hlp3}
		ldm	in, {st0, st1, st2, st3}
		stm	state, {st0, st1, st2, st3}
		bl	acl_aes_de
		pop	{out, hlp0, hlp1, hlp2, hlp3}
		eor	st0, hlp0
		eor	st1, hlp1
		eor	st2, hlp2
		eor	st3, hlp3
		stm	out, {st0, st1, st2, st3}
		pop	{r4-r11, r14}
		bx	lr

		.end
