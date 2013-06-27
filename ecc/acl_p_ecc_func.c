#include "acl.h"

const ecc_func_t acl_p_ecc_func = {
    acl_p_ecc_chk,
    acl_p_ecc_dbl,
    acl_p_ecc_add,
    acl_p_ecc_aff,
    acl_p_ecc_p2str,
    acl_p_ecc_str2p
};
