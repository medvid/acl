// ecdsa signature verification; the length of all arrays is len = c->ln
// (the length of the order of the base point)
// except for e (the hash) whose length is "len_e"

// r, s - signature to verify
// e - hash
// e_len - length of hash in 32-bit words
// qA - public key (ec point) or its pre-computation table
// base - the curve's base point or its pre-computation table
// wi - width of comb
// sp - spacing of comb (ignored if wi == 1)
// tmp - temporary storage (10*len ints)
// c - pointer to elliptic curve

#include "acl.h"
#include "acl_int.h"

bool_t acl_ecdsa_ver(vect r, vect s, vect e, size_t len_e, vectN qA, \
                     vectN base, uint wi, uint sp, vect10 tmp, ecc_t *c)
{
    uint len, m_inv; vect a, k, l, m;
    // tmp = tmp tmp tmp tmp tmp x y z k l

    m = c->n; len = c->ln; a = tmp + 5*len; k = a +3*len; l = k + len;

    if (acl_zero(r, len)) {
        return FALSE;
    }
    if (acl_zero(s, len)) {
        return FALSE;
    }
    if (acl_cmp(r, m, len) > 0) {
        return FALSE;
    }
    if (acl_cmp(s, m, len) > 0) {
        return FALSE;
    }
    acl_p_mod(k, e, len_e, m, len);         // k = e
    acl_p_mod_inv(l, s, 32*len, m, a, len); // l = s^(-1) * R
    m_inv = acl_p_mont_m_inv(m);
    acl_p_mul_mont(k, k, l);                // k = e * s^(-1)
    acl_p_mul_mont(l, l, r);                // l = r * s^(-1)
    acl_ecc_mul(a, base, qA, wi, sp, k, l, len, tmp, c);    // a = k*G + l*qA
    acl_p_mod(k, a, c->l, m, len);          // k = x1 mod n
    if (acl_cmp(r, k, len)) {                // k !=r ?
        return FALSE;
    }
    return TRUE;
}
