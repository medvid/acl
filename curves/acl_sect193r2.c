#include "acl.h"

const uint acl_sect193r2_fr[] = { 193, 15, 0 };

const uint acl_sect193r2_g[] = {
    0xae617e8f, 0xa651350a, 0x7e82ca14, 0x03f39e1a,0x2e0367c8, 0xd9b67d19,
    0x00000000,
    0x4cdecf6c, 0x96f92722, 0xd9ca01f5, 0x29e7defb,0x07c304ac, 0xce943356,
    0x00000001
};

const uint acl_sect193r2_a[] = {
    0x7702709b, 0x3ecd6997, 0x190b0bc4, 0xa6ed8667,0x37c2ce3e, 0x63f35a51,
    0x00000001
};

const uint acl_sect193r2_b[] = {
    0x1d4316ae, 0xe3efb7f6, 0x856a5b16, 0x377e2ab2,0x27d4d64c, 0xc9bb9e89,
    0x00000000
};

const uint acl_sect193r2_o[] = {
    0xd4ee99d5, 0x005413cc, 0x5aab561b, 0x00000001,0x00000000, 0x00000000,
    0x00000001
};

const ecc_t acl_sect193r2 = {
    "sect193r2",
    ECC_2,
    7,
    (vect) 0,
    (list) acl_sect193r2_fr,
    (vect2) acl_sect193r2_g,
    (vect) acl_sect193r2_a,
    (vect) acl_sect193r2_b,
    (vect) acl_sect193r2_o,
    7,
    2,
    (void *) &acl_2_ecc_func
};
