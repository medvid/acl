#include "acl.h"
#include "acl_int.h"
#include "acl_config.h"

static uint apb_m[2*ACL_PRNG_BBS_SIZE];
static uint apb_tmp[4*ACL_PRNG_BBS_SIZE];
static uint apb_x[2*ACL_PRNG_BBS_SIZE];
#if ACL_PRNG_BBS_MONT == 0
static uint apb_y[2*ACL_PRNG_BBS_SIZE];
#endif
static uint apb_m_inv;

// m = product of two primes (each == 3 mod 4)
// x = random number coprime with m
// x = x * R (go into montgomery domain)
// note that vect7 here means 7*ACL_PRNG_BBS_SIZE

void acl_prng_bbs_init(prng rnd_fast, prng rnd, vect7 tmp)
{
    do {
        acl_p_rnd_prime(apb_x, tmp, ACL_PRNG_BBS_K,1, \
                rnd_fast, rnd, ACL_PRNG_BBS_SIZE);
        acl_p_rnd_prime(apb_x + ACL_PRNG_BBS_SIZE, tmp, ACL_PRNG_BBS_K, 1, \
                rnd_fast, rnd, ACL_PRNG_BBS_SIZE);
    } while (!acl_cmp(apb_x, apb_x + ACL_PRNG_BBS_SIZE, ACL_PRNG_BBS_SIZE));
    acl_p_mul(apb_m, apb_x, apb_x + ACL_PRNG_BBS_SIZE, ACL_PRNG_BBS_SIZE);
    do {
        rnd(apb_x, 2*ACL_PRNG_BBS_SIZE);
    } while (!acl_p_coprime(apb_x, apb_m, tmp, 2*ACL_PRNG_BBS_SIZE));
    acl_p_mont_pre(0, apb_tmp, &apb_m_inv, apb_m, 2*ACL_PRNG_BBS_SIZE);
    acl_p_mul(tmp, apb_x, apb_tmp, 2*ACL_PRNG_BBS_SIZE);
    acl_p_mont_red(apb_x, tmp, apb_m, apb_m_inv, 2*ACL_PRNG_BBS_SIZE);
}

// for each bit: x = (x^2)/R
// note that the least significant bit can be taken from x^(2i) mod m (slower)
//   or from its montgomery representation x^(2i)*R mod m (faster)
// if multiplication by a non-zero number R modulo m is a one-to-one mapping
//   (ask a mathematician...) then this *should* be equivalent

void acl_prng_bbs(vect res, size_t len)
{
    int i;

    acl_mov32(res, 0, len);
    for (i = 0; i < 32*len; i++) {
        acl_p_sqr(apb_tmp, apb_x, 2*ACL_PRNG_BBS_SIZE);
        acl_p_mont_red(apb_x, apb_tmp, apb_m, apb_m_inv, 2*ACL_PRNG_BBS_SIZE);
#if ACL_PRNG_BBS_MONT == 0
        acl_mov(apb_tmp, apb_x, 2*ACL_PRNG_BBS_SIZE);
        acl_mov32(apb_tmp + 2*ACL_PRNG_BBS_SIZE, 0,2*ACL_PRNG_BBS_SIZE);
        acl_p_mont_red(apb_y, apb_tmp, apb_m, apb_m_inv, 2*ACL_PRNG_BBS_SIZE);
        if (apb_y[0] & 1) {
            acl_bit_set(res, i);
        }
#else
        if(apb_x[0] & 1) {
            acl_bit_set(res, i);
        }
#endif
    }
}
