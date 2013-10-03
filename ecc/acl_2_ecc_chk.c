// returns TRUE if affine point is on curve, FALSE otherwise

// a - pointer to ecc point in affine coordinates (x, y)
// tmp - pointer to storage space for 4*len ints
// c - pointer to elliptic curve

#include "acl.h"
#include "acl_int.h"
#include "acl_config.h"

bool_t acl_2_ecc_chk(vect2 a, vect4 tmp, ecc_t *c)
{
    vect t1, t2, yy, fr; uint len;
    // tmp = tmp tmp t1 t2

    len = c->l; fr = c->fr;
    yy = xx + len; t1 = tmp + 2*len; t2 = t1 + len;

#if ACL_CHK_INF_ON_CURVE
    if (acl_zero(a, 2*len)) return TRUE;
#endif
    acl_2_sqr_fr(t1, xx);           // t1 = x^2
    if (!c->a) {                    // t1 = x^2 * (x + a)
        acl_2_mul_fr(t1, t1, xx);
    } else if ((int) c->a == 1) {
        acl_xor32(t2, xx, 1, len);
        acl_2_mul_fr(t1, t1, t2);
    } else {
        acl_xor(t2, xx, c->a, len);
        acl_2_mul_fr(t1, t1, t2);
    }
    if ((int) c->b == 1) {          // t1 = x^3 + ax + b
        acl_xor32(t1, t1, 1, len);
    } else {
        acl_xor(t1, t1, c->b, len);
    }
    acl_xor(t2, xx, yy, len);       // t2 = y + x
    acl_2_mul_fr(t2, t2, yy);       // t2 = y^2 + xy
    return !acl_cmp(t1, t2, len);   // t1 == t2 ?
}
