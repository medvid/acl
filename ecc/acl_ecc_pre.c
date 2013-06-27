// pre-computation for ecc point multiplication

// pre - where to store the pre-comp - space for (2^w-1)*len*2 ints
// p - affine point
// w - number of teeth in comb
// s - distance between teeth of comb (bitlength ofexponent <= w*s !!!)
// tmp - temporary storage (8*len ints)
// c - pointer to elliptic curve

// examples of width, spacing
// width   spacing     comment                            table size (ints)
//   1        0        no precomputation                      2*len
//   2     32*len/2                                           2*len*3
//   3     32*len/3+1  to make sure that w*s >= 32*len        2*len*7
//   4     32*len/4                                           2*len*15
//   5     32*len/5+1  same                                   2*len*31
//   6     32*len/6+1                                         2*len*63
//     ...
// actually it shouldn't be +1, but ceiling(32*len/width)

#include "acl.h"
#include "acl_int.h"

void acl_ecc_pre(vectN pre, vect2 p, uint w, uint s, vect8 tmp, ecc_t *c)
{
    vect zz, t1, base, h; uint len, len2, comb, i, j;
    // tmp[8*len] = x y z t1 t1 t1 t1 t1

    len = c->l; len2 = 2*len; zz = tmp + len2; t1 =tmp + 3*len;

    acl_mov(pre, p, len2);
    base = pre;
    comb = 1;
    for(i = 1; i < w; i++) {
        acl_ecc_pro(tmp, base, len);            // previous base point
        for(j = 0; j < s; j++) acl_ecc_dbl(tmp, t1, c); // 2^s * base
        acl_ecc_aff(tmp, t1, c);
        base += comb * len2;                    // new base point
        acl_mov(base, tmp, len2);
        comb <<= 1;
        for(j = 1; j < comb; j++) {             // the in betweens
            acl_ecc_pro(tmp, base, len);
            h = pre + (j - 1) * len2;           // already done
            acl_ecc_add(tmp, h, t1, c);
            acl_ecc_aff(tmp, t1, c);
            h = base + j * len2;                // destination
            acl_mov(h, tmp, len2);
        }
    }
}
