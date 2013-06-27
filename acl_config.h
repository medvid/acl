#ifndef ACL_CONFIG_H
#define ACL_CONFIG_H

// Some magic numbers

// in routine acl_p_ecc_chk
#define ACL_MAX_B 9     // = maximum "small" value of b in ECC over GF(p)
                        // any larger value is considered a pointer
                        // the largest "small" value used in SECG curves is 7
                        // (secp160k1, secp256k1)

// in routines acl_p_ecc_chk and acl_2_ecc_chk
#define ACL_CHK_INF_ON_CURVE 0
    // determines how routines acl_p_ecc_chk and acl_2_ecc_chk treat
    // the point at infinity (x == 0, y == 0)
    // 0 -> acl_x_ecc_chk(point-at-infinity) returns FALSE
    // 1 -> acl_x_ecc_chk(point-at-infinity) returns TRUE
    // depending on where the acl_x_ecc_chk routines are used,
    // this can be used to make the point at infinity valid/invalid
    // or to force the user to check for the point at infinity separately

    // for example, in our library testing programs, the acl_x_ecc_chk routines
    // are only used to check for a valid base point. the point at infinity
    // is not a valid base point, so we set ACL_CHK_INF_ON_CURVE to 0.
    // this way we don't have to test for the point at infinity separately.

// There is an magic number in GF_p/acl_p_fr - called BORDER

// PRNG configuration
#define ACL_PRNG_AES_SIZE 4     // 4, 6, 8 = 128, 192, 256
#define ACL_PRNG_BBS_MONT 1     // get bits from montgomery representation
                                // of x^(2i) (see acl_prng_bbs.c)
#define ACL_PRNG_BBS_SIZE 8     // length of p, q
#define ACL_PRNG_BBS_K 8        // rabin-miller parameter for BBS primes

// number of entries in acl_pop_table
// to change this, you must also generate a new table
#define ACL_POP_SIZE 32

#endif
