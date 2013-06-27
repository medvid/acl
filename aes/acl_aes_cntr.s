@ void acl_aes_cntr(vect4 out, vect4 in, vect exp_key, \
@                   size_t key_size, vect4 counter);
@   encrypt/decrypt 16 bytes in counter mode (little endian)
@ on entry:
@   r0 = pointer to 16 output data bytes
@   r1 = pointer to 16 input data bytes
@   r2 = pointer to already expanded key
@   r3 = key size - 4: 128, 6: 192, 8: 256 (use constants ACL_xxx)
@   [sp] = pointer to 16 counter bytes

		.global	acl_aes_cntr
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
cntr		.req	r12	@ pointer to counter

acl_aes_cntr:	ldr	cntr, [sp]
		push	{r4-r11, r14}
		push	{out, in}
		ldm	cntr, {st0, st1, st2, st3}
		adds	hlp0, st0, #1
		adcs	hlp1, st1, #0
		adcs	hlp2, st2, #0
		adcs	hlp3, st3, #0
		stm	cntr, {hlp0, hlp1, hlp2, hlp3}
		bl	acl_aes_en
		pop	{out, in}
		ldm	in, {hlp0, hlp1, hlp2, hlp3}
		eor	hlp0, st0
		eor	hlp1, st1
		eor	hlp2, st2
		eor	hlp3, st3
		stm	out, {hlp0, hlp1, hlp2, hlp3}
		pop	{r4-r11, r14}
		bx	lr

		.end
