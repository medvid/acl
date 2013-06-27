@ void acl_mov(vect res, vect src, size_t len);
@   copies the array "src" to the array "res"
@ on entry:
@   r0 = pointer to destination
@   r1 = pointer to source
@   r2 = length of input/output arrays in 32-bit words

		.global acl_mov
		.text
		.arm

dest		.req	r0	@
src		.req	r1	@
len		.req	r2	@
tmp1		.req	r3	@
tmp2		.req	r12	@

acl_mov:	ldmia	src!, {tmp1, tmp2}
		subs	len, #2
		stmhsia	dest!, {tmp1, tmp2}
		bhi	acl_mov
		strlo	tmp1, [dest]
		bx	lr

		.end
