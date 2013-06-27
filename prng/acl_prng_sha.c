#include "acl.h"

static uint aps_state[23];
static uint aps_cntr[5];

void acl_prng_sha_init(prng rnd)
{
    rnd(aps_cntr, 5);
}

void acl_prng_sha(vect res, size_t len)
{
    int i, j;

    for(i = 0; i < len; i++) {
        acl_sha1_init(aps_state);
        for(j = 0; j < 20; j++) {
            acl_sha1(aps_state, ((byte *) aps_cntr)[j]);
        }
        acl_sha1_done(aps_state);
        res[i] = aps_state[0];
        acl_p_mod_add32(aps_cntr, aps_cntr, 1, 0, 5);
    }
}
