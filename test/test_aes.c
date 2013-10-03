// perform monte carlo tests of the aes implementation
// the files referenced here can be found for example at
//      www.gnu.org/software/gnu-crypto/vectors/
// the tables go all the way to 400 iterations, butwe only
// go to 2 for time's sake
// for more rigorous tests, change the number of iterations
// and the known answer

#include "test.h"
#include "stdio.h"
#include "timer.h"

uint key[8];
uint key_exp[60];  /* (nk+7)*4, biggest: nk = 8 foraes-256 */
uint pt[4];
uint ct[4];
uint oct[4];
uint iv[4];
uint tmp[4];
uint i, j, k, h, len_aes;

const char *aes_ecb_en_results[] = {
    "0AC15A9AFBB24D54AD99E987208272E2",
    "77BA00ED5412DFF27C8ED91F3C376172",
    "C737317FE0846F132B23C8C2A672CE22"
};

const char *aes_ecb_de_results[] = {
    "E3FD51123B48A2E2AB1DB29894202222",
    "CC01684BE9B29ED01EA7923E7D2380AA",
    "15173A0EB65F5CC05E704EFE61D9E346"
};

const char *aes_cbc_en_results[] = {
    "983BF6F5A6DFBCDAA19370666E83A99A",
    "C6FB25A188CF7F3F24B07896C0C76D90",
    "81EA5BA46945C1705F6F89778868CC67"
};

const char *aes_cbc_de_results[] = {
    "F5372F9735C5685F1DA362AF6ECB2940",
    "F9604074F8FA45AC71959888DD056F9F",
    "D36C27EBB8FA0BC9FA368DF850FD45FB"
};

static void zero_all(void) {
    acl_mov32(key, 0, 8);
    acl_mov32(pt, 0, 4);
    acl_mov32(ct, 0, 4);
    acl_mov32(oct, 0, 4);
    acl_mov32(iv, 0, 4);
    restart_timer(0);
}

bool_t test_aes_ecb_en(void) {

    /* 128-bit ecb monte carlo encryption test */
    put_str("\naes ecb en 128");
    len_aes = ACL_128;
    zero_all();
    for (i = 0; i < 2; i++) {
        acl_aes_key_en(key_exp, key, len_aes);
        acl_mov(ct, pt, 4);
        for (j = 0; j < TEST_AES_ITER; j++) {
            acl_aes_ecb_en(ct, ct, key_exp, len_aes);
        }
        /* compare with ecb_e_m.txt 128 bits */
        acl_mov(pt, ct, 4);
        acl_xor(key, key, ct, 4);
    }
    h = stop_timer(0);
    put_val("  20000 = ", h);
    acl_str2bytes(tmp, (bytes) aes_ecb_en_results[0], 4);
    if (acl_cmp(pt, tmp, 4)) {
        return TRUE;
    }

    /* 192-bit ecb monte carlo encryption test */
    put_str("\naes ecb en 192");
    len_aes = ACL_192;
    zero_all();
    for (i = 0; i < 2; i++) {
        acl_aes_key_en(key_exp, key, len_aes);
        acl_mov(ct, pt, 4);
        for (j = 0; j < TEST_AES_ITER-2; j++) {
            acl_aes_ecb_en(ct,ct, key_exp, len_aes);
        }
        acl_aes_ecb_en(oct, ct, key_exp, len_aes);
        acl_aes_ecb_en(ct, oct, key_exp, len_aes);
        /* compare with ecb_e_m.txt 192 bits */
        acl_mov(pt, ct, 4);
        acl_xor(key, key, oct + 2, 2);
        acl_xor(key + 2, key + 2, ct, 4);
    }
    h = stop_timer(0);
    put_val("  20000 = ", h);
    acl_str2bytes(tmp, (bytes) aes_ecb_en_results[1], 4);
    if (acl_cmp(pt, tmp, 4)) {
        return TRUE;
    }

    /* 256-bit ecb monte carlo encryption test */
    put_str("\naes ecb en 256");
    len_aes = ACL_256;
    zero_all();
    for (i = 0; i < 2; i++) {
        acl_aes_key_en(key_exp, key, len_aes);
        acl_mov(ct, pt, 4);
        for (j = 0; j < TEST_AES_ITER-2; j++) {
            acl_aes_ecb_en(ct,ct, key_exp, len_aes);
        }
        acl_aes_ecb_en(oct, ct, key_exp, len_aes);
        acl_aes_ecb_en(ct, oct, key_exp, len_aes);
        /* compare with ecb_e_m.txt 256 bits */
        acl_mov(pt, ct, 4);
        acl_xor(key, key, oct, 4);
        acl_xor(key+4, key+4, ct, 4);
    }
    h = stop_timer(0);
    put_val("  20000 = ", h);
    acl_str2bytes(tmp, (bytes) aes_ecb_en_results[2], 4);
    if (acl_cmp(pt, tmp, 4)) {
        return TRUE;
    }

    return FALSE;
}

