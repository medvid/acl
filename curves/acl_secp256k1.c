#include "acl.h"

const uint acl_secp256k1_m[] = {
    0xfffffc2f, 0xfffffffe, 0xffffffff, 0xffffffff,0xffffffff, 0xffffffff,
    0xffffffff, 0xffffffff
};

const uint acl_secp256k1_fr[] = { 256, 32, 0x03d1 };

const uint acl_secp256k1_g[] = {
    0x16f81798, 0x59f2815b, 0x2dce28d9, 0x029bfcdb,0xce870b07, 0x55a06295,
    0xf9dcbbac, 0x79be667e,
    0xfb10d4b8, 0x9c47d08f, 0xa6855419, 0xfd17b448,0x0e1108a8, 0x5da4fbfc,
    0x26a3c465, 0x483ada77
};

const uint acl_secp256k1_o[] = {
    0xd0364141, 0xbfd25e8c, 0xaf48a03b, 0xbaaedce6,0xfffffffe, 0xffffffff,
    0xffffffff, 0xffffffff
};

const ecc_t acl_secp256k1 = {
    "secp256k1",
    ECC_P + ECC_K,
    8,
    (vect) acl_secp256k1_m,
    (list) acl_secp256k1_fr,
    (vect2) acl_secp256k1_g,
    (vect) 0,
    (vect) 7,
    (vect) acl_secp256k1_o,
    8,
    1,
    (void *) &acl_p_ecc_func
};
