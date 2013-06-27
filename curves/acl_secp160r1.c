#include "acl.h"

const uint acl_secp160r1_m[] = {
    0x7fffffff, 0xffffffff, 0xffffffff, 0xffffffff,0xffffffff
};

const uint acl_secp160r1_fr[] = { 160, 31, 0 };

const uint acl_secp160r1_g[] = {
    0x13cbfc82, 0x68c38bb9, 0x46646989, 0x8ef57328,0x4a96b568,
    0x7ac5fb32, 0x04235137, 0x59dcc912, 0x3168947d,0x23a62855
};

const uint acl_secp160r1_b[] = {
    0xc565fa45, 0x81d4d4ad, 0x65acf89f, 0x54bd7a8b,0x1c97befc
};

const uint acl_secp160r1_o[] = {
    0xca752257, 0xf927aed3, 0x0001f4c8, 0x00000000,0x00000000, 0x00000001
};

const ecc_t acl_secp160r1 = {
    "secp160r1",
    ECC_P,
    5,
    (vect) acl_secp160r1_m,
    (list) acl_secp160r1_fr,
    (vect2) acl_secp160r1_g,
    (vect) -3,
    (vect) acl_secp160r1_b,
    (vect) acl_secp160r1_o,
    6,
    1,
    (void *) &acl_p_ecc_func
};
