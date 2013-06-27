// a[len_a] = a[len_a] div m[len] (tmp[len])

#include "acl.h"

void acl_p_div(vect a, size_t len_a, vect m, vect tmp, size_t len)
{
    int k; int h;

    if(len_a < len) {
        acl_mov(tmp, a, len_a);
        acl_mov32(tmp + len_a, 0, len - len_a); k =0;
    } else {
        acl_mov(tmp, a + (len_a - len), len); k = 32 * (len_a - len);
    }
    while(acl_cmp(tmp, m, len) >= 0) { acl_rsh(tmp,1, len); k++; }
    for(h = 32*len_a; h > k; h--) acl_bit_clr(a, h - 1);
    while(k--) {
        h = acl_p_mod_dbl(tmp, 1, m, len);
        if(acl_bit(a, k, len_a)) h += acl_p_mod_add32(tmp, tmp, 1, m, len);
        if(h) acl_bit_set(a, k);
        else acl_bit_clr(a, k);
    }
}
