#include "acl.h"

const uint acl_sect131r2_fr[] = { 131, 8, 3, 2, 0 };

const uint acl_sect131r2_g[] = {
    0x1bb366a8, 0x652d2395, 0xf95031ad, 0x56dcd8f2,0x00000003,
    0xe9eb240f, 0x6d9e265d, 0x7940a536, 0x48f06d86,0x00000006
};

const uint acl_sect131r2_a[] = {
    0x176573b2, 0x415f07c2, 0xd7cafcbf, 0xe5a88919,0x00000003
};

const uint acl_sect131r2_b[] = {
    0x018f2192, 0x734ce38f, 0xc55657ac, 0xb8266a46,0x00000004
};

const uint acl_sect131r2_o[] = {
    0x049ba98f, 0x6954a233, 0x00000001, 0x00000000,0x00000004
};

const ecc_t acl_sect131r2 = {
    "sect131r2",
    ECC_2,
    5,
    (vect) 0,
    (list) acl_sect131r2_fr,
    (vect2) acl_sect131r2_g,
    (vect) acl_sect131r2_a,
    (vect) acl_sect131r2_b,
    (vect) acl_sect131r2_o,
    5,
    2,
    (void *) &acl_2_ecc_func
};
