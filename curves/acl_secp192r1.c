#include "acl.h"

const uint acl_secp192r1_m[] = {
    0xffffffff, 0xffffffff, 0xfffffffe, 0xffffffff,0xffffffff, 0xffffffff
};

const uint acl_secp192r1_fr[] = { 192, 64, 0 };

const uint acl_secp192r1_g[] = {
    0x82ff1012, 0xf4ff0afd, 0x43a18800, 0x7cbf20eb,0xb03090f6, 0x188da80e,
    0x1e794811, 0x73f977a1, 0x6b24cdd5, 0x631011ed,0xffc8da78, 0x07192b95
};

const uint acl_secp192r1_b[] = {
    0xc146b9b1, 0xfeb8deec, 0x72243049, 0x0fa7e9ab,0xe59c80e7, 0x64210519
};

const uint acl_secp192r1_o[] = {
    0xb4d22831, 0x146bc9b1, 0x99def836, 0xffffffff,0xffffffff, 0xffffffff
};

const ecc_t acl_secp192r1 = {
    "secp192r1",
    ECC_P,
    6,
    (vect) acl_secp192r1_m,
    (list) acl_secp192r1_fr,
    (vect2) acl_secp192r1_g,
    (vect) -3,
    (vect) acl_secp192r1_b,
    (vect) acl_secp192r1_o,
    6,
    1,
    (void *) &acl_p_ecc_func
};
