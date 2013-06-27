@ tables used by the aes routines

		.global acl_aes_rcon
		.global acl_aes_fwd_sbox
		.global acl_aes_inv_sbox
		.global acl_aes_fwd_table
		.global acl_aes_inv_table
		.text
acl_aes_rcon:		.include "./aes/acl_aes_rcon.txt"	@ 14
acl_aes_fwd_sbox:	.include "./aes/acl_aes_fwd_sbox.txt"	@ 256
acl_aes_inv_sbox:	.include "./aes/acl_aes_inv_sbox.txt"	@ 256
	.align 2
acl_aes_fwd_table:	.include "./aes/acl_aes_fwd_table.txt"	@ 1k
acl_aes_inv_table:	.include "./aes/acl_aes_inv_table.txt"	@ 1k
	.end
