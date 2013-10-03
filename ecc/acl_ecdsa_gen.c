// ecdsa signature generation; the length of all arrays is len = c->ln
// (the length of the order of the base point)
// except for e (the hash) whose length is "len_e"

// r, s - resulting signature
// e - hash
// e_len - length of hash in 32-bit words
// dA - private key
// base - the curve's base-point or its pre-computation table
// wi - width of comb
// sp - spacing of comb (ignored if wi == 1)
// rnd_strong - random number generator
// tmp - temporary storage (9*len ints)
// c - pointer to elliptic curve

#include "acl.h"
#include "acl_int.h"

void acl_ecdsa_gen(vect r, vect s, vect e, size_t len_e, vect dA, \
                   vectN base, uint wi, uint sp, \
                   prng rnd_strong, vect9 tmp, ecc_t *c)
{
    uint len, m_inv; vect t1, t2, a, k, m;
    // tmp = tmp tmp t1 t2 tmp x y z k

    m = c->n; len = c->ln; a = tmp + 5*len; k = a +3*len;
    t1 = tmp + 2*len; t2 = t1 + len;

aeg_again:
    rnd_strong(t1, len);
    acl_p_mod(k, t1, len, m, len);          // k = rnd mod n
    if (acl_zero(k, len)) {
        goto aeg_again;
    }
    acl_ecc_mul(a, base, 0, wi, sp, k, 0, len, tmp,c);  // a = k * G
    acl_p_mod(r, a, c->l, m, len);          // r = x1 mod n
    if (acl_zero(r, len)) {
        goto aeg_again;
    }
    acl_p_mont_pre(0, t1, &m_inv, m, len);
    acl_p_mul_mont(t1, r, t1);              // t1 =r * R
    acl_p_mul_mont(t1, t1, dA);             // t1 =r * dA
    acl_p_mod(t2, e, len_e, m, len);        // t2 =e
    acl_p_mod_add(t1, t1, t2, m, len);      // t1 =e + r * dA
    acl_p_mod_inv(t2, k, 32*len, m, a, len);    // t2 = k^(-1) * R
    acl_p_mul_mont(s, t1, t2);              // s = k^(-1) * (e + r * dA)
    if (acl_zero(s, len)) {
        goto aeg_again;
    }
}
