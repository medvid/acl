#include "acl.h"

const uint acl_sect233k1_fr[] = { 233, 74, 0 };

const uint acl_sect233k1_g[] = {
    0xefad6126, 0x0a4c9d6e, 0x19c26bf5, 0x149563a4,0x29f22ff4, 0x7e731af1,
    0x32ba853a, 0x00000172,
    0x56fae6a3, 0x56e0c110, 0xf18aeb9b, 0x27a8cd9b,0x555a67c4, 0x19b7f70f,
    0x537dece8, 0x000001db
};

const uint acl_sect233k1_o[] = {
    0xf173abdf, 0x6efb1ad5, 0xb915bcd4, 0x00069d5b,0x00000000, 0x00000000,
    0x00000000, 0x00000080
};

const ecc_t acl_sect233k1 = {
    "sect233k1",
    ECC_2 + ECC_K,
    8,
    (vect) 0,
    (list) acl_sect233k1_fr,
    (vect2) acl_sect233k1_g,
    (vect) 0,
    (vect) 1,
    (vect) acl_sect233k1_o,
    8,
    4,
    (void *) &acl_2_ecc_func
};