bool_t test_aes_ecb_de(void) {

    /* 128-bit ecb monte carlo decryption test */
    put_str("\naes ecb de 128");
    len_aes = ACL_128;
    zero_all();
    for (i = 0; i < 2; i++) {
        acl_aes_key_de(key_exp, key, len_aes);
        acl_mov(pt, ct, 4);
        for (j = 0; j < TEST_AES_ITER; j++) {
            acl_aes_ecb_de(pt, pt, key_exp, len_aes);
        }
        /* compare with ecb_d_m.txt 128 bits */
        acl_mov(ct, pt, 4);
        acl_xor(key, key, pt, 4);
    }
    h = stop_timer(0);
    put_val("  20000 = ", h);
    acl_str2bytes(tmp, (bytes) aes_ecb_de_results[0], 4);
    if (acl_cmp(ct, tmp, 4)) {
        return TRUE;
    }

    /* 192-bit ecb monte carlo decryption test */
    put_str("\naes ecb de 192");
    len_aes = ACL_192;
    zero_all();
    for (i = 0; i < 2; i++) {
        acl_aes_key_de(key_exp, key, len_aes);
        acl_mov(pt, ct, 4);
        for (j = 0; j < TEST_AES_ITER-2; j++) {
            acl_aes_ecb_de(pt,pt, key_exp, len_aes);
        }
        acl_aes_ecb_de(oct, pt, key_exp, len_aes);
        acl_aes_ecb_de(pt, oct, key_exp, len_aes);
        /* compare with ecb_d_m.txt 192 bits */
        acl_mov(ct, pt, 4);
        acl_xor(key, key, oct+2, 2);
        acl_xor(key+2, key+2, pt, 4);
    }
    h = stop_timer(0);
    put_val("  20000 = ", h);
    acl_str2bytes(tmp, (bytes) aes_ecb_de_results[1], 4);
    if (acl_cmp(ct, tmp, 4)) {
        return TRUE;
    }

    /* 256-bit ecb monte carlo decryption test */
    put_str("\naes ecb de 256");
    len_aes = ACL_256;
    zero_all();
    for (i = 0; i < 2; i++) {
        acl_aes_key_de(key_exp, key, len_aes);
        acl_mov(pt, ct, 4);
        for (j = 0; j < TEST_AES_ITER-2; j++) {
            acl_aes_ecb_de(pt,pt, key_exp, len_aes);
        }
        acl_aes_ecb_de(oct, pt, key_exp, len_aes);
        acl_aes_ecb_de(pt, oct, key_exp, len_aes);
        /* compare with ecb_d_m.txt 256 bits */
        acl_mov(ct, pt, 4);
        acl_xor(key, key, oct, 4);
        acl_xor(key+4, key+4, pt, 4);
    }
    h = stop_timer(0);
    put_val("  20000 = ", h);
    acl_str2bytes(tmp, (bytes) aes_ecb_de_results[2], 4);
    if (acl_cmp(ct, tmp, 4)) {
        return TRUE;
    }

    return FALSE;
}

bool_t test_aes_cbc_en(void) {

    /* 128-bit cbc monte carlo encryption test */
    put_str("\naes cbc en 128");
    len_aes = ACL_128;
    zero_all();
    for (i = 0; i < 2; i++) {
        /* compare with cbc_e_m.txt 128 bits */
        acl_aes_key_en(key_exp, key, len_aes);
        acl_mov(oct, iv, 4);
        for (j = 0; j < TEST_AES_ITER; j++) {
            acl_aes_cbc_en(ct, pt, key_exp, len_aes, iv);
            acl_mov(pt, oct, 4);
            acl_mov(oct, ct, 4);
        }
        acl_xor(key, key, ct, 4);
    }
    h = stop_timer(0);
    put_val("  20000 = ", h);
    acl_str2bytes(tmp, (bytes) aes_cbc_en_results[0], 4);
    if (acl_cmp(pt, tmp, 4)) {
        return TRUE;
    }

    /* 192-bit cbc monte carlo encryption test */
    put_str("\naes cbc en 192");
    len_aes = ACL_192;
    zero_all();
    for (i = 0; i < 2; i++) {
        /* compare with cbc_e_m.txt 192 bits */
        acl_aes_key_en(key_exp, key, len_aes);
        acl_mov(oct, iv, 4);
        for (j = 0; j < TEST_AES_ITER; j++) {
            acl_aes_cbc_en(ct, pt, key_exp, len_aes, iv);
            acl_mov(pt, oct, 4);
            acl_mov(oct, ct, 4);
        }
        acl_xor(key, key, pt+2, 2);
        acl_xor(key+2, key+2, ct, 4);
    }
    h = stop_timer(0);
    put_val("  20000 = ", h);
    acl_str2bytes(tmp, (bytes) aes_cbc_en_results[1], 4);
    if (acl_cmp(pt, tmp, 4)) {
        return TRUE;
    }

    /* 256-bit cbc monte carlo encryption test */
    put_str("\naes cbc en 256");
    len_aes = ACL_256;
    zero_all();
    for (i = 0; i < 2; i++) {
        /* compare with cbc_e_m.txt 256 bits */
        acl_aes_key_en(key_exp, key, len_aes);
        acl_mov(oct, iv, 4);
        for (j = 0; j < TEST_AES_ITER; j++) {
            acl_aes_cbc_en(ct, pt, key_exp, len_aes, iv);
            acl_mov(pt, oct, 4);
            acl_mov(oct, ct, 4);
        }
        acl_xor(key, key, pt, 4);
        acl_xor(key+4, key+4, ct, 4);
    }
    h = stop_timer(0);
    put_val("  20000 = ", h);
    acl_str2bytes(tmp, (bytes) aes_cbc_en_results[2], 4);
    if (acl_cmp(pt, tmp, 4)) {
        return TRUE;
    }

    return FALSE;
}

