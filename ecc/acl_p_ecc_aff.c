// convert projective to affine coordinates (x, y, z) -> (x', y', ??)
// where x' and y' are the affine coordinates (z iscorrupted)

// a - pointer to ecc point in projective coordinates (x, y, z)
// tmp - pointer to storage space for 4*len ints
// c - pointer to elliptic curve

#include "acl.h"
#include "acl_int.h"

void acl_p_ecc_aff(vect3 a, vect4 tmp, ecc_t *c)
{
    vect m, t1, yy, zz, fr; uint len;
    // tmp = tmp tmp tmp t1

    m = c->m; len = c->l; fr = c->fr;
    yy = xx + len; zz = yy + len; t1 = tmp + 3*len;

    if (c->t & ECC_A_MASK) {
        m = m + len;
        acl_mov(t1, zz, len); acl_p_mod(zz, t1, len, m, len);
    }
    if (acl_zero(zz, len)) {
        acl_mov32(a, 0, 2*len);
    } else {
        acl_p_mod_inv(t1, zz, 0, m, tmp, len);
        acl_p_mul_fr(yy, yy, t1);
        acl_p_sqr_fr(t1, t1);
        acl_p_mul_fr(yy, yy, t1);
        acl_p_mul_fr(xx, xx, t1);
        acl_mov(t1, xx, len); acl_p_mod(xx, t1, len, m, len);
        acl_mov(t1, yy, len); acl_p_mod(yy, t1, len, m, len);
    }
}
