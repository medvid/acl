#include "acl.h"

const uint acl_sect283k1_fr[] = { 283, 12, 7, 5, 0 };

const uint acl_sect283k1_g[] = {
    0x58492836, 0xb0c2ac24, 0x16876913, 0x23c1567a,0x53cd265f, 0x62f188e5,
    0x3f1a3b81, 0x78ca4488, 0x0503213f,
    0x77dd2259, 0x4e341161, 0xe4596236, 0xe8184698,0xe87e45c0, 0x07e5426f,
    0x8d90f95d, 0x0f1c9e31, 0x01ccda38
};

const uint acl_sect283k1_o[] = {
    0x1e163c61, 0x94451e06, 0x265dff7f, 0x2ed07577,0xffffe9ae, 0xffffffff,
    0xffffffff, 0xffffffff, 0x01ffffff
};

const ecc_t acl_sect283k1 = {
    "sect283k1",
    ECC_2 + ECC_K,
    9,
    (vect) 0,
    (list) acl_sect283k1_fr,
    (vect2) acl_sect283k1_g,
    (vect) 0,
    (vect) 1,
    (vect) acl_sect283k1_o,
    9,
    4,
    (void *) &acl_2_ecc_func
};
