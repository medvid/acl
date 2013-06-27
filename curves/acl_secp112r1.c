#include "acl.h"

const uint acl_secp112r1_m[] = {
    0xfffffffd, 0xffffffff, 0xffffffff, 0xffffffff,
    0xbead208b, 0x5e668076, 0x2abf62e3, 0x0000db7c
};

const uint acl_secp112r1_fr[] = { 128, 1, 0 };

const uint acl_secp112r1_g[] = {
    0xf9c2f098, 0x5ee76b55, 0x7239995a, 0x00000948,
    0x0ff77500, 0xc0a23e0e, 0xe5af8724, 0x0000a89c
};

const uint acl_secp112r1_b[] = {
    0x11702b22, 0x16eede89, 0xf8ba0439, 0x0000659e
};

const uint acl_secp112r1_o[] = {
    0xac6561c5, 0x5e7628df, 0x2abf62e3, 0x0000db7c
};

const ecc_t acl_secp112r1 = {
    "secp112r1",
    ECC_P + ECC_A,
    4,
    (vect) acl_secp112r1_m,
    (list) acl_secp112r1_fr,
    (vect2) acl_secp112r1_g,
    (vect) -3,
    (vect) acl_secp112r1_b,
    (vect) acl_secp112r1_o,
    4,
    1,
    (void *) &acl_p_ecc_func
};
