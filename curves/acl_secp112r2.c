#include "acl.h"

const uint acl_secp112r2_m[] = {
    0xfffffffd, 0xffffffff, 0xffffffff, 0xffffffff,
    0xbead208b, 0x5e668076, 0x2abf62e3, 0x0000db7c
};

const uint acl_secp112r2_fr[] = { 128, 1, 0 };

const uint acl_secp112r2_g[] = {
    0xd0928643, 0xb4e1649d, 0x0ab5e892, 0x00004ba3,
    0x6e956e97, 0x3747def3, 0x46f5882e, 0x0000adcd
};

const uint acl_secp112r2_a[] = {
    0x5c0ef02c, 0x8a0aaaf6, 0xc24c05f3, 0x00006127
};

const uint acl_secp112r2_b[] = {
    0x4c85d709, 0xed74fcc3, 0xf1815db5, 0x000051de
};

const uint acl_secp112r2_o[] = {
    0x0520d04b, 0xd7597ca1, 0x0aafd8b8, 0x000036df
};

const ecc_t acl_secp112r2 = {
    "secp112r2",
    ECC_P + ECC_A,
    4,
    (vect) acl_secp112r2_m,
    (list) acl_secp112r2_fr,
    (vect2) acl_secp112r2_g,
    (vect) acl_secp112r2_a,
    (vect) acl_secp112r2_b,
    (vect) acl_secp112r2_o,
    4,
    1,
    (void *) &acl_p_ecc_func
};
