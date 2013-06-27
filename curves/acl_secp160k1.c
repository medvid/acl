#include "acl.h"

const uint acl_secp160k1_m[] = {
    0xffffac73, 0xfffffffe, 0xffffffff, 0xffffffff,0xffffffff
};

const uint acl_secp160k1_fr[] = { 160, 32, 0x538d };

const uint acl_secp160k1_g[] = {
    0xdd4d7ebb, 0x3036f4f5, 0xa4019e76, 0xe37aa192,0x3b4c382c,
    0xf03c4fee, 0x531733c3, 0x6bc28286, 0x318fdced,0x938cf935
};

const uint acl_secp160k1_o[] = {
    0xca16b6b3, 0x16dfab9a, 0x0001b8fa, 0x00000000,0x00000000, 0x00000001
};

const ecc_t acl_secp160k1 = {
    "secp160k1",
    ECC_P + ECC_K,
    5,
    (vect) acl_secp160k1_m,
    (list) acl_secp160k1_fr,
    (vect2) acl_secp160k1_g,
    (vect) 0,
    (vect) 7,
    (vect) acl_secp160k1_o,
    6,
    1,
    (void *) &acl_p_ecc_func
};
