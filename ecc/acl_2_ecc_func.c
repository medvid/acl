#include "acl.h"

const ecc_func_t acl_2_ecc_func = {
    acl_2_ecc_chk,
    acl_2_ecc_dbl,
    acl_2_ecc_add,
    acl_2_ecc_aff,
    acl_2_ecc_p2str,
    acl_2_ecc_str2p
};
