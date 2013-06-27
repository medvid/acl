#include "acl.h"

const uint acl_sect283r1_fr[] = { 283, 12, 7, 5, 0 };

const uint acl_sect283r1_g[] = {
    0x86b12053, 0xf8cdbecd, 0x80e2e198, 0x557eac9c,0x2eed25b8, 0x70b0dfec,
    0xe1934f8c, 0x8db7dd90, 0x05f93925,
    0xbe8112f4, 0x13f0df45, 0x826779c8, 0x350eddb0,0x516ff702, 0xb20d02b4,
    0xb98fe6d4, 0xfe24141c, 0x03676854
};

const uint acl_sect283r1_b[] = {
    0x3b79a2f5, 0xf6263e31, 0xa581485a, 0x45309fa2,0xca97fd76, 0x19a0303f,
    0xa5a4af8a, 0xc8b8596d, 0x027b680a
};

const uint acl_sect283r1_o[] = {
    0xefadb307, 0x5b042a7c, 0x938a9016, 0x399660fc,0xffffef90, 0xffffffff,
    0xffffffff, 0xffffffff, 0x03ffffff
};

const ecc_t acl_sect283r1 = {
    "sect283r1",
    ECC_2,
    9,
    (vect) 0,
    (list) acl_sect283r1_fr,
    (vect2) acl_sect283r1_g,
    (vect) 1,
    (vect) acl_sect283r1_b,
    (vect) acl_sect283r1_o,
    9,
    2,
    (void *) &acl_2_ecc_func
};
