// point addition with fast reduction (Lopez-Dahab<= Lopez-Dahab + Affine)
// taken directly from
//      D. Hankerson, A. Menezes, S.A. Vanstone:
//      Guide to Elliptic Curve Cryptography, Springer-Verlag, 2004
// algortihm 3.25, p. 95

// a - pointer to ecc point in projective coordinates (x, y, z)
// b - pointer to ecc point in affine coordinates (x, y)
// tmp - pointer to storage space for 5*len ints
// c - pointer to elliptic curve

#include "acl.h"
#include "acl_int.h"

void acl_2_ecc_add(vect3 a, vect2 b, vect5 tmp, ecc_t *c)
{
    vect t1, t2, t3, yy1, zz1, yy2, fr; uint len;
    // tmp = tmp tmp t1 t2 t3

    len = c->l; fr = c->fr;
    yy2 = b + len; yy1 = xx1 + len; zz1 = yy1 + len;
    t1 = tmp + 2*len; t2 = t1 + len; t3 = t2 + len;

    if(!acl_zero(b, 2*len)) {       // if b == inf then ret a
        if(acl_zero(zz1, len))      // if a == inf then ret b
            acl_ecc_pro(a, b, len);
        else {
            acl_2_mul_fr(t1, zz1, xx2);     // 3 t1= zz1 * xx2
            acl_2_sqr_fr(t2, zz1);          // 4 t2= zz1^2
            acl_xor(xx1, xx1, t1, len);     // 5 xx1 = xx1 + t1 == B
            acl_2_mul_fr(t1, zz1, xx1);     // 6 t1= zz1 * B == C
            acl_2_mul_fr(t3, t2, yy2);      // 7 t3= t2 * yy2
            acl_xor(yy1, yy1, t3, len);     // 8 yy1 = yy1 + t3 == A
            if(acl_zero(xx1, len))
                if(acl_zero(yy1, len)) {
                    acl_ecc_pro(a, b, len);
                    acl_ecc_dbl(a, tmp, c);
                } else
                    acl_mov32(zz1, 0, len);
            else {
                acl_2_sqr_fr(zz1, t1);      // 10 zz1 = C^2 == new zz
                acl_2_mul_fr(t3, t1, yy1);  // 11 t3 = A * C == E
                if((int) c->a == 1)         // 12 t1 = t1 + a * t2
                        acl_xor(t1, t1, t2, len);
                else if(c->a) {
                        acl_2_mul_fr(t2, c->a, t2);
                        acl_xor(t1, t1, t2, len);
                }
                acl_2_sqr_fr(t2, xx1);       // 13 t2 = B^2
                acl_2_mul_fr(xx1, t2, t1);   // 14 xx1 = t2 * t1 == D
                acl_2_sqr_fr(t2, yy1);       // 15 t2 = A^2
                acl_xor(xx1, xx1, t2, len);  // 16 xx1 = xx1 + t2
                acl_xor(xx1, xx1, t3, len);  // 17 xx1 = xx1 + t3 == new xx
                acl_2_mul_fr(t2, xx2, zz1);  // 18 t2 = xx2 * zz1
                acl_xor(t2, t2, xx1, len);   // 19 t2 = t2 + xx1 == F
                acl_2_sqr_fr(t1, zz1);       // 20 t1 = zz1^2
                acl_xor(t3, t3, zz1, len);   // 21 t3 = t3 + zz1
                acl_2_mul_fr(yy1, t3, t2);   // 22 yy1 = t3 * t2
                acl_xor(t2, xx2, yy2, len);  // 23 t2 = xx2 + yy2
                acl_2_mul_fr(t3, t1, t2);    // 24 t3 = t1 * t2 == G
                acl_xor(yy1, yy1, t3, len);  // 25 yy1 = yy1 + t3 == new yy
            }
        }
    }
}
