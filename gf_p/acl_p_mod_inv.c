// res = a^(-1)*(2^e) mod m, m mod 2 == 1, res != a

#include "acl.h"

void acl_p_mod_inv(vect res, vect a, uint e, vect m, vect3 tmp, size_t len)
{
    int k; uint m_inv;

    k = e;
    acl_mov32(res, 0, len);
    if(!acl_zero(a, len)) {
        res[0] = 1;
        if(acl_cmp(res, a, len)) {
            k = acl_p_mont_inv(res, a, m, tmp, len);
            if(k == 0)
                acl_mov32(res, 0, len);
            else if(k < 0) {
                k = -k;
                acl_p_mod_sub(res, m, res, m, len);
            }
        }
        if(k < e)
            acl_p_mod_dbl(res, e - k, m, len);
        else {
            k = k - e;
            m_inv = acl_p_mont_m_inv(m);
            while(k >= 32*len) {
                k -= 32*len;
                acl_mov32(tmp + len, 0, len);
                acl_mov(tmp, res, len);
                acl_p_mont_red(res, tmp, m, m_inv, len);
            }
            if((k > len) && (k >= 32)) {
                acl_mov32(tmp, 0, 2*len);
                acl_mov(tmp + len - (k >> 5), res, len);
                acl_p_mont_red(res, tmp, m, m_inv, len);
                k -= 32*(k >> 5);
            }
            if(k) acl_p_mod_hlv(res, k, m, len);
        }
    }
}
