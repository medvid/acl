// RSA decryption using CRT
// pt = decrypt(ct)

#include "acl.h"

#define sq pt

void acl_rsa_crt(vect2 pt, vect2 ct, \
                 vect p, vect r2_mod_p, uint p_inv,\
                 vect q, vect r2_mod_q, uint q_inv,
                 vect dmp1, vect dmq1, vect iqmp, vect4 tmp, size_t len)
{
    vect sp;

    sp = tmp + 3*len;
    acl_p_mod(sp, ct, 2*len, p, len);       // sp = ct mod p
    acl_p_mont_exp(sp, sp, dmp1, len, p, tmp, p_inv, r2_mod_p, len);
                                            // sp = sp^dmp1 mod p
    acl_p_mod(sq, ct, 2*len, q, len);       // sq = ct mod q
    acl_p_mont_exp(sq, sq, dmq1, len, q, tmp, q_inv, r2_mod_q, len);
                                            // sq = sq^dmq1 mod q
    acl_p_mod_sub(sp, sp, sq, p, len);      // sp = (sp - sq) mod p
    acl_p_mul(tmp, sp, iqmp, len);          // tmp = sp * iqmp
    acl_p_mod(sp, tmp, 2*len, p, len);      // sp = tmp mod p
    acl_p_mul(tmp, sp, q, len);             // tmp = sp * q
    acl_mov32(pt + len, 0, len);
    acl_p_mod_add(pt, sq, tmp, 0, 2*len);   // pt =sq + tmp
}
