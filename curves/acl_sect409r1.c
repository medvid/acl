#include "acl.h"

const uint acl_sect409r1_fr[] = { 409, 87, 0 };

const uint acl_sect409r1_g[] = {
    0xbb7996a7, 0x60794e54, 0x5603aeab, 0x8a118051,0xdc255a86, 0x34e59703,
    0xb01ffe5b, 0xf1771d4d, 0x441cde4a, 0x64756260,0x496b0c60, 0xd088ddb3,
    0x015d4860,
    0x0273c706, 0x81c364ba, 0xd2181b36, 0xdf4b4f40,0x38514f1f, 0x5488d08f,
    0x0158aa4f, 0xa7bd198d, 0x7636b9c5, 0x24ed106a,0x2bbfa783, 0xab6be5f3,
    0x0061b1cf
};

const uint acl_sect409r1_b[] = {
    0x7b13545f, 0x4f50ae31, 0xd57a55aa, 0x72822f6c,0xa9a197b2, 0xd6ac27c8,
    0x4761fa99, 0xf1f3dd67, 0x7fd6422e, 0x3b7b476b,0x5c4b9a75, 0xc8ee9feb,
    0x0021a5c2
};

const uint acl_sect409r1_o[] = {
    0xd9a21173, 0x8164cd37, 0x9e052f83, 0x5fa47c3c,0xf33307be, 0xaad6a612,
    0x000001e2, 0x00000000, 0x00000000, 0x00000000,0x00000000, 0x00000000,
    0x01000000
};

const ecc_t acl_sect409r1 = {
    "sect409r1",
    ECC_2,
    13,
    (vect) 0,
    (list) acl_sect409r1_fr,
    (vect2) acl_sect409r1_g,
    (vect) 1,
    (vect) acl_sect409r1_b,
    (vect) acl_sect409r1_o,
    13,
    2,
    (void *) &acl_2_ecc_func
};
