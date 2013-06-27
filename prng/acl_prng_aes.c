#include "acl.h"
#include "acl_config.h"

static uint apa_key[ACL_PRNG_AES_SIZE];
static uint apa_key_exp[(ACL_PRNG_AES_SIZE + 7)*4];
static uint apa_cntr[4];
static uint apa_tmp[4];
static const uint apa_in[4] = {0, 0, 0, 0};

void acl_prng_aes_init(prng rnd)
{
    rnd(apa_key, ACL_PRNG_AES_SIZE);
    acl_aes_key_en(apa_key_exp, apa_key, ACL_PRNG_AES_SIZE);
    rnd(apa_cntr, 4);
}

void acl_prng_aes(vect res, size_t len)
{
    int i;

    for(i = 0; i < len; i++) {
        acl_aes_cntr(apa_tmp, (vect) apa_in, apa_key_exp, \
                ACL_PRNG_AES_SIZE, apa_cntr);
        res[i] = apa_tmp[0];
    }
}
