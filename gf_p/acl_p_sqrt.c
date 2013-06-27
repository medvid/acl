// res[len]^2 = a[len_a] mod m[len] res != a
// assuming m is an odd prime
// returns TRUE if square root exists, FALSE otherwise
// taken from http://mersennewiki.org/index.php/Modular_Square_Root

#include "acl.h"
#include "acl_int.h"

bool_t acl_p_sqrt(vect res, vect a, vect m, prng rnd, vect8 tmp, size_t len)
{
    uint m_inv, e, k, i; vect r_mod_m, r2_mod_m, t1, t2, t3;
    // tmp = tmp tmp tmp r_mod_m r2_mod_m t1 t2 t3

    r_mod_m = tmp + 3*len; r2_mod_m = r_mod_m + len;
    t1 = r2_mod_m + len; t2 = t1 + len; t3 = t2 + len;

    if(acl_zero(a, len)) {                      // if a == 0 -> res = 0;
        acl_mov(res, a, len); return TRUE;
    }
    acl_p_mod_sub32(tmp, a, 1, m, len);
    if(acl_zero(tmp, len)) {                    // if a == 1 -> res = 1;
        acl_mov(res, a, len); return TRUE;
    }
    acl_p_mont_pre(r_mod_m, r2_mod_m, &m_inv, m, len);
    acl_mov(t1, m, len);
    acl_rsh(t1, 1, len);
    acl_p_mont_exp(res, a, t1, len, m, tmp, m_inv, r2_mod_m, len);
    acl_p_mod_sub32(tmp, res, 1, m, len);
    if(!acl_zero(tmp, len)) return FALSE;       // if a^((m-1)/2) != 1, no sqrt
    switch(m[0] & 7) {
        case 3:
        case 7:                                 // if m mod 4 == 3
            acl_mov(t1, m, len);
            acl_rsh(t1, 2, len);
            acl_p_mod_add32(t1, t1, 1, 0, len);
            acl_p_mont_exp(res, a, t1, len, m, tmp,m_inv, r2_mod_m, len);
            return TRUE;
        case 5:                                 // if m mod 8 == 5
            acl_mov(t1, a, len);
            acl_p_mod_dbl(t1, 1, m, len);       // t1 = 2a
            acl_mov(t3, m, len);
            acl_rsh(t3, 3, len);                // t2 = (2a)^(m >> 3) == v
            acl_p_mont_exp(t2, t1, t3, len, m, tmp, m_inv, r2_mod_m, len);
            acl_p_mul_mont(t1, t1, r2_mod_m);   // into montgomery domain
            acl_p_mul_mont(t2, t2, r2_mod_m);
            acl_p_sqr_mont(res, t2);            // res = t2^2 == v^2
            acl_p_mul_mont(res, res, t1);       // res = 2a * v^2 == i
            acl_p_mod_sub(res, res, r_mod_m, m, len); // res = i - 1
            acl_p_mul_mont(res, res, t2);       // res = v * (i - 1)
            acl_p_mod_hlv(t1, 1, m, len);       // t1 = t1 / 2 == a
            acl_p_mul_mont(res, res, t1);       // res = a * v * (i - 1)
            acl_mov(tmp, res, len);             // out of montgomery domain
            acl_mov32(tmp + len, 0, len);
            acl_p_mont_red(res, tmp, m, m_inv, len);
            return TRUE;
        case 1:                                 // if m mod 8 == 1
            // res  t1  t2  t3
            // y    q   x   a
            //      v   d   w
            acl_p_mod_sub32(t1, m, 1, 0, len);
            e = acl_ctz(t1, len);
            acl_rsh(t1, e, len);                // t1 = q
            do {
                rnd(t2, len);               // t2 = x       res = x^q == z
                acl_p_mont_exp(res, t2, t1, len, m,tmp, m_inv, r2_mod_m, len);
                acl_p_mul_mont(res, res, r2_mod_m); // into montgomery domain
                acl_mov(t3, res, len);
                for(i=0; i<e-1; i++) { acl_p_sqr_mont(t3, t3); }
                acl_p_mod_add(t3, t3, r_mod_m, m, len);
            } while(!acl_zero(t3, len));        // repeat until x^m == -1
            acl_rsh(t1, 1, len);                // t1 = (q - 1)/2
            acl_p_mont_exp(t2, a, t1, len, m, tmp, m_inv, r2_mod_m, len);
            acl_p_mul_mont(t2, t2, r2_mod_m);   // x = a^((q - 1)/2)
            acl_p_mul_mont(t3, a, r2_mod_m);
            acl_p_mul_mont(t1, t2, t3);         // v = x * a
            acl_p_mul_mont(t3, t1, t2);         // w = v * x
            while(acl_cmp(t3, r_mod_m, len)) {  // while w != 1
                k = 0;
                acl_mov(t2, t3, len);           // t2 = w
                do {
                    k++;
                    acl_p_sqr_mont(t2, t2);
                } while(acl_cmp(t2, r_mod_m, len));
                acl_mov(t2, res, len);          // t2 = y
                for(i=0; i<e-k-1; i++) { acl_p_sqr_mont(t2, t2); }  // t2 = d
                acl_p_sqr_mont(res, t2);        // y = d^2
                e = k;
                acl_p_mul_mont(t1, t1, t2);     // v = d * v
                acl_p_mul_mont(t3, t3, res);    // w = w * y
            }                                   // w == 1, return v
            acl_mov(tmp, t1, len);              // out of montgomery domain
            acl_mov32(tmp + len, 0, len);
            acl_p_mont_red(res, tmp, m, m_inv, len);
            return TRUE;
        default:
            return FALSE;                   // m is composite - beyond our scope
    }
}
