#ifndef TEST_H
#define TEST_H

#include "acl.h"

bool_t test_aes(void);
bool_t test_sha(void);
bool_t test_rsa(void);
bool_t test_ecc(void);
bool_t test_ecdsa(void);

#define TEST_AES_ITER           10000
#define TEST_ECC_ITER           100
#define TEST_ECDSA_ITER         100
#define TEST_RSA_ITER           100   // number of times to run the main loop
#define TEST_RSA_PRIME_ITER     1     // number of fermat tests to try for prime p
#define TEST_SHA_ITER           1000000


#endif
