// rabin-miller test with generic a
// returns false if a proves the compositeness of m, true otherwise
// len is the length of a, m, r_mod_m, r2_mod_m, res, a_r in 32-bit words
// tmp is used for temporary storage;
// its size should be at least (4 x len) 32-bit words

#include "acl.h"
#include "acl_int.h"

bool_t acl_p_rm_test(vect a, vect m, vect4 tmp, uint m_inv, \
                     vect r_mod_m, vect r2_mod_m, size_t len)
{
    uint i, k; vect res, a_r;    // tmp tmp res a_r

    res = tmp + 2*len; a_r = res + len;
    i = acl_log2(m, len);
    if (i < 2) {
        return FALSE;
    }
    k = 1;
    while (!acl_bit(m, k, len)) {
        k++;
    }
    acl_p_mul_mont(a_r, a, r2_mod_m);
    acl_mov(res, a_r, len);
    while (i > k) {
        i--;
        acl_p_sqr_mont(res, res);
        if (acl_bit(m, i, len)) {
            acl_p_mul_mont(res, res, a_r);
        }
    }
    if (acl_cmp(res, r_mod_m, len) == 0) {
        return TRUE;
    }
    acl_p_mod_add(tmp, res, r_mod_m, m, len);
    if (acl_zero(tmp, len)) {
        return TRUE;
    }
    k--;
    while (k--) {
        acl_p_sqr_mont(res, res);
        acl_p_mod_add(tmp, res, r_mod_m, m, len);
        if (acl_zero(tmp, len)) {
            return TRUE;
        }
    }
    return FALSE;
}
