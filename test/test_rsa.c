// test prime generation and RSA algorithm

#include "acl.h"
#include "system.h"

#define NO_TESTS 100    // number of times to run the main loop
#define NO_TESTS2 1     // number of fermat tests to try for prime p
#define RM_K 8          // k-parameter for rabin miller test
#define PRNG 2          // 0: AES, 1: SHA, 2: BBS

#define LEN 8           // length of primes to generate in 32-bit words
uint p[LEN], q[LEN], r2_mod_p[LEN], r2_mod_q[LEN];
uint dmp1[LEN], dmq1[LEN], iqmp[LEN];
uint n[2*LEN], e[2*LEN], d[2*LEN];
uint dt[2*LEN], pt[2*LEN], ct[2*LEN], r_mod_m[2*LEN], r2_mod_m[2*LEN];
uint tmp[7*LEN];

bool_t test_rsa(void) {

    uint i, j, k, h, avg1, avg2, avg3, m_inv, p_inv, q_inv;
    size_t len = LEN;
    prng rnd_strong;

    put_str("\nrnd");

    // choose a strong pseudo-random number generator
#if PRNG == 0
    rnd_strong = &acl_prng_aes;
    acl_prng_aes_init(&acl_prng_lc);
#elif PRNG == 1
    rnd_strong = &acl_prng_sha;
    acl_prng_sha_init(&acl_prng_lc);
#else
    rnd_strong = &acl_prng_bbs;
    acl_prng_bbs_init(&acl_prng_lc, &acl_prng_lc, tmp);
#endif

    // rnd_strong(p, 8);      // use to measure throughput of prngs
    put_str("\nrsa");
    for(i = 0; i < NO_TESTS; i++) {
        // generate two primes p and q (with the 2 most significant bits set)
rsa_again:
        restart_timer(0);
        acl_p_rnd_prime(p, tmp, RM_K, 32*len-2, &acl_prng_lc, rnd_strong, len);
        h = stop_timer(0); put_val("\ngen = ", h);
        //put_str("\np = 0x"); put_vect(p, len);  // print it

        restart_timer(0);
        acl_p_rnd_prime(q, tmp, RM_K, 32*len-2, &acl_prng_lc, rnd_strong, len);
        h = stop_timer(0); put_val("\ngen = ", h);
        //put_str("\nq = 0x"); put_vect(q, len);  // print it

        // test fermat's little theorem
        acl_p_mont_pre(0, r2_mod_m, &m_inv, p, len); // montgomery
        for(j = 0; j < NO_TESTS2; j++) {
            acl_prng_lc(tmp, len);          // choose random number
            acl_p_mod(pt, tmp, len, p, len); // make sure it's smaller than p
            acl_p_mont_exp(ct, pt, p, len, p, tmp, m_inv, r2_mod_m, len);
                                        // raise itto the power of p
            if(acl_cmp(pt, ct, len)) {  // should be the same as before
                put_str("\np not prime"); return TRUE;
            }
        }

        // e = 65537
        acl_mov32(e, 0x10001, 2*len);      // expand e to 2*len

        // precomputation for RSA / CRT
        if(!acl_rsa_pre(n, d, dmp1, dmq1, iqmp, e, p, q, tmp, len))
            goto rsa_again;
        acl_p_mont_pre(0, r2_mod_p, &p_inv, p, len);
        acl_p_mont_pre(0, r2_mod_q, &q_inv, q, len);
        //put_str("\nn = 0x"); put_vect(n, 2*len);  // print it
        //put_str("\nd = 0x"); put_vect(d, 2*len);  // print it

        avg1 = 0; avg2 = 0; avg3 = 0;
        for(k = 0; k < 8; k++) {
            // choose plaintext
            acl_p_mont_pre(r_mod_m, r2_mod_m, &m_inv, n, 2*len); // montgomery
            acl_prng_lc(tmp, 2*len);             //choose random number
            acl_p_mod(pt, tmp, 2*len, n, 2*len); //make sure it's < n
            //put_str("\npt = 0x"); put_vect(pt, 2*len);  // print it

            // RSA encryption
            restart_timer(0);
            acl_p_mont_exp(ct, pt, e, 2*len, n, tmp, m_inv, r2_mod_m, 2*len);
                                                // encode plaintext
            h = stop_timer(0); avg1 += h; put_val("\nenc = ", h);
            //put_str("\nct = 0x"); put_vect(ct, 2*len);  // print it

            // RSA decryption (long)
            restart_timer(0);
            acl_p_mont_exp(dt, ct, d, 2*len, n, tmp, m_inv, r2_mod_m, 2*len);
            h = stop_timer(0); avg2 += h; put_val("dec = ", h);
            //put_str("\ndt = 0x"); put_vect(dt, 2*len);  // print it

            // compare decrypted ciphertext with plaintext
            if(acl_cmp(pt, dt, 2*len)) {
                put_str("\nerror: long"); return TRUE;
            }

            // RSA decryption (CRT)
            restart_timer(0);
            acl_rsa_crt(dt, ct, p, r2_mod_p, p_inv,q, r2_mod_q, q_inv, \
                    dmp1, dmq1, iqmp, tmp, len);
            h = stop_timer(0); avg3 += h; put_val("crt = ", h);
            //put_str("\ndt = 0x"); put_vect(dt, 2*len);  // print it
            // compare decrypted ciphertext with plaintext
            if(acl_cmp(pt, dt, 2*len)) {
                put_str("\nerror: crt"); return TRUE;
            }
        }
        put_val("\navg = ", avg1 >> 3);
        put_val("  avg = ", avg2 >> 3);
        put_val("  avg = ", avg3 >> 3);
    }
    return FALSE;
}
