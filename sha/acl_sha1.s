		.global acl_sha1_init
		.global acl_sha1
		.global acl_sha1_done
		.text
		.arm

@ based on Federal Information Processing StandardsPublication 180-2
@       (+ Change Notice to include SHA-224)
@ and on http://en.wikipedia.org/wiki/SHA_hash_functions
@ which translates very well into ARM code

@ throughout this file:
@   state[0..4] == hash state
@   state[5..20] == tmp storage
@   state[21..22] == length

@ void acl_sha1_init(vect23 state);
@   initialize sha-1 hash
@ on entry:
@   r0 = pointer to state

st		.req	r0	@
cnt		.req	r1	@
tmp		.req	r2	@
tab		.req	r3	@

acl_sha1_init_table:
		.int	0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476
		.int	0xc3d2e1f0

acl_sha1_init:	adr	tab, acl_sha1_init_table
		mov	cnt, #5
as1i_lp:	ldr	tmp, [tab], #4
		str	tmp, [st], #4
		subs	cnt, #1
		bne	as1i_lp
		add	st, #4*16
		mov	tmp, #0
		str	tmp, [st], #4
		str	tmp, [st]
		bx	lr

		.unreq	st
		.unreq	cnt
		.unreq	tmp
		.unreq	tab

@ void acl_sha1(vect23 state, byte data);
@   update sha-1 hash with byte "data"
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
rnd		.req	r6	@
acc		.req	r7	@
kay		.req	r8	@
tmp1		.req	r9	@
tmp2		.req	r10	@
tmp3		.req	r12	@

as1c_big:	ldr	len, [st, #4*21]
		add	len, #1
		str	len, [st, #4*21]
		b	as1_core

acl_sha1:	add	buf, st, #4*5
		ldr	len, [st, #4*22]
		eor	tmp0, len, #3 << 3
		lsl	tmp0, #23
		strb	chr, [buf, tmp0, lsr #26]
		adds	len, #8
		str	len, [st, #4*22]
		bcs	as1c_big
		tst	len, #63 << 3
		bxne	lr

as1_core:	push	{r4-r10}
		ldmia	st!, {aa, bb, cc, dd, ee}
		mov	rnd, #0

		@ rnd = 0-15
		ldr	kay, =0x5a827999
as1c_lp1:	ldr	acc, [st, rnd, lsl #2]
		and	tmp2, bb, cc
		bic	tmp1, dd, bb
		orr	tmp1, tmp2
		add	acc, tmp1
		add	acc, kay
		add	acc, ee
		add	acc, aa, ror #27
		mov	ee, dd
		mov	dd, cc
		mov	cc, bb, ror #2
		mov	bb, aa
		mov	aa, acc
		add	rnd, #1
		cmp	rnd, #16
		bne	as1c_lp1
		b	as1c_entry

		@ rnd = 16-79
as1c_main_lp:	add	acc, ee
		add	acc, aa, ror #27
		mov	ee, dd
		mov	dd, cc
		mov	cc, bb, ror #2
		mov	bb, aa
		mov	aa, acc

		@ get w(rnd)
as1c_entry:	mov	tmp3, rnd, lsl #28
		ldr	acc, [st, tmp3, lsr #26]
		add	tmp1, tmp3, #2 << 28
		ldr	tmp2, [st, tmp1, lsr #26]
		eor	acc, tmp2
		add	tmp1, #6 << 28
		ldr	tmp2, [st, tmp1, lsr #26]
		eor	acc, tmp2
		add	tmp1, #5 << 28
		ldr	tmp2, [st, tmp1, lsr #26]
		eor	acc, tmp2
		ror	acc, #31
		str	acc, [st, tmp3, lsr #26]

		@ get f(rnd)
		cmp	rnd, #60
		bhs	as1c_60
		cmp	rnd, #40
		bhs	as1c_40
		cmp	rnd, #20
		bhs	as1c_20

		@ rnd = 16-19
		and	tmp2, bb, cc
		bic	tmp1, dd, bb
		orr	tmp1, tmp2
		add	acc, tmp1
		add	acc, kay

		add	rnd, #1
		cmp	rnd, #20
		bne	as1c_main_lp
		ldr	kay, =0x6ed9eba1
		b	as1c_main_lp

		@ rnd = 20-39
as1c_20:	eor	tmp1, cc, dd
		eor	tmp1, bb
		add	acc, tmp1
		add	acc, kay

		add	rnd, #1
		cmp	rnd, #40
		bne	as1c_main_lp
		ldr	kay, =0x8f1bbcdc
		b	as1c_main_lp

		@ rnd = 40-59
as1c_40:	and	tmp2, bb, cc
		orr	tmp1, bb, cc
		and	tmp1, dd
		orr	tmp1, tmp2
		add	acc, tmp1
		add	acc, kay

		add	rnd, #1
		cmp	rnd, #60
		bne	as1c_main_lp
		ldr	kay, =0xca62c1d6
		b	as1c_main_lp

		@ rnd = 60-79
as1c_60:	eor	tmp1, cc, dd
		eor	tmp1, bb
		add	acc, tmp1
		add	acc, kay

		add	rnd, #1
		cmp	rnd, #80
		bne	as1c_main_lp

		add	acc, ee
		add	acc, aa, ror #27
		mov	ee, dd
		mov	dd, cc
		mov	cc, bb, ror #2
		mov	bb, aa
		mov	aa, acc

		ldmdb	st!, {rnd, acc, tmp1, tmp2}
		add	bb, rnd
		add	cc, acc
		add	dd, tmp1
		add	ee, tmp2
		ldr	tmp1, [st, #-4]!
		add	aa, tmp1
		stmia	st, {aa, bb, cc, dd, ee}

		pop	{r4-r10}
		bx	lr

@ void acl_sha1_done(vect23 state);
@   finish sha-1 hash - the result is in state[0..4]
@ on entry:
@   r0 = pointer to state

len1		.req	r1	@
len2		.req	r3	@

acl_sha1_done:	push	{lr}
		ldr	len1, [st, #4*22]
		ldr	len2, [st, #4*21]
		push	{len1, len2}
		mov	chr, #0x80
		b	as1d_entry

as1d_lp1:	mov	chr, #0
as1d_entry:	bl	acl_sha1
		ldr	len1, [st, #4*22]
		lsr	len1, #3
		tst	len1, #3
		bne	as1d_lp1
		and	len1, #63
		cmp	len1, #56
		bhi	as1d_lp1

		add	buf, st, #4*5
		beq	as1d_done
		mov	len2, #0
as1d_lp2:	str	len2, [buf, len1]
		add	len1, #4
		cmp	len1, #56
		bne	as1d_lp2

as1d_done:
		pop	{len1, len2}
		str	len1, [buf, #4*15]
		str	len2, [buf, #4*14]
		bl	as1_core
		pop	{lr}
		bx	lr

		.end
