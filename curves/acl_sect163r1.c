#include "acl.h"

const uint acl_sect163r1_fr[] = { 163, 7, 6, 3, 0 };

const uint acl_sect163r1_g[] = {
    0x7876a654, 0x567f787a, 0x89566789, 0xab438977,0x69979697, 0x00000003,
    0xf41ff883, 0xe3c80988, 0x9d51fefc, 0xefafb298,0x435edb42, 0x00000000
};

const uint acl_sect163r1_a[] = {
    0xd2782ae2, 0xbd88e246, 0x54ff8428, 0xefa84f95,0xb6882caa, 0x00000007
};

const uint acl_sect163r1_b[] = {
    0xf958afd9, 0xca91f73a, 0x946bda29, 0xdcb40aab,0x13612dcd, 0x00000007
};

const uint acl_sect163r1_o[] = {
    0xa710279b, 0xb689c29c, 0xffff48aa, 0xffffffff,0xffffffff, 0x00000003
};

const ecc_t acl_sect163r1 = {
    "sect163r1",
    ECC_2,
    6,
    (vect) 0,
    (list) acl_sect163r1_fr,
    (vect2) acl_sect163r1_g,
    (vect) acl_sect163r1_a,
    (vect) acl_sect163r1_b,
    (vect) acl_sect163r1_o,
    6,
    2,
    (void *) &acl_2_ecc_func
};
