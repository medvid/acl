// convert projective to affine coordinates (x, y, z) -> (x', y', ??)
// where x' and y' are the affine coordinates (z iscorrupted)

// a - pointer to ecc point in projective coordinates (x, y, z)
// tmp - pointer to storage space for 5*len ints
// c - pointer to elliptic curve

#include "acl.h"
#include "acl_int.h"

void acl_2_ecc_aff(vect3 a, vect5 tmp, ecc_t *c)
{
    vect t1, t2, yy, zz, fr; uint len; int k;
    // tmp = tmp tmp tmp t1 t2

    len = c->l; fr = c->fr;
    yy = xx + len; zz = yy + len; t1 = tmp + 3*len; t2 = t1 + len;

    if (acl_zero(zz, len)) {
        acl_mov32(a, 0, 2*len);
    } else {
        acl_mov32(t1, 1, len);      // recover m from fr
        k = 0;
        while (c->fr[k]) {
            acl_bit_set(t1, c->fr[k]);
            k++;
        }
        acl_2_mod_inv(t2, zz, t1, tmp, len);
        acl_2_mul_fr(xx, xx, t2);
        acl_2_sqr_fr(t2, t2);
        acl_2_mul_fr(yy, yy, t2);
    }
}
