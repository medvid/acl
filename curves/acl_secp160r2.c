#include "acl.h"

const uint acl_secp160r2_m[] = {
    0xffffac73, 0xfffffffe, 0xffffffff, 0xffffffff,0xffffffff
};

const uint acl_secp160r2_fr[] = { 160, 32, 0x538d };

const uint acl_secp160r2_g[] = {
    0x3144ce6d, 0x30f7199d, 0x1f4ff11b, 0x293a117e,0x52dcb034,
    0xa7d43f2e, 0xf9982cfe, 0xe071fa0d, 0xe331f296,0xfeaffef2
};

const uint acl_secp160r2_b[] = {
    0xf50388ba, 0x04664d5a, 0xab572749, 0xfb59eb8b,0xb4e134d3
};

const uint acl_secp160r2_o[] = {
    0xf3a1a16b, 0xe786a818, 0x0000351e, 0x00000000,0x00000000, 0x00000001
};

const ecc_t acl_secp160r2 = {
    "secp160r2",
    ECC_P,
    5,
    (vect) acl_secp160r2_m,
    (list) acl_secp160r2_fr,
    (vect2) acl_secp160r2_g,
    (vect) -3,
    (vect) acl_secp160r2_b,
    (vect) acl_secp160r2_o,
    6,
    1,
    (void *) &acl_p_ecc_func
};
