// point doubling with fast reduction (Lopez-Dahab <= 2 * Lopez-Dahab)
// taken directly from
//      D. Hankerson, A. Menezes, S.A. Vanstone:
//      Guide to Elliptic Curve Cryptography, Springer-Verlag, 2004
// algortihm 3.24, p. 94

// a - pointer to ecc point in projective coordinates (x, y, z)
// tmp - pointer to storage space for 4*len ints
// c - pointer to elliptic curve

#include "acl.h"
#include "acl_int.h"

void acl_2_ecc_dbl(vect3 a, vect4 tmp, ecc_t *c)
{
    vect m, t1, t2, yy, zz, fr; uint len;
    // tmp = tmp tmp t1 t2

    m = c->m; len = c->l; fr = c->fr;
    yy = xx + len; zz = yy + len; t1 = tmp + 2*len;t2 = t1 + len;

    if(!acl_zero(zz, len)) {    // 2 * inf == inf
        acl_2_sqr_fr(t1, zz);           // 2 t1 = zz^2
        acl_2_sqr_fr(t2, xx);           // 3 t2 = xx^2
        acl_2_mul_fr(zz, t1, t2);       // 4 zz = t1 * t2 == new zz
        acl_2_sqr_fr(xx, t2);           // 5 xx = xx^4
        acl_2_sqr_fr(t1, t1);           // 6 t1 = zz^4
        if((int) c->b == 1)             // 7 t2 = b* zz^4
            acl_mov(t2, t1, len);
        else {
            acl_2_mul_fr(t2, t1, c->b);
        }
        acl_xor(xx, xx, t2, len);       // 8 xx = xx + t2 == new xx
        acl_2_sqr_fr(yy, yy);           // 9 yy = yy^2
        if((int) c->a == 1)             // 10 yy = yy + a * zz
            acl_xor(yy, yy, zz, len);
        else if(c->a) {
            acl_2_mul_fr(t1, c->a, zz);
            acl_xor(yy, yy, t1, len);
        }
        acl_xor(yy, yy, t2, len);       // 11 yy = yy + t2
        acl_2_mul_fr(yy, yy, xx);       // 12 yy = yy * xx
        acl_2_mul_fr(t1, zz, t2);       // 13 t1 = zz * t2
        acl_xor(yy, yy, t1, len);       // 14 yy = yy + t1 == new yy
    }
}
