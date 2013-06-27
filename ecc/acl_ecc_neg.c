// negate a point in projective or affine coordinates

// a - pointer to ecc point in projective (x, y, z) or affine (x, y) coordinates
// c - pointer to elliptic curve

#include "acl.h"

void acl_ecc_neg(vect3 a, ecc_t *c)
{
    if((c->t & ECC_F_MASK) == ECC_2)    // over GF(2)
        acl_xor(a + c->l, a, a + c->l, c->l);
    else                                // over GF(p)
        acl_p_mod_sub(a + c->l, c->m, a + c->l, c->m, c->l);
}
