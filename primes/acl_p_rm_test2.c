// rabin-miller test with a == 2
// returns false if 2 proves the compositeness of m, true otherwise
// len is the length of m, r_mod_m, res in 32-bit words
// tmp is used for temporary storage;
//   its size should be at least (3 x len) 32-bit words

#include "acl.h"
#include "acl_int.h"

bool_t acl_p_rm_test2(vect m, vect3 tmp, uint m_inv, vect r_mod_m, size_t len)
{
    int i, k; vect res;     // tmp tmp res

    res = tmp + 2*len;

    i = acl_log2(m, len);
    if(i < 2) return FALSE;
    k = 1;
    while(!acl_bit(m, k, len)) k++;
    acl_mov(res, r_mod_m, len);
    acl_p_mod_dbl(res, 1, m, len);
    while(i > k) {
        i--;
        acl_p_sqr_mont(res, res);
        if(acl_bit(m, i, len)) acl_p_mod_dbl(res, 1, m, len);
    }
    if(acl_cmp(res, r_mod_m, len) == 0) return TRUE;
    acl_p_mod_add(tmp, res, r_mod_m, m, len);
    if(acl_zero(tmp, len)) return TRUE;
    k--;
    while(k--) {
        acl_p_sqr_mont(res, res);
        acl_p_mod_add(tmp, res, r_mod_m, m, len);
        if(acl_zero(tmp, len)) return TRUE;
    }
    return FALSE;
}
