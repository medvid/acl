# makefile for ARM cryptographic library
# has dependency tracking, but uses perl

.LIBPATTERNS =

AR = arm-none-eabi-ar
AS = arm-none-eabi-as
CC = arm-none-eabi-gcc
RANLIB = arm-none-eabi-ranlib

CCFLAGS = -c -mcpu=arm7tdmi -mthumb -MD -Wall -Os -mapcs-frame \
	-mthumb-interwork -Wa,-alhms=Lst/$*.lst -o Obj/$*.o
ASFLAGS = -mcpu=arm7tdmi -mthumb-interwork --MD Obj/$*.d -alhms=Lst/$*.lst \
	-o Obj/$*.o

VPATH = Obj:AES:Common:Curves:ECC:GF_2:GF_p:Primes:PRNG:RSA:SHA

AES = acl_aes_cbc_de.s acl_aes_cbc_en.s acl_aes_cntr.s acl_aes_de.s \
	acl_aes_ecb_de.s acl_aes_ecb_en.s acl_aes_en.s acl_aes_key_de.s \
	acl_aes_key_en.s acl_aes_tables.s
SHA = acl_sha1.s acl_sha256.s acl_sha512.s
COMMON = acl_bit.s acl_bit_clr.s acl_bit_set.s acl_cmp.s acl_ctz.s \
	acl_hex2str_dec.s acl_hex2str_le.s acl_log2.s acl_mov.s acl_mov32.s \
	acl_rev.s acl_rsh.s acl_str2bytes.s acl_str2hex_be.s acl_str2hex_le.s \
	acl_xor.s acl_xor32.s acl_zero.s
GF_P = acl_p_coprime.s acl_p_div.c acl_p_fr.s acl_p_mod.c acl_p_mod_add.s \
	acl_p_mod_dbl.s acl_p_mod_hlv.s acl_p_mod_inv.cacl_p_mod_sub.s \
	acl_p_mont_exp.c acl_p_mont_inv.s acl_p_mont_m_inv.s acl_p_mont_pre.c \
	acl_p_mont_red.s acl_p_mul.s acl_p_sqr.s acl_p_sqrt.c
PRIMES = acl_p_rm_test.c acl_p_rm_test2.c acl_p_rnd_prime.c acl_p_tables.s
PRNG = acl_prng_lc_.s acl_prng_lc.c acl_prng_aes.c acl_prng_sha.c acl_prng_bbs.c
RSA = acl_rsa_pre.c acl_rsa_crt.c
GF_2 = acl_2_fr.s acl_2_mod_hlv.s acl_2_mod_inv.c acl_2_mont_inv.s \
	acl_2_mul.s acl_2_sqr.s
CURVES =				acl_secp112r1.c		acl_secp112r2.c	\
						acl_secp128r1.c		acl_secp128r2.c	\
	acl_secp160k1.c		acl_secp160r1.c		acl_secp160r2.c	\
	acl_secp192k1.c		acl_secp192r1.c						\
	acl_secp224k1.c		acl_secp224r1.c						\
	acl_secp256k1.c		acl_secp256r1.c						\
						acl_secp384r1.c						\
						acl_secp521r1.c						\
	\
						acl_sect113r1.c		acl_sect113r2.c	\
						acl_sect131r1.c		acl_sect131r2.c	\
	acl_sect163k1.c		acl_sect163r1.c		acl_sect163r2.c	\
						acl_sect193r1.c		acl_sect193r2.c	\
	acl_sect233k1.c		acl_sect233r1.c						\
	acl_sect239k1.c											\
	acl_sect283k1.c		acl_sect283r1.c						\
	acl_sect409k1.c		acl_sect409r1.c						\
	acl_sect571k1.c		acl_sect571r1.c
ECC = acl_p_ecc_add.c acl_p_ecc_aff.c acl_p_ecc_chk.c acl_p_ecc_dbl.c \
	acl_p_ecc_func.c acl_p_ecc_p2str.c acl_p_ecc_str2p.c acl_2_ecc_add.c \
	acl_2_ecc_aff.c acl_2_ecc_chk.c acl_2_ecc_dbl.cacl_2_ecc_func.c \
	acl_2_ecc_p2str.c acl_2_ecc_str2p.c acl_ecc_mul.c acl_ecc_pre.c \
	acl_ecc_pro.c
ECDSA = acl_ecdsa_gen.c acl_ecdsa_ver.c
SRC := $(AES) $(SHA) $(COMMON) $(GF_P) $(PRIMES) $(PRNG) $(RSA) $(GF_2) \
	$(CURVES) $(ECC) $(ECDSA)
OBJ := $(subst .c,.o,$(SRC))
OBJ := $(subst .s,.o,$(OBJ))
OBJ_PRE := $(addprefix Obj/,$(OBJ))
DEPS := $(subst .o,.d,$(OBJ))
PERL = perl -p -e "s{[^\.]+\.o[ :]*}{$*\.o $*\.d : }g;s{/}{\\}g;"

.PHONY: all
all: acl.a

acl.a: $(OBJ)
	$(AR) rvu $@ $(OBJ_PRE)
	$(RANLIB) $@

include $(DEPS)

%.d : %.c
	-del Obj/$@
	$(CC) -c -M $< > Obj/$@.orig
	$(PERL) < Obj/$@.orig > Obj/$@
	-del Obj/$@.orig

%.d : %.s
	-del Obj/$@
	$(AS) $(ASFLAGS) $<
	copy Obj/$@ Obj/$@.orig
	$(PERL) < Obj/$@.orig > Obj/$@
	-del Obj/$@.orig

%.o : %.c
	$(CC) $(CCFLAGS) $<
	copy Obj/$*.d Obj/$*.d.orig
	$(PERL) < Obj/$*.d.orig > Obj/$*.d
	-del Obj/$*.d.orig

%.o : %.s
	$(AS) $(ASFLAGS) $<
	copy Obj/$*.d Obj/$*.d.orig
	$(PERL) < Obj/$*.d.orig > Obj/$*.d
	-del Obj/$*.d.orig
