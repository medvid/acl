@ uint acl_p_mont_m_inv(vect m);
@   precomputation for mongomery multiplication
@   returns -m^-1 mod 2^32 (m must be odd)
@ on entry:
@   r0 = pointer to m
@ returns:
@   r0 = result

		.global acl_p_mont_m_inv
		.text
		.arm

src		.req	r0	@
q		.req	r0	@
m		.req	r1	@
acc		.req	r2	@
mask		.req	r3	@

acl_p_mont_m_inv:
		ldr m, [src]
		mov q, #0
		mov acc, #0
		mov mask, #1
apmmi_lp:	tst acc, #1
		addeq acc, m
		orreq q, mask
		mov acc, acc, lsr #1
		adds mask, mask
		bcc apmmi_lp
		bx lr

		.end
