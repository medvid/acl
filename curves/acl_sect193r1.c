#include "acl.h"

const uint acl_sect193r1_fr[] = { 193, 15, 0 };

const uint acl_sect193r1_g[] = {
    0xd8c0c5e1, 0x79625372, 0xdef4bf61, 0xad6cdf6f,0x0ff84a74, 0xf481bc5f,
    0x00000001,
    0xf7ce1b05, 0xb3201b6a, 0x1ad17fb0, 0xf3ea9e3a,0x903712cc, 0x25e399f2,
    0x00000000
};

const uint acl_sect193r1_a[] = {
    0x11df7b01, 0x098ac8a9, 0x7b4087de, 0x69e171f7,0x7a989751, 0x17858feb,
    0x00000000
};

const uint acl_sect193r1_b[] = {
    0x31478814, 0xc1c2e5d8, 0x1e5bbc7c, 0xacadaa7a,0xe6c3a89f, 0xfdfb49bf,
    0x00000000
};

const uint acl_sect193r1_o[] = {
    0x920eba49, 0x8f443acc, 0xc7f34a77, 0x00000000,0x00000000, 0x00000000,
    0x00000001
};

const ecc_t acl_sect193r1 = {
    "sect193r1",
    ECC_2,
    7,
    (vect) 0,
    (list) acl_sect193r1_fr,
    (vect2) acl_sect193r1_g,
    (vect) acl_sect193r1_a,
    (vect) acl_sect193r1_b,
    (vect) acl_sect193r1_o,
    7,
    2,
    (void *) &acl_2_ecc_func
};
