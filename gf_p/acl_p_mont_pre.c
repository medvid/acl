// precomputation for montgomery arithmetic

// r_mod_m = 2^(32*len) mod m
// r2_mod_m = 2^(64*len) mod m
// m_inv = -m^(-1) mod 2^32
// len is the length of r_mod_m, r2_mod_m and m in 32-bit words

#include "acl.h"

void acl_p_mont_pre(vect r_mod_m, vect r2_mod_m, uint *m_inv, \
                    vect m, size_t len)
{
    int i;

    if(m_inv) *m_inv = acl_p_mont_m_inv(m);
    if(r_mod_m) {
        i = acl_log2(m, len);
        acl_mov32(r_mod_m, 0, len);
        acl_bit_set(r_mod_m, i);
        i = 32 * len - i;
        acl_p_mod_dbl(r_mod_m, i, m, len);
    }
    if(r2_mod_m) {
        if(r_mod_m) {
            acl_mov(r2_mod_m, r_mod_m, len);
            i = 32 * len;
        } else {
            i = acl_log2(m, len);
            acl_mov32(r2_mod_m, 0, len);
            acl_bit_set(r2_mod_m, i);
            i = 64 * len - i;
        }
        acl_p_mod_dbl(r2_mod_m, i, m, len);
    }
}
