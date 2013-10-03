// ecc point multiplication res = k * p + l * q

// res - result in affine coordinates (but must have space for projective)
// p - pointer to first point or its pre-computation (affine)
// q - pointer to second point or its pre-computation (affine)
// w - number of teeth in comb (1 -> no pre-computation)
// s - distance between teeth of comb (if pre-computation used)
// k - pointer to number multiplying p
// l - pointer to number multiplying q
// len_kl - length of k, l in 32-bit words
// tmp - temporary storage (5*len ints)
// c - pointer to elliptic curve

#include "acl.h"
#include "acl_int.h"

void acl_ecc_mul(vect3 res, vect p, vect q, uint w,uint s, vect k, vect l, \
        size_t len_kl, vect5 tmp, ecc_t *c)
{
    uint len2, i, j, hk, hl;

    len2 = 2 * c->l;

    acl_mov32(res + len2, 0, c->l);
    if (w == 1) {
        for (i = 32 * len_kl; i; i--) {
            acl_ecc_dbl(res, tmp, c);
            if (p && acl_bit(k, i - 1, len_kl)) {
                acl_ecc_add(res, p, tmp, c);
            }
            if (q && acl_bit(l, i - 1, len_kl)) {
                acl_ecc_add(res, q, tmp, c);
            }
        }
    } else {
        for (i = s; i; i--) {
            acl_ecc_dbl(res, tmp, c);
            hk = 0; hl = 0;
            for (j = w * s; j; j -= s) {
                if (p) {
                    hk = (hk << 1) + acl_bit(k, i- 1 + j - s, len_kl);
                }
                if (q) {
                    hl = (hl << 1) + acl_bit(l, i- 1 + j - s, len_kl);
                }
            }
            if (hk) {
                acl_ecc_add(res, p + (hk - 1) * len2, tmp, c);
            }
            if (hl) {
                acl_ecc_add(res, q + (hl - 1) * len2, tmp, c);
            }
        }
    }
    acl_ecc_aff(res, tmp, c);
}
