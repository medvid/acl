		.global acl_sha384_init
		.global acl_sha512_init
		.global acl_sha512
		.global acl_sha512_done
		.text
		.arm

@ based on Federal Information Processing StandardsPublication 180-2
@       (+ Change Notice to include SHA-224)
@ and on http://en.wikipedia.org/wiki/SHA_hash_functions
@ which translates very well into ARM code

@ throughout this file:
@ state[0..15] == hash state
@ state[16..31] == a-h
@ state[32..63] == tmp storage
@ state[64..67] == length

acl_sha512_table:
		.int	0x428a2f98, 0xd728ae22, 0x71374491, 0x23ef65cd
		.int	0xb5c0fbcf, 0xec4d3b2f, 0xe9b5dba5, 0x8189dbbc
		.int	0x3956c25b, 0xf348b538, 0x59f111f1, 0xb605d019
		.int	0x923f82a4, 0xaf194f9b, 0xab1c5ed5, 0xda6d8118
		.int	0xd807aa98, 0xa3030242, 0x12835b01, 0x45706fbe
		.int	0x243185be, 0x4ee4b28c, 0x550c7dc3, 0xd5ffb4e2
		.int	0x72be5d74, 0xf27b896f, 0x80deb1fe, 0x3b1696b1
		.int	0x9bdc06a7, 0x25c71235, 0xc19bf174, 0xcf692694
		.int	0xe49b69c1, 0x9ef14ad2, 0xefbe4786, 0x384f25e3
		.int	0x0fc19dc6, 0x8b8cd5b5, 0x240ca1cc, 0x77ac9c65
		.int	0x2de92c6f, 0x592b0275, 0x4a7484aa, 0x6ea6e483
		.int	0x5cb0a9dc, 0xbd41fbd4, 0x76f988da, 0x831153b5
		.int	0x983e5152, 0xee66dfab, 0xa831c66d, 0x2db43210
		.int	0xb00327c8, 0x98fb213f, 0xbf597fc7, 0xbeef0ee4
		.int	0xc6e00bf3, 0x3da88fc2, 0xd5a79147, 0x930aa725
		.int	0x06ca6351, 0xe003826f, 0x14292967, 0x0a0e6e70
		.int	0x27b70a85, 0x46d22ffc, 0x2e1b2138, 0x5c26c926
		.int	0x4d2c6dfc, 0x5ac42aed, 0x53380d13, 0x9d95b3df
		.int	0x650a7354, 0x8baf63de, 0x766a0abb, 0x3c77b2a8
		.int	0x81c2c92e, 0x47edaee6, 0x92722c85, 0x1482353b
		.int	0xa2bfe8a1, 0x4cf10364, 0xa81a664b, 0xbc423001
		.int	0xc24b8b70, 0xd0f89791, 0xc76c51a3, 0x0654be30
		.int	0xd192e819, 0xd6ef5218, 0xd6990624, 0x5565a910
		.int	0xf40e3585, 0x5771202a, 0x106aa070, 0x32bbd1b8
		.int	0x19a4c116, 0xb8d2d0c8, 0x1e376c08, 0x5141ab53
		.int	0x2748774c, 0xdf8eeb99, 0x34b0bcb5, 0xe19b48a8
		.int	0x391c0cb3, 0xc5c95a63, 0x4ed8aa4a, 0xe3418acb
		.int	0x5b9cca4f, 0x7763e373, 0x682e6ff3, 0xd6b2b8a3
		.int	0x748f82ee, 0x5defb2fc, 0x78a5636f, 0x43172f60
		.int	0x84c87814, 0xa1f0ab72, 0x8cc70208, 0x1a6439ec
		.int	0x90befffa, 0x23631e28, 0xa4506ceb, 0xde82bde9
		.int	0xbef9a3f7, 0xb2c67915, 0xc67178f2, 0xe372532b
		.int	0xca273ece, 0xea26619c, 0xd186b8c7, 0x21c0c207
		.int	0xeada7dd6, 0xcde0eb1e, 0xf57d4f7f, 0xee6ed178
		.int	0x06f067aa, 0x72176fba, 0x0a637dc5, 0xa2c898a6
		.int	0x113f9804, 0xbef90dae, 0x1b710b35, 0x131c471b
		.int	0x28db77f5, 0x23047d84, 0x32caab7b, 0x40c72493
		.int	0x3c9ebe0a, 0x15c9bebc, 0x431d67c4, 0x9c100d4c
		.int	0x4cc5d4be, 0xcb3e42b6, 0x597f299c, 0xfc657e2a
		.int	0x5fcb6fab, 0x3ad6faec, 0x6c44198c, 0x4a475817

