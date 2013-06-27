#include "acl.h"

const uint acl_sect409k1_fr[] = { 409, 87, 0 };

const uint acl_sect409k1_g[] = {
    0xe9023746, 0xb35540cf, 0xee222eb1, 0xb5aaaa62,0xc460189e, 0xf9f67cc2,
    0x27accfb8, 0xe307c84c, 0x0efd0987, 0x0f718421,0xad3ab189, 0x658f49c1,
    0x0060f05f,
    0xd8e0286b, 0x5863ec48, 0xaa9ca27a, 0xe9c55215,0xda5f6c42, 0xe9ea10e3,
    0xe6325165, 0x918ea427, 0x3460782f, 0xbf04299c,0xacba1dac, 0x0b7c4e42,
    0x01e36905
};

const uint acl_sect409k1_o[] = {
    0xe01e5fcf, 0x4b5c83b8, 0xe3e7ca5b, 0x557d5ed3,0x20400ec4, 0x83b2d4ea,
    0xfffffe5f, 0xffffffff, 0xffffffff, 0xffffffff,0xffffffff, 0xffffffff,
    0x007fffff
};

const ecc_t acl_sect409k1 = {
    "sect409k1",
    ECC_2 + ECC_K,
    13,
    (vect) 0,
    (list) acl_sect409k1_fr,
    (vect2) acl_sect409k1_g,
    (vect) 0,
    (vect) 1,
    (vect) acl_sect409k1_o,
    13,
    4,
    (void *) &acl_2_ecc_func
};