bool_t test_aes_cbc_de(void) {
    /* 128-bit cbc monte carlo decryption test */
    put_str("\naes cbc de 128");
    len_aes = ACL_128;
    zero_all();
    for (i = 0; i < 2; i++) {
        acl_aes_key_de(key_exp, key, len_aes);
        acl_mov(pt, ct, 4);
        for (j = 0; j < TEST_AES_ITER; j++) {
            acl_aes_cbc_de(pt, pt, key_exp, len_aes, iv);
        }
        /* compare with cbc_d_m.txt 128 bits */
        acl_mov(ct, pt, 4);
        acl_xor(key, key, pt, 4);
    }
    h = stop_timer(0);
    put_val("  20000 = ", h);
    acl_str2bytes(tmp, (bytes) aes_cbc_de_results[0], 4);
    if (acl_cmp(ct, tmp, 4)) {
        return TRUE;
    }

    /* 192-bit cbc monte carlo decryption test */
    put_str("\naes cbc de 192");
    len_aes = ACL_192;
    zero_all();
    for (i = 0; i < 2; i++) {
        acl_aes_key_de(key_exp, key, len_aes);
        acl_mov(pt, ct, 4);
        for (j = 0; j < TEST_AES_ITER-2; j++) {
            acl_aes_cbc_de(pt,pt, key_exp, len_aes, iv);
        }
        acl_aes_cbc_de(oct, pt, key_exp, len_aes, iv);
        acl_aes_cbc_de(pt, oct, key_exp, len_aes, iv);
        /* compare with cbc_d_m.txt 192 bits */
        acl_mov(ct, pt, 4);
        acl_xor(key, key, oct+2, 2);
        acl_xor(key+2, key+2, pt, 4);
    }
    h = stop_timer(0);
    put_val("  20000 = ", h);
    acl_str2bytes(tmp, (bytes) aes_cbc_de_results[1], 4);
    if (acl_cmp(ct, tmp, 4)) {
        return TRUE;
    }

    /* 256-bit cbc monte carlo decryption test */
    put_str("\naes cbc de 256");
    len_aes = ACL_256;
    zero_all();
    for (i = 0; i < 2; i++) {
        acl_aes_key_de(key_exp, key, len_aes);
        acl_mov(pt, ct, 4);
        for (j = 0; j < TEST_AES_ITER-2; j++) {
            acl_aes_cbc_de(pt,pt, key_exp, len_aes, iv);
        }
        acl_aes_cbc_de(oct, pt, key_exp, len_aes, iv);
        acl_aes_cbc_de(pt, oct, key_exp, len_aes, iv);
        /* compare with cbc_d_m.txt 256 bits */
        acl_mov(ct, pt, 4);
        acl_xor(key, key, oct, 4);
        acl_xor(key+4, key+4, pt, 4);
    }
    h = stop_timer(0);
    put_val("  20000 = ", h);
    acl_str2bytes(tmp, (bytes) aes_cbc_de_results[2], 4);
    if (acl_cmp(ct, tmp, 4)) {
        return TRUE;
    }

    return FALSE;
}

bool_t test_aes(void) {

    if (test_aes_ecb_en()) return TRUE;
    if (test_aes_ecb_de()) return TRUE;
    if (test_aes_cbc_en()) return TRUE;
    if (test_aes_cbc_de()) return TRUE;

    return FALSE;
}