@ void acl_sha384_init(vect68 state);
@   initialize sha-384 hash
@ on entry:
@   r0 = pointer to state

st		.req	r0	@
cnt		.req	r1	@
tmp		.req	r2	@
tab		.req	r3	@

acl_sha384_init_table:
		.int	0xcbbb9d5d, 0xc1059ed8
		.int	0x629a292a, 0x367cd507
		.int	0x9159015a, 0x3070dd17
		.int	0x152fecd8, 0xf70e5939
		.int	0x67332667, 0xffc00b31
		.int	0x8eb44a87, 0x68581511
		.int	0xdb0c2e0d, 0x64f98fa7
		.int	0x47b5481d, 0xbefa4fa4

acl_sha384_init:
		adr	tab, acl_sha384_init_table
		b	as512i_entry

@ void acl_sha512_init(vect68 state);
@   initialize sha-512 hash
@ on entry:
@   r0 = pointer to state

acl_sha512_init_table:
		.int	0x6a09e667, 0xf3bcc908
		.int	0xbb67ae85, 0x84caa73b
		.int	0x3c6ef372, 0xfe94f82b
		.int	0xa54ff53a, 0x5f1d36f1
		.int	0x510e527f, 0xade682d1
		.int	0x9b05688c, 0x2b3e6c1f
		.int	0x1f83d9ab, 0xfb41bd6b
		.int	0x5be0cd19, 0x137e2179

acl_sha512_init:
		adr	tab, acl_sha512_init_table
as512i_entry:	mov	cnt, #16
as512i_lp1:	ldr	tmp, [tab], #4
		str	tmp, [st], #4
		subs	cnt, #1
		bne	as512i_lp1
		add	st, #4*48
		mov	cnt, #4
		mov	tmp, #0
as512i_lp2:	str	tmp, [st], #4
		subs	cnt, #1
		bne	as512i_lp2
		bx	lr

		.unreq	st
		.unreq	cnt
		.unreq	tmp
		.unreq	tab

@ void acl_sha512(vect68 state, byte data);
@   update sha-512 hash with byte "data"
@ on entry:
@   r0 = pointer to state
@   r1 = next byte to update the hash

st		.req	r0	@
chr		.req	r1	@
buf		.req	r2	@

len		.req	r3	@
tmp0		.req	r12	@

kay		.req	r1	@
rnd		.req	r3	@
acch		.req	r4	@
accl		.req	r5	@
wh		.req	r6	@
wl		.req	r7	@
tmp1		.req	r8	@
tmp2		.req	r9	@
tmp3		.req	r10	@
tmp4		.req	r11	@
tmp5		.req	r12	@
tmp6		.req	r14	@

