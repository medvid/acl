#include "acl.h"

const uint acl_sect163k1_fr[] = { 163, 7, 6, 3, 0 };

const uint acl_sect163k1_g[] = {
    0x5c94eee8, 0xde4e6d5e, 0xaa07d793, 0x7bbc11ac,0xfe13c053, 0x00000002,
    0xccdaa3d9, 0x0536d538, 0x321f2e80, 0x5d38ff58,0x89070fb0, 0x00000002
};

const uint acl_sect163k1_o[] = {
    0x99f8a5ef, 0xa2e0cc0d, 0x00020108, 0x00000000,0x00000000, 0x00000004
};

const ecc_t acl_sect163k1 = {
    "sect163k1",
    ECC_2 + ECC_K,
    6,
    (vect) 0,
    (list) acl_sect163k1_fr,
    (vect2) acl_sect163k1_g,
    (vect) 1,
    (vect) 1,
    (vect) acl_sect163k1_o,
    6,
    2,
    (void *) &acl_2_ecc_func
};
