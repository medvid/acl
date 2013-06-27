// point doubling with fast reduction (Jacobian <= 2 * Jacobian)
// taken directly from
//      D. Hankerson, A. Menezes, S.A. Vanstone:
//      Guide to Elliptic Curve Cryptography, Springer-Verlag, 2004
// algortihm 3.21, p. 91

// a - pointer to ecc point in projective coordinates (x, y, z)
// tmp - pointer to storage space for 4*len ints
// c - pointer to elliptic curve

#include "acl.h"
#include "acl_int.h"

void acl_p_ecc_dbl(vect3 a, vect4 tmp, ecc_t *c)
{
    vect m, t1, t2, yy, zz, fr; uint len;
    // tmp = tmp tmp t1 t2

    m = c->m; len = c->l; fr = c->fr;
    yy = xx + len; zz = yy + len; t1 = tmp + 2*len; t2 = t1 + len;
    
    if(!acl_zero(zz, len)) {    // 2 * inf == inf
        if(!c->a) {
            acl_p_sqr_fr(t1, xx);               // t1 = xx^2
            acl_p_mod_add(t2, t1, t1, m, len);  // t2 = 2 * t1
        } else if((int) c->a == -3) {
            acl_p_sqr_fr(t1, zz);               // 2 t1 = zz^2
            acl_p_mod_sub(t2, xx, t1, m, len);  // 3 t2 = xx - t1
            acl_p_mod_add(t1, t1, xx, m, len);  // 4 t1 = t1 + xx
            acl_p_mul_fr(t1, t1, t2);           // 5 t1 = t2 * t1
            acl_p_mod_add(t2, t1, t1, m, len);  // t2 = 2 * t1
        } else {
            acl_p_sqr_fr(t2, zz);               // t2 = zz^2
            acl_p_sqr_fr(t2, t2);               // t2 = zz^4
            acl_p_mul_fr(t2, t2, c->a);         // t2 = t2 * a
            acl_p_sqr_fr(t1, xx);               // t1 = xx^2
            acl_p_mod_add(t2, t2, t1, m, len);  // t2 = t2 + xx^2
            acl_p_mod_dbl(t1, 1, m, len);       // t1 = 2 * t1
        }
        acl_p_mod_add(t2, t2, t1, m, len);  // 6 t2= t2 + t1 == D
        acl_p_mod_dbl(yy, 1, m, len);       // 7 yy= 2 * yy
        acl_p_mul_fr(zz, zz, yy);           // 8 zz= zz * yy == new zz
        acl_p_sqr_fr(yy, yy);               // 9 yy= yy^2 == 4 A
        acl_p_mul_fr(t1, xx, yy);           // 10 t1 = yy * xx == B
        acl_p_sqr_fr(yy, yy);               // 11 yy = yy^2 == 16 A^2
        acl_p_mod_hlv(yy, 1, m, len);       // 12 yy = yy/2 == C
        acl_p_sqr_fr(xx, t2);               // 13 xx = t2^2 == D^2
        acl_p_mod_sub(xx, xx, t1, m, len);
        acl_p_mod_sub(xx, xx, t1, m, len);  // 15 xx = xx - 2 * t1 == new xx
        acl_p_mod_sub(t1, t1, xx, m, len);  // 16 t1 = t1 - xx
        acl_p_mul_fr(t1, t1, t2);           // 17 t1 = t1 * t2
        acl_p_mod_sub(yy, t1, yy, m, len);  // 18 yy = t1 - yy == new yy
    }
}
