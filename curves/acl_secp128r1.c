#include "acl.h"

const uint acl_secp128r1_m[] = {
    0xffffffff, 0xffffffff, 0xffffffff, 0xfffffffd
};

const uint acl_secp128r1_fr[] = { 128, 97, 0 };

const uint acl_secp128r1_g[] = {
    0xa52c5b86, 0x0c28607c, 0x8b899b2d, 0x161ff752,
    0xdded7a83, 0xc02da292, 0x5bafeb13, 0xcf5ac839
};

const uint acl_secp128r1_b[] = {
    0x2cee5ed3, 0xd824993c, 0x1079f43d, 0xe87579c1
};

const uint acl_secp128r1_o[] = {
    0x9038a115, 0x75a30d1b, 0x00000000, 0xfffffffe
};

const ecc_t acl_secp128r1 = {
    "secp128r1",
    ECC_P,
    4,
    (vect) acl_secp128r1_m,
    (list) acl_secp128r1_fr,
    (vect2) acl_secp128r1_g,
    (vect) -3,
    (vect) acl_secp128r1_b,
    (vect) acl_secp128r1_o,
    4,
    1,
    (void *) &acl_p_ecc_func
};
