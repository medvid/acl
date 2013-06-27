// perform known answer tests of the sha implementations

#include "acl.h"
#include "system.h"

uint state[68];  // sha-1: 23, sha-256: 26, sha-512: 68
uint i, h;
uint tmp[16];

byte sha_test_str1[] = "abc";
byte sha_test_str2[] = "abcdbcdecdefdefgefghfghighijhijkijkljklm" \
                        "klmnlmnomnopnopq";
byte sha_test_str3[] = "abcdefghbcdefghicdefghijdefghijkefghijkl" \
                       "fghijklmghijklmnhijklmnoijklmnopjklmnopq" \
                       "klmnopqrlmnopqrsmnopqrstnopqrstu";

const char *sha1_results[] = {
    "a9993e364706816aba3e25717850c26c9cd0d89d",
    "84983e441c3bd26ebaae4aa1f95129e5e54670f1",
    "34aa973cd4c4daa4f61eeb2bdbad27316534016f"
};

const char *sha224_results[] = {
    "23097d223405d8228642a477bda255b32aadbce4bda0b3f7e36c9da7",
    "75388b16512776cc5dba5da1fd890150b0c6455cb4f58b1952522525",
    "20794655980c91d8bbb4c1ea97618a4bf03f42581948b2ee4ee7ad67"
};

const char *sha256_results[] = {
    "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad",
    "248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1",
    "cdc76e5c9914fb9281a1c7e284d73e67f1809a48a497200e046d39ccc7112cd0"
};

const char *sha384_results[] = {
    "cb00753f45a35e8bb5a03d699ac65007272c32ab0eded163"
    "1a8b605a43ff5bed8086072ba1e7cc2358baeca134c825a7",

    "09330c33f71147e83d192fc782cd1b4753111b173b3b05d2"
    "2fa08086e3b0f712fcc7c71a557e2db966c3e9fa91746039",

    "9d0e1809716474cb086e834e310a4a1ced149e9c00f24852"
    "7972cec5704c2a5b07b8b3dc38ecc4ebae97ddd87f3d8985"
};

const char *sha512_results[] = {
    "ddaf35a193617abacc417349ae20413112e6fa4e89a97ea20a9eeee64b55d39a"
    "2192992a274fc1a836ba3c23a3feebbd454d4423643ce80e2a9ac94fa54ca49f",

    "8e959b75dae313da8cf4f72814fc143f8f7779c6eb9f7fa17299aeadb6889018"
    "501d289e4900f7e4331b99dec4b5433ac7d329eeb6dd26545e96e55b874be909",

    "e718483d0ce769644e2e42c7bc15b4638e1f98b13b2044285632a803afa973eb"
    "de0ff244877ea60a4cb0432ce577c31beb009c5c2c49aa2e4eadb217ad8cc09b"
};

static void sha1_str(bytes p) {
    acl_sha1_init(state);
    while(*p) acl_sha1(state, *p++);
    acl_sha1_done(state);
}

static void sha224_str(bytes p) {
    acl_sha224_init(state);
    while(*p) acl_sha256(state, *p++);
    acl_sha256_done(state);
}

static void sha256_str(bytes p) {
    acl_sha256_init(state);
    while(*p) acl_sha256(state, *p++);
    acl_sha256_done(state);
}

static void sha384_str(bytes p) {
    acl_sha384_init(state);
    while(*p) acl_sha512(state, *p++);
    acl_sha512_done(state);
}

static void sha512_str(bytes p) {
    acl_sha512_init(state);
    while(*p) acl_sha512(state, *p++);
    acl_sha512_done(state);
}

bool_t test_sha1(void) {

    put_str("\n\nsha-1");

    sha1_str(sha_test_str1);
    acl_str2hex_be(tmp, (bytes) sha1_results[0], 5);
    if(acl_cmp(state, tmp, 5)) return TRUE;

    sha1_str(sha_test_str2);
    acl_str2hex_be(tmp, (bytes) sha1_results[1], 5);
    if(acl_cmp(state, tmp, 5)) return TRUE;

    restart_timer(0);
    acl_sha1_init(state);
    for(i=0; i<1000000; i++) acl_sha1(state, 'a');
    acl_sha1_done(state);
    h = stop_timer(0); put_val("\n1000000 = ", h);
    acl_str2hex_be(tmp, (bytes) sha1_results[2], 5);
    if(acl_cmp(state, tmp, 5)) return TRUE;

    return FALSE;
}

