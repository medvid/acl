// convert point a(x,y) to string (with or without compression)
// str - pointer to free space for resulting string
// a - pointer to ecc point in affine coordinates (x, y)
// comp - TRUE: use compression, FALSE: don't use compression
// tmp - unused, for compatibility with acl_2_ecc_p2str (set to zero)
// c - pointer to elliptic curve

// for exact description see SEC 1: Elliptic Curve Cryptography, p. 11
// if (x,y) == point at infinity then return "00"
// if comp == TRUE
//   if y mod 2 == 0 return "02xxxxxxxxxx..."
//   else            return "03xxxxxxxxxx..."
// else
//   return "04xxxxxxxxxx...yyyyyyyyy..."

#include "acl.h"

void acl_p_ecc_p2str(bytes str, vect2 a, bool_t comp, vect tmp, ecc_t *c)
{
    vect m; uint len, len_m;

    m = c->m; len = c->l;
    *str++ = '0';
    if (acl_zero(a, 2*len)) {
        *str++ = '0';
    } else {
        if (c->t & ECC_A_MASK) {
            m = m + len;
        }
        len_m = 4*len;
        while (((bytes) m)[len_m - 1] == 0) {
            len_m--;
        }
        if (comp) {
            if (a[len] & 1) {
                *str++ = '3';
            } else {
                *str++ = '2';
            }
            acl_hex2str_le(str, a, 2*len_m);
            str += 2*len_m;
        } else {
            *str++ = '4';
            acl_hex2str_le(str, a, 2*len_m);
            str += 2*len_m;
            acl_hex2str_le(str, a + len, 2*len_m);
            str += 2*len_m;
        }
    }
    *str = 0;
}
