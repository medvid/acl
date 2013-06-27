#include "acl.h"

const uint acl_sect233r1_fr[] = { 233, 74, 0 };

const uint acl_sect233r1_g[] = {
    0x71fd558b, 0xf8f8eb73, 0x391f8b36, 0x5fef65bc,0x39f1bb75, 0x8313bb21,
    0xc9dfcbac, 0x000000fa,
    0x01f81052, 0x36716f7e, 0xf867a7ca, 0xbf8a0bef,0xe58528be, 0x03350678,
    0x6a08a419, 0x00000100
};

const uint acl_sect233r1_b[] = {
    0x7d8f90ad, 0x81fe115f, 0x20e9ce42, 0x213b333b,0x0923bb58, 0x332c7f8c,
    0x647ede6c, 0x00000066
};

const uint acl_sect233r1_o[] = {
    0x03cfe0d7, 0x22031d26, 0xe72f8a69, 0x0013e974,0x00000000, 0x00000000,
    0x00000000, 0x00000100
};

const ecc_t acl_sect233r1 = {
    "sect233r1",
    ECC_2,
    8,
    (vect) 0,
    (list) acl_sect233r1_fr,
    (vect2) acl_sect233r1_g,
    (vect) 1,
    (vect) acl_sect233r1_b,
    (vect) acl_sect233r1_o,
    8,
    2,
    (void *) &acl_2_ecc_func
};
