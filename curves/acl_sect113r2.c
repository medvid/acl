#include "acl.h"

const uint acl_sect113r2_fr[] = { 113, 9, 0 };

const uint acl_sect113r2_g[] = {
    0xb8164797, 0x5ef52fcd, 0x6a7b26ca, 0x0001a57a,
    0x95baba1d, 0x674c06e6, 0xc94ed1fe, 0x0000b3ad
};

const uint acl_sect113r2_a[] = {
    0xc0aa55c7, 0x5a0dd6df, 0x18dbec7e, 0x00006899
};

const uint acl_sect113r2_b[] = {
    0xe059184f, 0x7bd4bf36, 0xa9ec9b29, 0x000095e9
};

const uint acl_sect113r2_o[] = {
    0x2496af93, 0x0108789b, 0x00000000, 0x00010000
};

const ecc_t acl_sect113r2 = {
    "sect113r2",
    ECC_2,
    4,
    (vect) 0,
    (list) acl_sect113r2_fr,
    (vect2) acl_sect113r2_g,
    (vect) acl_sect113r2_a,
    (vect) acl_sect113r2_b,
    (vect) acl_sect113r2_o,
    4,
    2,
    (void *) &acl_2_ecc_func
};
