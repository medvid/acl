#include "acl.h"

const uint acl_secp128r2_m[] = {
    0xffffffff, 0xffffffff, 0xffffffff, 0xfffffffd
};

const uint acl_secp128r2_fr[] = { 128, 97, 0 };

const uint acl_secp128r2_g[] = {
    0xcdebc140, 0xe6fb32a7, 0x5e572983, 0x7b6aa5d8,
    0x5fc34b44, 0x7106fe80, 0x894d3aee, 0x27b6916a
};

const uint acl_secp128r2_a[] = {
    0xbff9aee1, 0xbf59cc9b, 0xd1b3bbfe, 0xd6031998
};

const uint acl_secp128r2_b[] = {
    0xbb6d8a5d, 0xdc2c6558, 0x80d02919, 0x5eeefca3
};

const uint acl_secp128r2_o[] = {
    0x0613b5a3, 0xbe002472, 0x7fffffff, 0x3fffffff
};

const ecc_t acl_secp128r2 = {
    "secp128r2",
    ECC_P,
    4,
    (vect) acl_secp128r2_m,
    (list) acl_secp128r2_fr,
    (vect2) acl_secp128r2_g,
    (vect) acl_secp128r2_a,
    (vect) acl_secp128r2_b,
    (vect) acl_secp128r2_o,
    4,
    4,
    (void *) &acl_p_ecc_func
};
