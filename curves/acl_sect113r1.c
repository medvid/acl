#include "acl.h"

const uint acl_sect113r1_fr[] = { 113, 9, 0 };

const uint acl_sect113r1_g[] = {
    0x3562c10f, 0xab1407d7, 0x616f35f4, 0x00009d73,
    0x5ed31886, 0xee84d131, 0x30277958, 0x0000a528
};

const uint acl_sect113r1_a[] = {
    0xe85820f7, 0xc7fe649c, 0x250ca6e7, 0x00003088
};

const uint acl_sect113r1_b[] = {
    0xe0e9c723, 0x0744188b, 0xe4d3e226, 0x0000e8be
};

const uint acl_sect113r1_o[] = {
    0x8a39e56f, 0x00d9ccec, 0x00000000, 0x00010000
};

const ecc_t acl_sect113r1 = {
    "sect113r1",
    ECC_2,
    4,
    (vect) 0,
    (list) acl_sect113r1_fr,
    (vect2) acl_sect113r1_g,
    (vect) acl_sect113r1_a,
    (vect) acl_sect113r1_b,
    (vect) acl_sect113r1_o,
    4,
    2,
    (void *) &acl_2_ecc_func
};
