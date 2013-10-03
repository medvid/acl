// returns TRUE if affine point is on curve, FALSE otherwise

// the routine also calculates:
// t1 = tmp + 2*len = right side of equation (x^3 +ax + b)
// t2 = tmp + 3*len = left side of equation (y^2)
// this "feature" is used by the point decompression routine acl_p_ecc_str2p

// a - pointer to ecc point in affine coordinates (x, y)
// tmp - pointer to storage space for 4*len ints
// c - pointer to elliptic curve

#include "acl.h"
#include "acl_int.h"
#include "acl_config.h"

bool_t acl_p_ecc_chk(vect2 a, vect4 tmp, ecc_t *c)
{
    vect m, t1, t2, yy, fr; uint len;
    // tmp = tmp tmp t1 t2

    m = c->m; len = c->l; fr = c->fr;
    yy = xx + len; t1 = tmp + 2*len; t2 = t1 + len;

#if ACL_CHK_INF_ON_CURVE
    if (acl_zero(a, 2*len)) {
        return TRUE;
    }
#endif
    acl_p_sqr_fr(t1, xx);           // t1 = x^2
    if ((int) c->a == -3) {         // t1 = x^2 + a
        acl_p_mod_sub32(t1, t1, 3, m, len);
    } else if(c->a) {
        acl_p_mod_add(t1, t1, c->a, m, len);
    }
    acl_p_mul_fr(t1, t1, xx);       // t1 = x^3 + ax
    if ((int) c->b <= ACL_MAX_B) {  // t1 = x^3 + ax + b
        acl_p_mod_add32(t1, t1, (int) c->b, m, len);
    } else {
        acl_p_mod_add(t1, t1, c->b, m, len);
    }
    acl_p_sqr_fr(t2, yy);           // t2 = y^2
    if (c->t & ECC_A_MASK) {
        acl_mov(tmp, t1, len); acl_p_mod(t1, tmp, len, m + len, len);
        acl_mov(tmp, t2, len); acl_p_mod(t2, tmp, len, m + len, len);
    }
    return !acl_cmp(t1, t2, len);
}
