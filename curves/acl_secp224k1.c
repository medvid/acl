#include "acl.h"

const uint acl_secp224k1_m[] = {
    0xffffe56d, 0xfffffffe, 0xffffffff, 0xffffffff,0xffffffff, 0xffffffff,
    0xffffffff
};

const uint acl_secp224k1_fr[] = { 224, 32, 0x1a93};

const uint acl_secp224k1_g[] = {
    0xb6b7a45c, 0x0f7e650e, 0xe47075a9, 0x69a467e9,0x30fc28a1, 0x4df099df,
    0xa1455b33,
    0x556d61a5, 0xe2ca4bdb, 0xc0b0bd59, 0xf7e319f7,0x82cafbd6, 0x7fba3442,
    0x7e089fed
};

const uint acl_secp224k1_o[] = {
    0x769fb1f7, 0xcaf0a971, 0xd2ec6184, 0x0001dce8,0x00000000, 0x00000000,
    0x00000000, 0x00000001
};

const ecc_t acl_secp224k1 = {
    "secp224k1",
    ECC_P + ECC_K,
    7,
    (vect) acl_secp224k1_m,
    (list) acl_secp224k1_fr,
    (vect2) acl_secp224k1_g,
    (vect) 0,
    (vect) 5,
    (vect) acl_secp224k1_o,
    8,
    1,
    (void *) &acl_p_ecc_func
};