as512c_big:	ldr	len, [st, #4*66]
		adds	len, #1
		str	len, [st, #4*66]
		ldr	len, [st, #4*65]
		adcs	len, #0
		str	len, [st, #4*65]
		ldr	len, [st, #4*64]
		adc	len, #0
		str	len, [st, #4*64]
		b	as512_core

acl_sha512:	add	buf, st, #4*32
		ldr	len, [st, #4*67]
		eor	tmp0, len, #3 << 3
		lsl	tmp0, #22
		strb	chr, [buf, tmp0, lsr #25]
		adds	len, #8
		str	len, [st, #4*67]
		bcs	as512c_big
		tst	len, #127 << 3
		bxne	lr

as512_core:	push	{r4-r11, r14}
		add	buf, st, #4*16
		ldmia	st!, {r4-r11}
		stmia	buf!, {r4-r11}
		ldmia	st!, {r4-r11}
		stmia	buf!, {r4-r11}
		adr	kay, acl_sha512_table
		mov	rnd, #0

		@ get w[rnd]
as512c_main_lp:	mov	tmp1, rnd, lsl #28
		ldr	acch, [buf, tmp1, lsr #25]
		add	tmp1, #1 << 27
		ldr	accl, [buf, tmp1, lsr #25]
		cmp	rnd, #16
		blo	as512c_skip

		@ add sigma0(w[rnd+1])
		add	tmp1, #1 << 27
		ldr	wh, [buf, tmp1, lsr #25]
		add	tmp1, #1 << 27
		ldr	wl, [buf, tmp1, lsr #25]
		mov	tmp2, wl, lsr #1
		eor	tmp2, wh, lsl #31
		eor	tmp2, wl, lsr #8
		eor	tmp2, wh, lsl #24
		eor	tmp2, wl, lsr #7
		eor	tmp2, wh, lsl #25
		adds	accl, tmp2
		mov	tmp2, wh, lsr #1
		eor	tmp2, wl, lsl #31
		eor	tmp2, wh, lsr #8
		eor	tmp2, wl, lsl #24
		eor	tmp2, wh, lsr #7
		adc	acch, tmp2

		@ add w[rnd+9]
		add	tmp1, #(8 << 28) - (1 << 27)
		ldr	wh, [buf, tmp1, lsr #25]
		add	tmp1, #1 << 27
		ldr	wl, [buf, tmp1, lsr #25]
		adds	accl, wl
		adc	acch, wh

		@ add sigma1(w[rnd+14])
		add	tmp1, #(5 << 28) - (1 << 27)
		ldr	wh, [buf, tmp1, lsr #25]
		add	tmp1, #1 << 27
		ldr	wl, [buf, tmp1, lsr #25]
		mov	tmp2, wl, lsr #19
		eor	tmp2, wh, lsl #13
		eor	tmp2, wl, lsl #3
		eor	tmp2, wh, lsr #29
		eor	tmp2, wl, lsr #6
		eor	tmp2, wh, lsl #26
		adds	accl, tmp2
		mov	tmp2, wh, lsr #19
		eor	tmp2, wl, lsl #13
		eor	tmp2, wh, lsl #3
		eor	tmp2, wl, lsr #29
		eor	tmp2, wh, lsr #6
		adc	acch, tmp2

		@ store new w[rnd]
		mov	tmp1, rnd, lsl #28
		str	acch, [buf, tmp1, lsr #25]
		add	tmp1, #1 << 27
		str	accl, [buf, tmp1, lsr #25]

		@ add k[rnd]
		as512c_skip:	ldmia kay!, {wh, wl}
		adds	accl, wl
		adc	acch, wh

		@ add sigma1(e)
		add	st, #4*8
		ldmia	st!, {wh, wl}
		mov	tmp2, wl, lsr #14
		eor	tmp2, wh, lsl #18
		eor	tmp2, wl, lsr #18
		eor	tmp2, wh, lsl #14
		eor	tmp2, wh, lsr #9
		eor	tmp2, wl, lsl #23
		adds	accl, tmp2
		mov	tmp2, wh, lsr #14
		eor	tmp2, wl, lsl #18
		eor	tmp2, wh, lsr #18
		eor	tmp2, wl, lsl #14
		eor	tmp2, wl, lsr #9
		eor	tmp2, wh, lsl #23
		adc	acch, tmp2

		@ add ch(e,f,g)
		ldmia	st!, {tmp1, tmp2}
		and	tmp1, wh
		and	tmp2, wl
		ldmia	st!, {tmp3, tmp4}
		bic	tmp3, wh
		bic	tmp4, wl
		eor	tmp1, tmp3
		eor	tmp2, tmp4
		adds	accl, tmp2
		adc	acch, tmp1

		@ add h
		ldmia	st!, {tmp1, tmp2}
		adds	tmp2, accl
		adc	tmp1, acch

		@ get maj(a,b,c)
		sub	st, #4*16
		ldmia	st, {wh, wl, tmp3, tmp4, tmp5, tmp6}
		eor	acch, wh, tmp3
		eor	accl, wl, tmp4
		and	acch, tmp5
		and	accl, tmp6
		and	tmp3, wh
		and	tmp4, wl
		eor	acch, tmp3
		eor	accl, tmp4

		@ add sigma0(a)
		mov	tmp4, wl, lsr #28
		eor	tmp4, wh, lsl #4
		eor	tmp4, wh, lsr #2
		eor	tmp4, wl, lsl #30
		eor	tmp4, wh, lsr #7
		eor	tmp4, wl, lsl #25
		adds	tmp4, accl
		mov	tmp3, wh, lsr #28
		eor	tmp3, wl, lsl #4
		eor	tmp3, wl, lsr #2
		eor	tmp3, wh, lsl #30
		eor	tmp3, wl, lsr #7
		eor	tmp3, wh, lsl #25
		adc	tmp3, acch

		@ shift
		adds	tmp4, tmp2
		adc	tmp3, tmp1
		stmia	st!, {tmp3, tmp4}  @ a = t1+ t2
		ldmia	st, {acch, accl, tmp3, tmp4, tmp5, tmp6}  @ b, c, d
		stmia	st!, {wh, wl}                             @ a
		stmia	st!, {acch, accl, tmp3, tmp4}             @ b, c
		adds	tmp2, tmp6
		adc	tmp1, tmp5
		ldmia	st, {acch, accl, tmp3, tmp4, tmp5, tmp6}  @ e, f, g
		stmia	st!, {tmp1, tmp2}                         @ e = d + t1
		stmia	st!, {acch, accl, tmp3, tmp4, tmp5, tmp6} @ f, g, h
		sub	st, #4*16
		add	rnd, #1
		cmp	rnd, #80
		bne	as512c_main_lp
		mov	rnd, #4
		as512c_lp2:	ldmdb st, {acch, accl, wh, wl}
		ldmdb	buf!, {tmp1, tmp2, tmp3, tmp4}
		adds	tmp4, wl
		adc	tmp3, wh
		adds	tmp2, accl
		adc	tmp1, acch
		stmdb	st!, {tmp1, tmp2, tmp3, tmp4}
		subs	rnd, #1
		bne	as512c_lp2
		pop	{r4-r11, r14}
		bx	lr

@ void acl_sha512_done(vect68 state);
@ finish sha-512 hash - the result is in state[0..15]
@ on entry:
@ r0 = pointer to state

len1		.req	r1	@
len2		.req	r3	@

acl_sha512_done:
		push	{lr}
		ldr	len1, [st, #4*67]
		ldr	len2, [st, #4*66]
		push	{len1, len2}
		ldr	len1, [st, #4*65]
		ldr	len2, [st, #4*64]
		push	{len1, len2}
		mov	chr, #0x80
		b	as512d_entry

as512d_lp1:	mov chr, #0
as512d_entry:	bl acl_sha512
		ldr	len1, [st, #4*67]
		lsr	len1, #3
		tst	len1, #3
		bne	as512d_lp1
		and	len1, #127
		cmp	len1, #112
		bhi	as512d_lp1

		add	buf, st, #4*32
		beq	as512d_done
		mov	len2, #0
as512d_lp2:	str len2, [buf, len1]
		add	len1, #4
		cmp	len1, #112
		bne	as512d_lp2

as512d_done:	pop {len1, len2}
		str	len1, [buf, #4*29]
		str	len2, [buf, #4*28]
		pop	{len1, len2}
		str	len1, [buf, #4*31]
		str	len2, [buf, #4*30]
		bl	as512_core
		pop	{lr}
		bx	lr

		.end
