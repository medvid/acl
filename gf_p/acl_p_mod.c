// res[len] = a[len_a] mod m[len]
// does not work in-place (res != a) !!!

#include "acl.h"

void acl_p_mod(vect res, vect a, size_t len_a, vect m, size_t len)
{
    int k;

    if (len_a < len) {
        acl_mov(res, a, len_a);
        acl_mov32(res + len_a, 0, len - len_a); k =0;
    } else {
        acl_mov(res, a + (len_a - len), len); k = 32 * (len_a - len);
    }
    while (acl_cmp(res, m, len) >= 0) {
        acl_rsh(res,1, len); k++;
    }
    while (k--) {
        acl_p_mod_dbl(res, 1, m, len);
        if (acl_bit(a, k, len_a)) {
            acl_p_mod_add32(res, res, 1, m, len);
        }
    }
}
