// res = a^(-1) mod poly, poly mod z == 1, res != a

#include "acl.h"

void acl_2_mod_inv(vect res, vect a, vect poly, vect3 tmp, size_t len)
{
    uint k;

    acl_mov32(res, 0, len);
    if (!acl_zero(a, len)) {
        res[0] = 1;
        if (acl_cmp(res, a, len)) {
            k = acl_2_mont_inv(res, a, poly, tmp, len);
            if (k == 0) {
                acl_mov32(res, 0, len);
            } else {
                acl_2_mod_hlv(res, k, poly, len);
            }
        }
    }
}
