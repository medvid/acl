// calculate values necessary for RSA
// input: e, p, q (p and q have to be stored in ram- they get dec'd and inc'd)
// output: n, d
// output: dmp1, dmq1, iqmp (if 0, will not be generated)
// returns false if gcd(phi, e) != 1
// len is the length of p, q in 32-bit words
// tmp is used for temporary storage (6 x len) 32-bit words

#include "acl.h"

#define phi n

bool_t acl_rsa_pre(vect2 n, vect2 d, vect dmp1, vect dmq1, vect iqmp, \
                   vect2 e, vect p, vect q, vect6 tmp, size_t len)
{
    p[0]--; q[0]--;                                 // p = p - 1, q = q - 1
    acl_p_mul(phi, p, q, len);                      // phi = (p - 1) * (q - 1)
    if(!acl_p_coprime(e, phi, tmp, 2*len)) return FALSE;
    acl_p_mod_inv(d, phi, 0, e, tmp, 2*len);        // d = phi^(-1) mod e
    acl_p_mul(tmp, d, phi, 2*len);                  // tmp = d * phi
    acl_p_mod_sub32(tmp, tmp, 1, 0, 4*len);         // tmp = d * phi - 1
    acl_p_div(tmp, 4*len, e, tmp + 4*len, 2*len);   // tmp = tmp / e
    acl_p_mod(d, tmp, 4*len, phi, 2*len);           // d = tmp mod phi
    acl_p_mod_sub(d, phi, d, phi, 2*len);           // d = -d
    if(dmp1) acl_p_mod(dmp1, d, 2*len, p, len);     // dmp1 = d mod (p - 1)
    if(dmq1) acl_p_mod(dmq1, d, 2*len, q, len);     // dmq1 = d mod (q - 1)
    p[0]++; q[0]++;                                 // p = p + 1, q = q + 1
    if(iqmp) acl_p_mod_inv(iqmp, q, 0, p, tmp, len); // iqmp = q^(-1) mod p
    acl_mov32(tmp + len, 0, len);
    acl_mov(tmp, p, len);
    acl_p_mod_add(phi, phi, tmp, 0, 2*len);         // phi = phi + p
    acl_mov(tmp, q, len);
    acl_p_mod_add(phi, phi, tmp, 0, 2*len);         // phi = phi + q
    acl_p_mod_sub32(n, phi, 1, 0, 2*len);           // n = phi - 1
    return TRUE;
}
