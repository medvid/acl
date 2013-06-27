@ void acl_prng_lc_init(uint seed);
@   initialize linear congruential prng
@ on entry:
@   r0 = seed

@ void acl_prng_lc(vect res, size_t len);
@   write output array with pseudorandom numbers from linear congruential prng
@   prng: x = (279470273 * x) mod (2^32-5)
@   parameters taken from:
@       Tables of Linear Congruential Generators of Different Sizes
@       and Good Lattice Structure
@       Pierre L'Ecuyer
@       Mathematics of Computation, Vol. 68, No. 225 (Jan., 1999), pp. 249-260
@
@ on entry:
@   r0 = pointer to result
@   r1 = length of input/output arrays in 32-bit words

		.global acl_prng_lc_init
		.global acl_prng_lc

		.data
acl_prng_lc_val:	.int	1

		.text
		.arm

seed		.req	r0	@
ptr		.req	r1	@
out		.req	r0	@
len		.req	r1	@
tmp1		.req	r2	@
tmp2		.req	r3	@
sum1		.req	r4	@
sum2		.req	r5	@
top		.req	r12	@

acl_prng_lc_init:
		ldr	ptr, =acl_prng_lc_val
		str	seed, [ptr]
		bx	lr
acl_prng_lc:	push	{r4-r5}
		ldr	tmp1, =acl_prng_lc_val
		ldr	sum1, [tmp1]
aplc_lp1:	ldr	tmp1, =279470273
		mov	tmp2, sum1
		umull	sum1, top, tmp1, tmp2
aplc_lp2:	mov	sum2, top, lsr #30
		adds	sum1, top
		adc	sum2, #0
		adds	sum1, top, lsl #2
		adc	sum2, #0
		movs	top, sum2
		bne	aplc_lp2
		cmp	sum1, #0
		moveq	sum1, #1
		str	sum1, [out], #4
		subs	len, #1
		bne	aplc_lp1
		ldr	tmp1, =acl_prng_lc_val
		str	sum1, [tmp1]
		pop	{r4-r5}
		bx	lr

		.end
