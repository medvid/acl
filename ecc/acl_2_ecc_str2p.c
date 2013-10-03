// convert string (with or without compression) to point a(x,y)
// a - resulting point in affine coordinates (x, y)
// str - string representation of point
// tmp - pointer to storage space for 6*len ints
// c - pointer to elliptic curve
// returns TRUE if the point is valid, FALSE otherwise

// for exact description see SEC 1: Elliptic Curve Cryptography, p. 12

// str can be one of the following:
// "00"
// "02xxxxxxxx..."
// "03xxxxxxxx..."
// "04xxxxxxxx...yyyyyyyyyyy..."
// for square root and half-trace calculation details see
// D. Hankerson, A. Menezes, and S.A. Vanstone,
// Guide to Elliptic Curve Cryptography, Springer-Verlag, 2004
// half-trace: p. 132, square root: p. 136

#include "acl.h"
#include "acl_int.h"

bool_t acl_2_ecc_str2p(vect2 a, bytes str, vect6 tmp, ecc_t *c)
{
    vect fr, m, t1, t2, yy; uint i, len, len_m;
    // tmp = tmp tmp tmp m t1 t2

    len = c->l; fr = c->fr;
    m = tmp + 3*len; t1 = m + len; t2 = t1 + len; yy = a + len;

    if (str[1] == '0') {
        acl_mov32(a, 0, 2*len);
    } else {
        acl_mov32(m, 1, len);      // recover m from fr
        i = 0;
        while (fr[i]) {
            acl_bit_set(m, fr[i]);
            i++;
        }
        len_m = 4*len;
        while (((bytes) m)[len_m - 1] == 0) {
            len_m--;
        }
        acl_str2hex_le(a, len, str + 2, 2*len_m);
        for (i = fr[0]; i < 32*len; i++) {
            if(acl_bit(a, i, len)) {
                return FALSE;
            }
        }
        if (str[1] == '4') {
            acl_str2hex_le(yy, len, str + 2 + 2*len_m, 2*len_m);
            for(i = fr[0]; i < 32*len; i++) {
                if(acl_bit(yy, i, len)) {
                    return FALSE;
                }
            }
        } else {
            if (acl_zero(a, len)) {
                if ((int) c->b == 1)
                    acl_mov32(yy, 1, len);
                else {
                    acl_mov(yy, c->b, len);
                    for (i = 0; i < fr[0] - 1; i++) {
                        acl_2_sqr_fr(yy, yy);       // calculate square root
                    }
                }
            } else {
                acl_2_mod_inv(t2, a, m, tmp, len);
                acl_2_sqr_fr(t2, t2);
                if ((int) c->b != 1) {
                    acl_2_mul_fr(t2, t2, c->b);
                }
                if ((int) c->a == 1) {
                    acl_xor32(t2, t2, 1, len);
                } else if (c->a) {
                    acl_xor(t2, t2, c->a, len);
                }
                acl_xor(t2, t2, a, len);
                acl_mov(t1, t2, len);
                for (i = 1; i <= (fr[0] >> 1); i++) {
                    acl_2_sqr_fr(t2, t2);       // calculate half-trace
                    acl_2_sqr_fr(t2, t2);
                    acl_xor(t1, t1, t2, len);
                }
                if (((str[1] - '2') ^ t1[0]) & 1) {
                    acl_xor32(t1, t1, 1, len);
                }
                acl_2_mul_fr(yy, a, t1);
            }
        }
        if (!acl_2_ecc_chk(a, tmp, c)) return FALSE;
    }
    return TRUE;
}
