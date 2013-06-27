#include "system.h"
#include "acl.h"

bool_t test_aes(void);
bool_t test_sha(void);
bool_t test_rsa(void);
bool_t test_ecc(void);
bool_t test_ecdsa(void);


static void acl_error(char *p) {
    put_str("\nerror: "); put_str(p);
    while(1) ;
}

static void init_random(void) {
    int res;

    put_str("\nEnter 4 chars: ");
    res = get_char();
    res = (res << 8) | get_char();
    res = (res << 8) | get_char();
    acl_prng_lc_init(res);
}

int main(void)
{
    init_serial();
    init_random();

    init_timers(UNIT_CYCLES);
    if(test_aes()) acl_error("AES");

    init_timers(UNIT_MICROSECONDS);
    if(test_sha()) acl_error("SHA");
    if(test_rsa()) acl_error("RSA");
    if(test_ecc()) acl_error("ECC");
    if(test_ecdsa()) acl_error("ECDSA");

    put_str("\n\na-ok");   // if program gets here, the tests have been passed
    while(1) ;
}
