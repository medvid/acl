#include "acl.h"

const uint acl_sect131r1_fr[] = { 131, 8, 3, 2, 0 };

const uint acl_sect131r1_g[] = {
    0x43638399, 0x0f9c1813, 0xdf9833c4, 0x81baf91f,0x00000000,
    0x4ef9e150, 0xc8134b1b, 0x8c001f73, 0x8c6e7ea3,0x00000007
};

const uint acl_sect131r1_a[] = {
    0x8c2570b8, 0x418ff3ff, 0x6b562144, 0xa11b09a7,0x00000007
};

const uint acl_sect131r1_b[] = {
    0x78f9d341, 0xc6c72916, 0x884b63b9, 0x17c05610,0x00000002
};

const uint acl_sect131r1_o[] = {
    0x9464b54d, 0x3123953a, 0x00000002, 0x00000000,0x00000004
};

const ecc_t acl_sect131r1 = {
    "sect131r1",
    ECC_2,
    5,
    (vect) 0,
    (list) acl_sect131r1_fr,
    (vect2) acl_sect131r1_g,
    (vect) acl_sect131r1_a,
    (vect) acl_sect131r1_b,
    (vect) acl_sect131r1_o,
    5,
    2,
    (void *) &acl_2_ecc_func
};
