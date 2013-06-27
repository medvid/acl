#include "acl.h"

const uint acl_sect239k1_fr[] = { 239, 158, 0 };

const uint acl_sect239k1_g[] = {
    0x193035dc, 0x7b2a6555, 0xc44cc2cc, 0xa8b2d126,0x88a68727, 0x83e97309,
    0xb6a887a9, 0x000029a0,
    0x6553f0ca, 0x2a5dc6b7, 0xb275fc31, 0xe73510ac,0x1c103089, 0x549bdb01,
    0x0804f12e, 0x00007631
};

const uint acl_sect239k1_o[] = {
    0x00e478a5, 0x1f1c1da8, 0xc67cb6e9, 0x005a79fe,0x00000000, 0x00000000,
    0x00000000, 0x00002000
};

const ecc_t acl_sect239k1 = {
    "sect239k1",
    ECC_2 + ECC_K,
    8,
    (vect) 0,
    (list) acl_sect239k1_fr,
    (vect2) acl_sect239k1_g,
    (vect) 0,
    (vect) 1,
    (vect) acl_sect239k1_o,
    8,
    4,
    (void *) &acl_2_ecc_func
};
