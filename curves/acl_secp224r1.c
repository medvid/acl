#include "acl.h"

const uint acl_secp224r1_m[] = {
    0x00000001, 0x00000000, 0x00000000, 0xffffffff,0xffffffff, 0xffffffff,
    0xffffffff
};

const uint acl_secp224r1_fr[] = { 224, 96, ~0 };

const uint acl_secp224r1_g[] = {
    0x115c1d21, 0x343280d6, 0x56c21122, 0x4a03c1d3,0x321390b9, 0x6bb4bf7f,
    0xb70e0cbd,
    0x85007e34, 0x44d58199, 0x5a074764, 0xcd4375a0,0x4c22dfe6, 0xb5f723fb,
    0xbd376388
};

const uint acl_secp224r1_b[] = {
    0x2355ffb4, 0x270b3943, 0xd7bfd8ba, 0x5044b0b7,0xf5413256, 0x0c04b3ab,
    0xb4050a85
};

const uint acl_secp224r1_o[] = {
    0x5c5c2a3d, 0x13dd2945, 0xe0b8f03e, 0xffff16a2,0xffffffff, 0xffffffff,
    0xffffffff
};

const ecc_t acl_secp224r1 = {
    "secp224r1",
    ECC_P,
    7,
    (vect) acl_secp224r1_m,
    (list) acl_secp224r1_fr,
    (vect2) acl_secp224r1_g,
    (vect) -3,
    (vect) acl_secp224r1_b,
    (vect) acl_secp224r1_o,
    7,
    1,
    (void *) &acl_p_ecc_func
};
