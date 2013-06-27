#include "acl.h"

const uint acl_secp192k1_m[] = {
    0xffffee37, 0xfffffffe, 0xffffffff, 0xffffffff,0xffffffff, 0xffffffff
};

const uint acl_secp192k1_fr[] = { 192, 32, 0x11c9 };

const uint acl_secp192k1_g[] = {
    0xeae06c7d, 0x1da5d1b1, 0x80b7f434, 0x26b07d02,0xc057e9ae, 0xdb4ff10e,
    0xd95e2f9d, 0x4082aa88, 0x15be8634, 0x844163d0,0x9c5628a7, 0x9b2f2f6d
};

const uint acl_secp192k1_o[] = {
    0x74defd8d, 0x0f69466a, 0x26f2fc17, 0xfffffffe,0xffffffff, 0xffffffff
};

const ecc_t acl_secp192k1 = {
    "secp192k1",
    ECC_P + ECC_K,
    6,
    (vect) acl_secp192k1_m,
    (list) acl_secp192k1_fr,
    (vect2) acl_secp192k1_g,
    (vect) 0,
    (vect) 3,
    (vect) acl_secp192k1_o,
    6,
    1,
    (void *) &acl_p_ecc_func
};
