#include "acl.h"

const uint acl_secp256r1_m[] = {
    0xffffffff, 0xffffffff, 0xffffffff, 0x00000000,0x00000000, 0x00000000,
    0x00000001, 0xffffffff
};

const uint acl_secp256r1_fr[] = { 256, 224, ~192, ~96, 0 };

const uint acl_secp256r1_g[] = {
    0xd898c296, 0xf4a13945, 0x2deb33a0, 0x77037d81,0x63a440f2, 0xf8bce6e5,
    0xe12c4247, 0x6b17d1f2,
    0x37bf51f5, 0xcbb64068, 0x6b315ece, 0x2bce3357,0x7c0f9e16, 0x8ee7eb4a,
    0xfe1a7f9b, 0x4fe342e2
};

const uint acl_secp256r1_b[] = {
    0x27d2604b, 0x3bce3c3e, 0xcc53b0f6, 0x651d06b0,0x769886bc, 0xb3ebbd55,
    0xaa3a93e7, 0x5ac635d8
};

const uint acl_secp256r1_o[] = {
    0xfc632551, 0xf3b9cac2, 0xa7179e84, 0xbce6faad,0xffffffff, 0xffffffff,
    0x00000000, 0xffffffff
};

const ecc_t acl_secp256r1 = {
    "secp256r1",
    ECC_P,
    8,
    (vect) acl_secp256r1_m,
    (list) acl_secp256r1_fr,
    (vect2) acl_secp256r1_g,
    (vect) -3,
    (vect) acl_secp256r1_b,
    (vect) acl_secp256r1_o,
    8,
    1,
    (void *) &acl_p_ecc_func
};
