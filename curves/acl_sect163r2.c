#include "acl.h"

const uint acl_sect163r2_fr[] = { 163, 7, 6, 3, 0 };

const uint acl_sect163r2_g[] = {
    0xe8343e36, 0xd4994637, 0xa0991168, 0x86a2d57e,0xf0eba162, 0x00000003,
    0x797324f1, 0xb11c5c0c, 0xa2cdd545, 0x71a0094f,0xd51fbc6c, 0x00000000
};

const uint acl_sect163r2_b[] = {
    0x4a3205fd, 0x512f7874, 0x1481eb10, 0xb8c953ca,0x0a601907, 0x00000002,
};

const uint acl_sect163r2_o[] = {
    0xa4234c33, 0x77e70c12, 0x000292fe, 0x00000000,0x00000000, 0x00000004
};

const ecc_t acl_sect163r2 = {
    "sect163r2",
    ECC_2,
    6,
    (vect) 0,
    (list) acl_sect163r2_fr,
    (vect2) acl_sect163r2_g,
    (vect) 1,
    (vect) acl_sect163r2_b,
    (vect) acl_sect163r2_o,
    6,
    2,
    (void *) &acl_2_ecc_func
};
