// test ECDSA operation

#include "system.h"
#include "acl.h"

#define PART 0      // 0-5 (uVision3 won't simulatea target > 16kB)
#define LEN 18      // the biggest curve (acl_sect571r1) needs 18 32-bit words
                    // of storage for each field element
#define WIDTH 4     // width of comb used with pre-computation
                    // memory required grows exponentially:
                    // 8*LEN*((2^WIDTH)-1) bytes for each ECC point
                    // 1: no pre-computation
                    // 2 - 5: realistic values
                    // 6: requires 2kB for smallest, 9kB for biggest curve

uint tmp[(LEN+1)*10];
uint a[LEN*3];
uint pre1[2*LEN*((1<<WIDTH)-1)];
uint pre2[2*LEN*((1<<WIDTH)-1)];
uint hash[5] = { 0x12452643, 0xabcda431, 0xff509ac8, 0xb909cd90, 0x5329cb0a };
uint r[LEN+1];
uint s[LEN+1];
uint dA[LEN+1];
uint qA[LEN*3];

#if PART == 0
    #define CURVES 5
    const ecc_t *ecdsa_list[] = {
                        &acl_secp112r1, &acl_secp112r2,
                        &acl_secp128r1, &acl_secp128r2,
        &acl_secp160k1,
    };
#elif PART == 1
    #define CURVES 5
    const ecc_t *ecdsa_list[] = {
                        &acl_secp160r1, &acl_secp160r2,
        &acl_secp192k1, &acl_secp192r1,
        &acl_secp224k1
    };
#elif PART == 2
    #define CURVES 3
    const ecc_t *ecdsa_list[] = {
                        &acl_secp224r1,
        &acl_secp256k1, &acl_secp256r1,
    };
#elif PART == 3
    #define CURVES 2
    const ecc_t *ecdsa_list[] = {
                        &acl_secp384r1,
                        &acl_secp521r1
    };
#elif PART == 4
    #define CURVES 11
    const ecc_t *ecdsa_list[] = {
                        &acl_sect113r1, &acl_sect113r2,
                        &acl_sect131r1, &acl_sect131r2,
        &acl_sect163k1, &acl_sect163r1, &acl_sect163r2,
                        &acl_sect193r1, &acl_sect193r2,
        &acl_sect233k1, &acl_sect233r1
};
#elif PART == 5
#define CURVES 7
    const ecc_t *ecdsa_list[] = {
        &acl_sect239k1,
        &acl_sect283k1, &acl_sect283r1,
        &acl_sect409k1, &acl_sect409r1,
        &acl_sect571k1, &acl_sect571r1
};
#endif

bool_t test_ecdsa(void) {

    ecc_t *c; int i, j, k, wi, sp; bool_t res; uint h, avg1, avg2, avg3, avg4;

    for(j=0; j<100; j++) {
        for(i=0; i<CURVES; i++) {
            c = (ecc_t *) ecdsa_list[i];

            // print name of curve
            put_str("\n\n");
            put_str((char *) c->s);

            // width and spacing of comb
            wi = WIDTH; // number of teeth of the comb
            sp = 1; // spacing = how many bits apart the teeth of the comb are
            while(sp * wi < 32 * c->ln) sp++; // sp >= 32*len / width !!!

            // generate private key for ecdsa
            acl_prng_lc(tmp, c->ln);
            acl_p_mod(dA, tmp, c->ln, c->n, c->ln);

            // generate public key for ecdsa
            acl_ecc_mul(qA, c->g, 0, 1, 0, dA, 0, c->ln, tmp, c);

            // generate pre-computation for base point
            acl_ecc_pre(pre1, c->g, wi, sp, tmp, c);

            // generate pre-computation for qA
            acl_ecc_pre(pre2, qA, wi, sp, tmp, c);

            avg1 = 0; avg2 = 0; avg3 = 0; avg4 = 0;
            for(k = 0; k < 4; k++) {
                // generate ecdsa signature withoutpre-computation
                restart_timer(0);
                acl_ecdsa_gen(r, s, hash, 5, dA, pre1, 1, 0, \
                        &acl_prng_lc, tmp, c);
                h = stop_timer(0); avg1 += h; put_val("\ngen =", h);

                // verify ecdsa signature without pre-computation
                restart_timer(0);
                res = acl_ecdsa_ver(r, s, hash, 5, pre2, pre1, 1, 0, tmp, c);
                h = stop_timer(0); avg2 += h; put_val(" ver =", h);
                if(!res) { put_str(" ecdsa"); return TRUE; }

                // generate ecdsa signature with pre-computation
                restart_timer(0);
                acl_ecdsa_gen(r, s, hash, 5, dA, pre1, wi, sp, \
                        &acl_prng_lc, tmp, c);
                h = stop_timer(0); avg3 += h; put_val(" gen pre =", h);

                // verify ecdsa signature with pre-computation
                restart_timer(0);
                res = acl_ecdsa_ver(r, s, hash, 5, pre2, pre1, wi, sp, tmp, c);
                h = stop_timer(0); avg4 += h; put_val(" ver pre =", h);
                if(!res) { put_str(" ecdsa"); return TRUE; }
            }
            put_str("\naverages\n");
            put_val("gen =", avg1 >> 2);
            put_val(" ver =", avg2 >> 2);
            put_val(" gen pre =", avg3 >> 2);
            put_val(" ver pre =", avg4 >> 2);
        }
    }
    return FALSE;
}
