// convert point a(x,y) to string (with or without compression)
// str - pointer to free space for resulting string
// a - pointer to ecc point in affine coordinates (x, y)
// comp - TRUE: use compression, FALSE: don't use compression
// tmp - pointer to storage space for 5*len ints
// c - pointer to elliptic curve

// for exact description see SEC 1: Elliptic Curve Cryptography, p. 11
// if (x,y) == point at infinity then return "00"
// if comp == TRUE
//   if x == 0 return "02xxxxxxxxxx..."
// else
//   if y/x mod z == 0 return "02xxxxxxxxxx..."
//   else              return "03xxxxxxxxxx..."
// if comp == FALSE
// return "04xxxxxxxxxx...yyyyyyyyy..."

#include "acl.h"
#include "acl_int.h"

void acl_2_ecc_p2str(bytes str, vect2 a, bool_t comp, vect5 tmp, ecc_t *c)
{
    vect t1, t2, fr; uint len, len_m; int k;
    // tmp = tmp tmp tmp t1 t2

    len = c->l; fr = c->fr; t1 = tmp + 3*len; t2 = t1 + len;

    *str++ = '0';
    if(acl_zero(a, 2*len))
        *str++ = '0';
    else {
        acl_mov32(t1, 1, len);      // recover m from fr
        k = 0;
        while(c->fr[k]) {
            acl_bit_set(t1, c->fr[k]);
            k++;
        }
        len_m = 4*len;
        while(((bytes) t1)[len_m - 1] == 0) len_m--;
        if(comp) {
            acl_2_mod_inv(t2, a, t1, tmp, len);
            acl_2_mul_fr(t1, a + len, t2);
            if(t1[0] & 1) *str++ = '3';
            else *str++ = '2';
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
