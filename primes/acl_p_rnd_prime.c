// returns in res a random probable prime of lengthlen
// runs the rabin-miller test k-times
// sets the msb and lsb; also sets bit "also_set";
// if you don't want to set any bit other than msb and lsb, set "also_set"
// to zero - sets the lsb again
// len is the length of res in 32-bit words; tmp is(7 x len) ints big,
// used for temporary storage

#include "acl.h"
#include "acl_config.h"

void acl_p_rnd_prime(vect res, vect7 tmp, uint k, uint also_set, \
                     prng rnd_fast, prng rnd_strong, size_t len)
{
    uint m_inv, cnt; vect ptr, tmp1, tmp2, r_mod_m,r2_mod_m, aa;

    // tmp tmp tmp1 tmp2 r_mod_m r2_mod_m aa
    tmp1 = tmp + 2*len; tmp2 = tmp1 + len; r_mod_m = tmp2 + len;
    r2_mod_m = r_mod_m + len; aa = r2_mod_m + len;

    rnd_strong(res, len);           // generate random number
    acl_bit_set(res, 0);            // make sure number is odd
    while (1) {
        acl_p_mod_add32(res, res, 2, 0, len); // increment candidate number
        acl_bit_set(res, 32*len-1); // make sure number is full-length
        acl_bit_set(res, also_set); // allow user to set arbitrary bit
        if (len <= ACL_POP_SIZE) {
            ptr = (vect) ((uint) &acl_pop_table + 2*len*(len-1));
        } else {
            ptr = (vect) ((uint) &acl_pop_table \
                                 + 2*ACL_POP_SIZE*(ACL_POP_SIZE-1));
            acl_mov32(tmp, 0, len);
            acl_mov(tmp, ptr, ACL_POP_SIZE);
            ptr = tmp;
        }
        if (acl_p_coprime(res, ptr, tmp1, len)) {
            acl_p_mont_pre(r_mod_m, r2_mod_m, &m_inv, res, len);
            if (acl_p_rm_test2(res, tmp, m_inv, r_mod_m, len)) {
                cnt = k;
                do {
                    if (cnt-- == 0) return;
                    rnd_fast(tmp, len);
                    acl_p_mod(aa, tmp, len, res, len);
                } while (acl_p_rm_test(aa, res, tmp,m_inv, \
                                      r_mod_m, r2_mod_m, len));
            }
        }
    }
}
