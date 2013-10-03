// convert string (with or without compression) to point a(x,y)
// a - resulting point in affine coordinates (x, y)
// str - string representation of point
// tmp - pointer to storage space for 9*len ints
// c - pointer to elliptic curve
// returns TRUE if the point is valid, FALSE otherwise

// for exact description see SEC 1: Elliptic Curve Cryptography, p. 12
// str can be one of the following:
// "00"
// "02xxxxxxxx..."
// "03xxxxxxxx..."
// "04xxxxxxxx...yyyyyyyyyyy..."

#include "acl.h"

bool_t acl_p_ecc_str2p(vect2 a, bytes str, vect9 tmp, ecc_t *c)
{
    vect m, t1, yy; uint len, len_m, h;

    m = c->m; len = c->l; t1 = tmp + 8*len; yy = a + len;

    if (str[1] == '0') {
        acl_mov32(a, 0, 2*len);
    } else {
        if (c->t & ECC_A_MASK) {
            m = m + len;
        }
        len_m = 4*len;
        while (((bytes) m)[len_m - 1] == 0) {
            len_m--;
        }
        acl_str2hex_le(a, len, str + 2, 2*len_m);
        if (acl_cmp(a, m, len) >= 0) {
            return FALSE;
        }
        if (str[1] == '4') {
            acl_str2hex_le(yy, len, str + 2 + 2*len_m, 2*len_m);
            if (acl_cmp(yy, m, len) >= 0) {
                return FALSE;
            }
            if(!acl_p_ecc_chk(a, tmp, c)) {
                return FALSE;
            }
        } else {
            h = str[1] - '2';
            if (h & ~1) {
                return FALSE;
            }
            acl_p_ecc_chk(a, tmp, c);   // now tmp+2*len holds (x^3 + ax + b)
            acl_mov(yy, tmp + 2*len, len);
            if (!acl_p_sqrt(t1, yy, m, &acl_prng_lc,tmp, len)) {
                return FALSE;
            }
            acl_mov(yy, t1, len);
            if ((h ^ yy[0]) & 1) {
                acl_p_mod_sub(yy, m, yy, m, len);
            }
        }
    }
    return TRUE;
}