bool_t test_sha224(void) {

    put_str("\n\nsha-224");

    sha224_str(sha_test_str1);
    acl_str2hex_be(tmp, (bytes) sha224_results[0], 7);
    if(acl_cmp(state, tmp, 7)) return TRUE;

    sha224_str(sha_test_str2);
    acl_str2hex_be(tmp, (bytes) sha224_results[1], 7);
    if(acl_cmp(state, tmp, 7)) return TRUE;

    restart_timer(0);
    acl_sha224_init(state);
    for(i=0; i<1000000; i++) acl_sha256(state, 'a');
    acl_sha256_done(state);
    h = stop_timer(0); put_val("\n1000000 = ", h);
    acl_str2hex_be(tmp, (bytes) sha224_results[2], 7);
    if(acl_cmp(state, tmp, 7)) return TRUE;

    return FALSE;
}

bool_t test_sha256(void) {

    put_str("\n\nsha-256");

    sha256_str(sha_test_str1);
    acl_str2hex_be(tmp, (bytes) sha256_results[0], 8);
    if(acl_cmp(state, tmp, 8)) return TRUE;

    sha256_str(sha_test_str2);
    acl_str2hex_be(tmp, (bytes) sha256_results[1], 8);
    if(acl_cmp(state, tmp, 8)) return TRUE;

    restart_timer(0);
    acl_sha256_init(state);
    for(i=0; i<1000000; i++) acl_sha256(state, 'a');
    acl_sha256_done(state);
    h = stop_timer(0); put_val("\n1000000 = ", h);
    acl_str2hex_be(tmp, (bytes) sha256_results[2], 8);
    if(acl_cmp(state, tmp, 8)) return TRUE;

    return FALSE;
}

bool_t test_sha384(void) {

    put_str("\n\nsha-384");

    sha384_str(sha_test_str1);
    acl_str2hex_be(tmp, (bytes) sha384_results[0], 12);
    if(acl_cmp(state, tmp, 12)) return TRUE;

    sha384_str(sha_test_str3);
    acl_str2hex_be(tmp, (bytes) sha384_results[1], 12);
    if(acl_cmp(state, tmp, 12)) return TRUE;

    restart_timer(0);
    acl_sha384_init(state);
    for(i=0; i<1000000; i++) acl_sha512(state, 'a');
    acl_sha512_done(state);
    h = stop_timer(0); put_val("\n1000000 = ", h);
    acl_str2hex_be(tmp, (bytes) sha384_results[2], 12);
    if(acl_cmp(state, tmp, 12)) return TRUE;

    return FALSE;
}

bool_t test_sha512(void) {

    put_str("\n\nsha-512");

    sha512_str(sha_test_str1);
    acl_str2hex_be(tmp, (bytes) sha512_results[0], 16);
    if(acl_cmp(state, tmp, 16)) return TRUE;

    sha512_str(sha_test_str3);
    acl_str2hex_be(tmp, (bytes) sha512_results[1], 16);
    if(acl_cmp(state, tmp, 16)) return TRUE;

    restart_timer(0);
    acl_sha512_init(state);
    for(i=0; i<1000000; i++) acl_sha512(state, 'a');
    acl_sha512_done(state);
    h = stop_timer(0); put_val("\n1000000 = ", h);
    acl_str2hex_be(tmp, (bytes) sha512_results[2], 16);
    if(acl_cmp(state, tmp, 16)) return TRUE;

    return FALSE;
}

bool_t test_sha(void) {

    if(test_sha1()) return TRUE;
    if(test_sha224()) return TRUE;
    if(test_sha256()) return TRUE;
    if(test_sha384()) return TRUE;
    if(test_sha512()) return TRUE;

    return FALSE;
}
