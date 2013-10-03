// res = x^e mod m (using montgomery exponentiation)
// len is the length of res, x, m, r2_mod_m in 32-bit words
// len_e is the length of e in 32-bit words
// tmp is (3 x len) big; tmp and x_r are used for temporary storage

#include "acl.h"
#include "acl_int.h"

void acl_p_mont_exp(vect res, vect x, vect e, size_t len_e, vect m, vect3 tmp, \
                    uint m_inv, vect r2_mod_m, size_t len)
{
    int i; vect x_r; // tmp tmp x_r

    x_r = tmp + 2*len;
    i = acl_log2(e, len_e);
    if (i != -1) {
        acl_p_mul_mont(x_r, x, r2_mod_m);
        acl_mov(res, x_r, len);
        while (i--) {
            acl_p_sqr_mont(res, res);
            if (acl_bit(e, i, len_e)) {
                acl_p_mul_mont(res, res, x_r);
            }
        }
        acl_mov(tmp, res, len);
        acl_mov32(tmp + len, 0, len);
        acl_p_mont_red(res, tmp, m, m_inv, len);
    } else {
        acl_mov32(res, 1, len);
    }
}
