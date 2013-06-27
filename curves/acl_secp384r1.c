#include "acl.h"

const uint acl_secp384r1_m[] = {
    0xffffffff, 0x00000000, 0x00000000, 0xffffffff,0xfffffffe, 0xffffffff,
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,0xffffffff, 0xffffffff
};

const uint acl_secp384r1_fr[] = { 384, 128, 96, ~32, 0 };

const uint acl_secp384r1_g[] = {
    0x72760ab7, 0x3a545e38, 0xbf55296c, 0x5502f25d,0x82542a38, 0x59f741e0,
    0x8ba79b98, 0x6e1d3b62, 0xf320ad74, 0x8eb1c71e,0xbe8b0537, 0xaa87ca22,
    0x90ea0e5f, 0x7a431d7c, 0x1d7e819d, 0x0a60b1ce,0xb5f0b8c0, 0xe9da3113,
    0x289a147c, 0xf8f41dbd, 0x9292dc29, 0x5d9e98bf,0x96262c6f, 0x3617de4a
};

const uint acl_secp384r1_b[] = {
    0xd3ec2aef, 0x2a85c8ed, 0x8a2ed19d, 0xc656398d,0x5013875a, 0x0314088f,
    0xfe814112, 0x181d9c6e, 0xe3f82d19, 0x988e056b,0xe23ee7e4, 0xb3312fa7
};

const uint acl_secp384r1_o[] = {
    0xccc52973, 0xecec196a, 0x48b0a77a, 0x581a0db2,0xf4372ddf, 0xc7634d81,
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,0xffffffff, 0xffffffff
};

const ecc_t acl_secp384r1 = {
    "secp384r1",
    ECC_P,
    12,
    (vect) acl_secp384r1_m,
    (list) acl_secp384r1_fr,
    (vect2) acl_secp384r1_g,
    (vect) -3,
    (vect) acl_secp384r1_b,
    (vect) acl_secp384r1_o,
    12,
    1,
    (void *) &acl_p_ecc_func
};
