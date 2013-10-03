// point addition with fast reduction (Jacobian <=Jacobian + Affine)
// taken directly from
//      D. Hankerson, A. Menezes, S.A. Vanstone:
//      Guide to Elliptic Curve Cryptography, Springer-Verlag, 2004
// algortihm 3.22, pp. 91-92

// a - pointer to ecc point in projective coordinates (x, y, z)
// b - pointer to ecc point in affine coordinates (x, y)
// tmp - pointer to storage space for 5*len ints
// c - pointer to elliptic curve

#include "acl.h"
#include "acl_int.h"

void acl_p_ecc_add(vect3 a, vect2 b, vect5 tmp, ecc_t *c)
{
    vect m, t1, t2, t3, yy1, zz1, yy2, fr; uint len;
    // tmp = tmp tmp t1 t2 t3

    m = c->m; len = c->l; fr = c->fr;
    yy2 = b + len; yy1 = xx1 + len; zz1 = yy1 + len;
    t1 = tmp + 2*len; t2 = t1 + len; t3 = t2 + len;

    if (!acl_zero(b, 2*len)) {      // if b == inf then ret a
        if(acl_zero(zz1, len)) {    // if a == inf then ret b
            acl_ecc_pro(a, b, len);
        } else {
            acl_p_sqr_fr(t1, zz1);              // 3 t1 = zz1^2 == A
            acl_p_mul_fr(t2, t1, zz1);          // 4 t2 = t1 * zz1 == B
            acl_p_mul_fr(t1, t1, xx2);          // 5 t1 = t1 * xx2 == C
            acl_p_mul_fr(t2, t2, yy2);          // 6 t2 = t2 * yy2 == D
            acl_p_mod_sub(t1, t1, xx1, m, len); // 7 t1 = t1 - xx1 == E
            acl_p_mod_sub(t2, t2, yy1, m, len); // 8 t2 = t2 - yy1 == F
            if (acl_zero(t1, len)) {
                if (acl_zero(t2, len)) {
                    acl_ecc_dbl(a, tmp, c);
                } else {
                    acl_mov32(zz1, 0, len);
                }
            } else {
                acl_p_mul_fr(zz1, zz1, t1);     // 10 zz1 = zz1 * t1 -> zz
                acl_p_sqr_fr(t3, t1);           // 11 t3 = t1^2 == G
                acl_p_mul_fr(t1, t3, t1);       // 12 t1 = t3 * t1 == H
                acl_p_mul_fr(t3, t3, xx1);      // 13 t3 = t3 * xx1 == I
                acl_p_sqr_fr(xx1, t2);          // 15 xx1 = t2^2
                acl_p_mod_sub(xx1, xx1, t3, m, len);
                acl_p_mod_sub(xx1, xx1, t3, m, len); // 16 xx1 = xx1 - 2 * t3
                acl_p_mod_sub(xx1, xx1, t1, m, len); // 17 xx1 = xx1 - t1 -> xx
                acl_p_mod_sub(t3, t3, xx1, m, len);  // 18 t3 = t3 - xx1
                acl_p_mul_fr(t3, t3, t2);            // 19 t3 = t3 * t2
                acl_p_mul_fr(t1, t1, yy1);           // 20 t1 = t1 * yy1
                acl_p_mod_sub(yy1, t3, t1, m, len);  // 21 yy1 = t3 - t1 -> yy
            }
        }
    }
}
