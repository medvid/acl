		.global _start
		.text
		.arm

_start:		LDR sp, =stack_top
		BL main
		B _start

		.end
