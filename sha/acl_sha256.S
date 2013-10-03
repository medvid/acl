		.global	acl_sha224_init
		.global	acl_sha256_init
		.global	acl_sha256
		.global	acl_sha256_done
		.text
		.arm

@ based on Federal Information Processing Standards Publication 180-2
@       (+ Change Notice to include SHA-224)
@ and on http://en.wikipedia.org/wiki/SHA_hash_functions
@ which translates very well into ARM code

@ throughout this file:
@   state[0..7] == hash state
@   state[8..23] == tmp storage
@   state[24..25] == length

acl_sha256_table:
		.int	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5
		.int	0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5
		.int	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3
		.int	0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174
		.int	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc
		.int	0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da
		.int	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7
		.int	0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967
		.int	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13
		.int	0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85
		.int	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3
		.int	0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070
		.int	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5
		.int	0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3
		.int	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208
		.int	0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2

@ void acl_sha224_init(vect26 state);
@   initialize sha-224 hash
@ on entry:
@   r0 = pointer to state

st		.req	r0	@
cnt		.req	r1	@
tmp		.req	r2	@
tab		.req	r3	@

acl_sha224_init_table:
		.int	0xc1059ed8, 0x367cd507, 0x3070dd17, 0xf70e5939
		.int	0xffc00b31, 0x68581511, 0x64f98fa7, 0xbefa4fa4

acl_sha224_init:
		adr	tab, acl_sha224_init_table
		b	as256i_entry

@ void acl_sha256_init(vect26 state);
@   initialize sha-256 hash
@ on entry:
@   r0 = pointer to state

acl_sha256_init_table:
		.int	0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a
		.int	0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19

acl_sha256_init:
		adr	tab, acl_sha256_init_table
as256i_entry:	mov	cnt, #8
as256i_lp:	ldr	tmp, [tab], #4
		str	tmp, [st], #4
		subs	cnt, #1
		bne	as256i_lp
		add	st, #4*16
		mov	tmp, #0
		str	tmp, [st], #4
		str	tmp, [st]
		bx	lr

		.unreq	st
		.unreq	cnt
		.unreq	tmp
		.unreq	tab

@ void acl_sha256(vect26 state, byte data);
@   update sha-256 hash with byte "data"
@ on entry:
@   r0 = pointer to state
@   r1 = next byte to update the hash

st		.req	r0	@
chr		.req	r1	@
buf		.req	r2	@
len		.req	r3	@
tmp0		.req	r12	@
aa		.req	r1	@
bb		.req	r2	@
cc		.req	r3	@
dd		.req	r4	@
ee		.req	r5	@
ff		.req	r6	@
gg		.req	r7	@
hh		.req	r8	@
rnd		.req	r9	@
acc		.req	r10	@
tmp1		.req	r11	@
tmp2		.req	r12	@
tmp3		.req	r14	@

as256c_big:	ldr	len, [st, #4*24]
		add	len, #1
		str	len, [st, #4*24]
		b	as256_core

acl_sha256:	add	buf, st, #4*8
		ldr	len, [st, #4*25]
		eor	tmp0, len, #3 << 3
		lsl	tmp0, #23
		strb	chr, [buf, tmp0, lsr #26]
		adds	len, #8
		str	len, [st, #4*25]
		bcs	as256c_big
		tst	len, #63 << 3
		bxne	lr

as256_core:	push	{r4-r11, r14}
		ldmia	st!, {aa, bb, cc, dd, ee, ff, gg, hh}
		mov	rnd, #0

		@	get w(rnd)
as256c_main_lp:	mov	tmp1, rnd, lsl #28
		ldr	acc, [st, tmp1, lsr #26]
		cmp	rnd, #16
		blo	as256c_skip
		add	tmp1, #1 << 28
		ldr	tmp2, [st, tmp1, lsr #26]
		mov	tmp3, tmp2, ror #7
		eor	tmp3, tmp2, ror #18
		eor	tmp3, tmp2, lsr #3
		add	acc, tmp3
		add	tmp1, #8 << 28
		ldr	tmp2, [st, tmp1, lsr #26]
		add	acc, tmp2
		add	tmp1, #5 << 28
		ldr	tmp2, [st, tmp1, lsr #26]
		mov	tmp3, tmp2, ror #17
		eor	tmp3, tmp2, ror #19
		eor	tmp3, tmp2, lsr #10
		add	acc, tmp3
		mov	tmp1, rnd, lsl #28
		str	acc, [st, tmp1, lsr #26]

		@	get k(rnd), ch, s1, hh -> tmp1
as256c_skip:	adr	tmp1, acl_sha256_table
		ldr	tmp1, [tmp1, rnd, lsl #2]
		add	acc, tmp1
		and	tmp1, ee, ff
		bic	tmp2, gg, ee
		eor	tmp1, tmp2
		add	acc, tmp1
		mov	tmp1, ee, ror #6
		eor	tmp1, ee, ror #11
		eor	tmp1, ee, ror #25
		add	acc, tmp1
		add	tmp1, acc, hh

		@	get s0, maj -> tmp2
		mov	acc, aa, ror #2
		eor	acc, aa, ror #13
		eor	acc, aa, ror #22
		and	tmp2, aa, bb
		eor	tmp3, aa, bb
		and	tmp3, cc
		eor	tmp2, tmp3
		add	tmp2, acc

		mov	hh, gg
		mov	gg, ff
		mov	ff, ee
		add	ee, dd, tmp1
		mov	dd, cc
		mov	cc, bb
		mov	bb, aa
		add	aa, tmp1, tmp2

		add	rnd, #1
		cmp	rnd, #64
		bne	as256c_main_lp

		ldmdb	st!, {rnd, acc, tmp1, tmp2}
		add	ee, rnd
		add	ff, acc
		add	gg, tmp1
		add	hh, tmp2
		ldmdb	st!, {rnd, acc, tmp1, tmp2}
		add	aa, rnd
		add	bb, acc
		add	cc, tmp1
		add	dd, tmp2
		stmia	st, {aa, bb, cc, dd, ee, ff, gg, hh}

		pop	{r4-r11, r14}
		bx	lr

@ void acl_sha256_done(vect26 state);
@   finish sha-256 hash - the result is in state[0..4]
@ on entry:
@   r0 = pointer to state

		len1	.req r1 @
		len2	.req r3 @

acl_sha256_done:
		push	{lr}
		ldr	len1, [st, #4*25]
		ldr	len2, [st, #4*24]
		push	{len1, len2}
		mov	chr, #0x80
		b	as256d_entry

as256d_lp1:	mov	chr, #0
as256d_entry:	bl	acl_sha256
		ldr	len1, [st, #4*25]
		lsr	len1, #3
		tst	len1, #3
		bne	as256d_lp1
		and	len1, #63
		cmp	len1, #56
		bhi	as256d_lp1
		add	buf, st, #4*8
		beq	as256d_done
		mov	len2, #0
as256d_lp2:	str	len2, [buf, len1]
		add	len1, #4
		cmp	len1, #56
		bne	as256d_lp2

as256d_done:	pop	{len1, len2}
		str	len1, [buf, #4*15]
		str	len2, [buf, #4*14]
		bl	as256_core
#ifdef LITTLE_ENDIAN
		mov	len1, #8
		bl	acl_rev_bytes
#endif
		pop	{lr}
		bx	lr

		.end
