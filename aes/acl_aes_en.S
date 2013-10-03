@ no c prototype exists for this function, as it isonly called from assembler
@   core encryption routine for aes
@   based on Federal Information Processing Standards Publication 197
@ on entry:
@   r2 = key = pointer to already expanded key
@   r3 = rnd = key size - 4: 128, 6: 192, 8: 256
@   r4-r7 = st0-st3 = 16 input data bytes
@ returns:
@   r4-r7 = st0-st3 = 16 output data bytes
@ corrupts:
@   r0-r12

		.global acl_aes_en
		.text
		.arm

ff		.req	r0	@ holds mask value
lut		.req	r1	@ aes look up table
key		.req	r2	@ expanded key
rnd		.req	r3	@ round counter
st0		.req	r4	@ aes state word 0
st1		.req	r5	@ aes state word 1
st2		.req	r6	@ aes state word 2
st3		.req	r7	@ aes state word 3
tmp		.req	r8	@ temp
key0		.req	r8	@ key tmp 0
acc		.req	r9	@ xor accumulator
key1		.req	r9	@ key tmp 1
nst0		.req	r10	@ next state word 0
nst1		.req	r11	@ next state word 1
nst2		.req	r12	@ next state word 2

		@ rnd = number of rounds - 1
acl_aes_en:	add	rnd, #5
		mov	ff, #0xff
		ldr	lut, =acl_aes_fwd_table

		@ add round key
		ldm	key!, {key0, key1}
		eor	st0, key0
		eor	st1, key1
		ldm	key!, {key0, key1}
		eor	st2, key0
		eor	st3, key1

		@ 1. column
aae_lp:		and	tmp, ff, st0                            @ st 0 0
		ldr	acc, [lut, tmp, lsl #2]
		and	tmp, ff, st1, lsr #8                    @ st 1 1
		ldr	tmp, [lut, tmp, lsl #2]
		eor	acc, tmp, ror #24
		and	tmp, ff, st2, lsr #16                   @ st 2 2
		ldr	tmp, [lut, tmp, lsl #2]
		eor	acc, tmp, ror #16
		mov	tmp, st3, lsr #24                       @ st 3 3
		ldr	tmp, [lut, tmp, lsl #2]
		eor	nst0, acc, tmp, ror #8                  @ store new st 0

		@ 2. column
		and	tmp, ff, st1                            @ st 1 0
		ldr	acc, [lut, tmp, lsl #2]
		and	tmp, ff, st2, lsr #8                    @ st 2 1
		ldr	tmp, [lut, tmp, lsl #2]
		eor	acc, tmp, ror #24
		and	tmp, ff, st3, lsr #16                   @ st 3 2
		ldr	tmp, [lut, tmp, lsl #2]
		eor	acc, tmp, ror #16
		mov	tmp, st0, lsr #24                       @ st 0 3
		ldr	tmp, [lut, tmp, lsl #2]
		eor	nst1, acc, tmp, ror #8                  @ store new st 1

		@ 3. column
		and	tmp, ff, st2                            @ st 2 0
		ldr	acc, [lut, tmp, lsl #2]
		and	tmp, ff, st3, lsr #8                    @ st 3 1
		ldr	tmp, [lut, tmp, lsl #2]
		eor	acc, tmp, ror #24
		and	tmp, ff, st0, lsr #16                   @ st 0 2
		ldr	tmp, [lut, tmp, lsl #2]
		eor	acc, tmp, ror #16
		mov	tmp, st1, lsr #24                       @ st 1 3
		ldr	tmp, [lut, tmp, lsl #2]
		eor	nst2, acc, tmp, ror #8                  @ store new st 2

		@ 4. column
		and	tmp, ff, st3                            @ st 3 0
		ldr	acc, [lut, tmp, lsl #2]
		and	tmp, ff, st0, lsr #8                    @ st 0 1
		ldr	tmp, [lut, tmp, lsl #2]
		eor	acc, tmp, ror #24
		and	tmp, ff, st1, lsr #16                   @ st 1 2
		ldr	tmp, [lut, tmp, lsl #2]
		eor	acc, tmp, ror #16
		mov	tmp, st2, lsr #24                       @ st 2 3
		ldr	tmp, [lut, tmp, lsl #2]
		eor	st3, acc, tmp, ror #8                   @ store new st 3

		@ add round key
		ldm	key!, {key0, key1}
		eor	st0, nst0, key0
		eor	st1, nst1, key1
		ldm	key!, {key0, key1}
		eor	st2, nst2, key0
		eor	st3, key1

		@ decrement counter
		subs	rnd, #1                                 @ do all the rounds,
		bne	aae_lp                                  @ except the last one

		@ last round
		ldr	lut, =acl_aes_fwd_sbox

		@ 1. column
		and	tmp, ff, st0                            @ st 0 0
		ldrb	acc, [lut, tmp]
		and	tmp, ff, st1, lsr #8                    @ st 1 1
		ldrb	tmp, [lut, tmp]
		orr	acc, tmp, lsl #8
		and	tmp, ff, st2, lsr #16                   @ st 2 2
		ldrb	tmp, [lut, tmp]
		orr	acc, tmp, lsl #16
		mov	tmp, st3, lsr #24                       @ st 3 3
		ldrb	tmp, [lut, tmp]
		orr	nst0, acc, tmp, lsl #24                 @ store new st 0

		@ 2. column
		and	tmp, ff, st1                            @ st 1 0
		ldrb	acc, [lut, tmp]
		and	tmp, ff, st2, lsr #8                    @ st 2 1
		ldrb	tmp, [lut, tmp]
		orr	acc, tmp, lsl #8
		and	tmp, ff, st3, lsr #16                   @ st 3 2
		ldrb	tmp, [lut, tmp]
		orr	acc, tmp, lsl #16
		mov	tmp, st0, lsr #24                       @ st 0 3
		ldrb	tmp, [lut, tmp]
		orr	nst1, acc, tmp, lsl #24                 @ store new st 1

		@ 3. column
		and	tmp, ff, st2                            @ st 2 0
		ldrb	acc, [lut, tmp]
		and	tmp, ff, st3, lsr #8                    @ st 3 1
		ldrb	tmp, [lut, tmp]
		orr	acc, tmp, lsl #8
		and	tmp, ff, st0, lsr #16                   @ st 0 2
		ldrb	tmp, [lut, tmp]
		orr	acc, tmp, lsl #16
		mov	tmp, st1, lsr #24                       @ st 1 3
		ldrb	tmp, [lut, tmp]
		orr	nst2, acc, tmp, lsl #24                 @ store new st 2

		@ 4. column
		and	tmp, ff, st3                            @ st 3 0
		ldrb	acc, [lut, tmp]
		and	tmp, ff, st0, lsr #8                    @ st 0 1
		ldrb	tmp, [lut, tmp]
		orr	acc, tmp, lsl #8
		and	tmp, ff, st1, lsr #16                   @ st 1 2
		ldrb	tmp, [lut, tmp]
		orr	acc, tmp, lsl #16
		mov	tmp, st2, lsr #24                       @ st 2 3
		ldrb	tmp, [lut, tmp]
		orr	st3, acc, tmp, lsl #24                  @ store new st 3

		@ add round key
		ldm	key!, {key0, key1}
		eor	st0, nst0, key0
		eor	st1, nst1, key1
		ldm	key!, {key0, key1}
		eor	st2, nst2, key0
		eor	st3, st3, key1
		bx	lr

		.end
