/*
    ARM Cryptographic Library
    -------------------------------------------------------
    Author: Jaroslav Ban      Supervisor: Milos Drutarovsky
    Department of Electronics and Multimedia Communications
           Technical University of Kosice (Slovakia)
    Version: 1.00                 Last revision: 2007-04-24
    -------------------------------------------------------
    Contributor: Volodymyr Medvid
    Version: 1.01                 Last revision: 2013-09-06
*/

#ifndef ACL_H
#define ACL_H

/*
 Notes:
 - certain parts of this library may be covered by certain patents
   in certain coutries; if you want to use said parts in said coutries,
   make sure to first obtain a license from the respective patent holder(s)
 - most routines will work if you use them in-place(result == input)
 - some routines will not work with "len" small (assuming len ~ 4 and more)
 - AES routines are byte-order dependent (little-endian)
 - most routines assume that m (the modulus) is odd,
   and won't work properly with m even
*/

#define TRUE -1
#define FALSE 0

#define VPint       *(volatile unsigned int *)

/* The vect type is an array of 32-bit words with the LSW first (offset +0)
   and the MSW last (offset +4*(length-1)), its size indicated by "len".
   The following types "vectN" imply an N-times bigger array than vect. */
typedef unsigned int * vect;
typedef unsigned int * vect2;
typedef unsigned int * vect3;
typedef unsigned int * vect4;
typedef unsigned int * vect5;
typedef unsigned int * vect6;
typedef unsigned int * vect7;
typedef unsigned int * vect8;
typedef unsigned int * vect9;
typedef unsigned int * vect10;
typedef unsigned int * vect11;
typedef unsigned int * vectN;

/* The following vect types indicate the size of the array in ints. */
typedef unsigned int * vect16;
typedef unsigned int * vect23;
typedef unsigned int * vect26;
typedef unsigned int * vect68;

typedef char byte;
typedef char * bytes;
typedef unsigned int uint;      // 32-bit integers
typedef int bool_t;             // booleans
typedef unsigned int size_t;    // lengths (in multiples of 32-bits)
typedef unsigned int * list;    // various lists

/* Pseudo-random number generator function type */
typedef void (* prng)(vect, size_t);

/* Lengths of "vects" (bits -> 32-bit words) */
#define ACL_64 2
#define ACL_96 3
#define ACL_128 4
#define ACL_192 6
#define ACL_224 7
#define ACL_256 8
#define ACL_384 12
#define ACL_512 16
#define ACL_768 24
#define ACL_1024 32
#define ACL_1536 48
#define ACL_2048 64
#define ACL_4096 128

/* AES */
void acl_aes_key_en(vect key_out, vect key_in, size_t key_size);
                                                // expand key for encryption
void acl_aes_key_de(vect key_out, vect key_in, size_t key_size);
                                                // expand key for decryption
void acl_aes_ecb_en(vect4 out, vect4 in, vect exp_key, size_t key_size);
                                                // encrypt in ecb mode
void acl_aes_ecb_de(vect4 out, vect4 in, vect exp_key, size_t key_size);
                                                // decrypt in ecb mode
void acl_aes_cbc_en(vect4 out, vect4 in, vect exp_key, \
                    size_t key_size, vect4 state);  // encrypt in cbc mode
void acl_aes_cbc_de(vect4 out, vect4 in, vect exp_key, \
                    size_t key_size, vect4 state);  // decrypt in cbc mode
void acl_aes_cntr(vect4 out, vect4 in, vect exp_key, \
                  size_t key_size, vect4 counter); // encrypt in counter mode

/* SHA */
void acl_sha1_init(vect23 state);
void acl_sha1(vect23 state, byte data);
void acl_sha1_done(vect23 state);
void acl_sha224_init(vect26 state);
void acl_sha256_init(vect26 state);
void acl_sha256(vect26 state, byte data);        // (== acl_sha224)
void acl_sha256_done(vect26 state);              // (== acl_sha224_done)
void acl_sha384_init(vect68 state);
void acl_sha512_init(vect68 state);
void acl_sha512(vect68 state, byte data);        // (== acl_sha384)
void acl_sha512_done(vect68 state);              // (== acl_sha384_done)

// Functions with a '32' suffix are versions of theoriginal functions where
// one of the operands is a 32-bit value (the more significant bits being zero).

/* The Commons */
void acl_mov(vect res, vect src, size_t len);         // res = src
void acl_mov32(vect res, uint val, size_t len);       // res = val[32-bit]
void acl_xor(vect res, vect a, vect b, size_t len);   // res = a xor b
void acl_xor32(vect res, vect a, uint b, size_t len); // res = a xor b[32-bit]
uint acl_ctz(vect a, size_t len);           // count trailing zeroes
int acl_log2(vect a, size_t len);           // position of highest non-zero bit
uint acl_bit(vect a, uint pos, size_t len); // return value of bit
void acl_bit_set(vect a, uint pos);         // set bit at given position
void acl_bit_clr(vect a, uint pos);         // clear bit at given position
int acl_cmp(vect a, vect b, size_t len);    // compare two arrays
bool_t acl_zero(vect a, size_t len);        // returns true if the array is zero
void acl_rsh(vect a, uint k, size_t len);   // a = a >> k
uint acl_rev(uint a);                       // return value with byte order reversed
void acl_rev_bytes(vect a, size_t len);     // reverse byte order in array
void acl_hex2str_dec(bytes res, size_t len_r, vect a, size_t len);
                              // convert number to string(decimal)
void acl_hex2str_le(bytes res, vect a, size_t len);
                              // convert number (little endian) to string(hex)
void acl_str2hex_le(vect res, size_t len, bytes str, size_t len_s);
                              // convert string(hex) to number (little endian)
void acl_str2bytes(vect res, bytes str, size_t len);
                              // convert string(hex) to array of bytes
void acl_str2hex_be(vect res, bytes str, size_t len);
                              // convert string(hex) to number (big endian)
void acl_str2hex(vect res, bytes str, size_t len);
                              // convert string(hex) to number
                              // use configured target endianess

/* GF(p) */
uint acl_p_mod_add(vect res, vect a, vect b, vect m, size_t len);
                                                // res = (a + b) mod m
uint acl_p_mod_add32(vect res, vect a, uint b, vect m, size_t len);
                                                // res = (a + b[32-bit]) mod m
void acl_p_mod_sub(vect res, vect a, vect b, vect m, size_t len);
                                                // res = (a - b) mod m
void acl_p_mod_sub32(vect res, vect a, uint b, vect m, size_t len);
                                                // res = (a - b[32-bit]) mod m
uint acl_p_mod_dbl(vect a, uint k, vect m, size_t len); // a = a*(2^k) mod m
void acl_p_mod_hlv(vect a, uint k, vect m, size_t len); // a = a/(2^k) mod m
void acl_p_mul(vect2 res, vect a, vect b, size_t len);
                                            // res = a * b, res != a, res != b
void acl_p_sqr(vect2 res, vect a, size_t len);  // res = a * a, res != a
void acl_p_mod(vect res, vect a, size_t len_a, vect m, size_t len);
                                                // res = a mod m, res != a !!!
void acl_p_div(vect a, size_t len_a, vect m, vect tmp, size_t len);
                                                // a = a div m
bool_t acl_p_sqrt(vect res, vect a, vect m, prng rnd, vect8 tmp, size_t len);
                                                // res^2 = a mod m, res != a
void acl_p_fr(vect res, vect2 a, list data, size_t len);
                                        // res = fast reduction(a), res != a
bool_t acl_p_coprime(vect a, vect b, vect2 tmp, size_t len);
                                                // true if gcd(a,b) == 1
int acl_p_mont_inv(vect res, vect a, vect m, vect3 tmp, size_t len);
                // res = a^(-1)*(+-2^k) mod m, m odd, a!=0, a!=1, m mod 2 == 1
void acl_p_mod_inv(vect res, vect a, uint e, vect m, vect3 tmp, size_t len);
                            // res = a^(-1)*(2^e) mod m, m mod 2 == 1, res != a

/* Montgomery */
uint acl_p_mont_m_inv(vect m);              // returns -m^(-1) mod 2^32
void acl_p_mont_pre(vect r_mod_m, vect r2_mod_m, uint *m_inv, \
vect m, size_t len);                        // precomputation for montgomery
void acl_p_mont_red(vect res, vect2 a, vect m, uint m_inv, size_t len);
                                            // res = a*r^(-1) mod m, res!=a !!!
void acl_p_mont_exp(vect res, vect x, vect e, size_t len_e, vect m, vect3 tmp, \
                    uint m_inv, vect r2_mod_m, size_t len); // res = x^e mod m

/* Pseudorandom number generators */
void acl_prng_lc_init(uint seed);        // linear congruential prng
void acl_prng_lc(vect res, size_t len);  // for bootstrapping purposes
void acl_prng_aes_init(prng rnd);
void acl_prng_aes(vect res, size_t len);            // aes
void acl_prng_sha_init(prng rnd);
void acl_prng_sha(vect res, size_t len);            // sha-1
void acl_prng_bbs_init(prng rnd_fast, prng rnd, vect7 tmp);
                       // note that vect7 here means 7*ACL_PRNG_BBS_SIZE
void acl_prng_bbs(vect res, size_t len);            // blum-blum-shub

/* Primes */
extern uint *acl_pop_table;             // product-of-small-primes table
bool_t acl_p_rm_test(vect a, vect m, vect4 tmp, uint m_inv, \
                     vect r_mod_m, vect r2_mod_m, size_t len);
                                        // rabin-miller test
bool_t acl_p_rm_test2(vect m, vect3 tmp, uint m_inv, \
                      vect r_mod_m, size_t len);
                                        // rabin-miller test with a == 2
void acl_p_rnd_prime(vect res, vect7 tmp, uint k, uint also_set, \
                     prng rnd_fast, prng rnd_strong, size_t len);
                                        // generate random prime

/* RSA */
bool_t acl_rsa_pre(vect2 n, vect2 d, vect dmp1, vect dmq1, vect iqmp, \
                   vect2 e, vect p, vect q, vect6 tmp, size_t len);
                                        // n, d, dmp1, dmq1, iqmp = f(e, p, q)
void acl_rsa_crt(vect2 pt, vect2 ct, \
                 vect p, vect r2_mod_p, uint p_inv,\
                 vect q, vect r2_mod_q, uint q_inv,
                 vect dmp1, vect dmq1, vect iqmp, vect4 tmp, size_t len);
                                        // pt = rsa_inv(ct) (using crt)

/* GF(2^m) */
void acl_2_mul(vect2 res, vect a, vect b, size_t len);
                                        // res = a * b, res != a, res != b
void acl_2_sqr(vect2 res, vect a, size_t len);  // res = a * a, res != a
void acl_2_fr(vect res, vect2 a, list data, size_t len);
                                        // res = fast reduction(a), res != a
int acl_2_mont_inv(vect res, vect a, vect poly, vect3 tmp, size_t len);
                    // res = a^(-1)*z^k mod poly, a!=0, a!=1, poly mod z == 1
void acl_2_mod_hlv(vect a, uint k, vect poly, size_t len);
                                    // a = a/(z^k) mod poly, poly mod z == 1
void acl_2_mod_inv(vect res, vect a, vect poly, vect3 tmp, size_t len);
                        // res = a^(-1) mod poly, poly mod z == 1, res != a

/* ECC curve struct */
typedef struct {
    const char *s;      // name of curve
    uint t;             // type of curve (see below)
    size_t l;           // length of m, g (2x), a, b in 32-bit words
    vect m;             // pointer to modulus or reduction polynomial
    list fr;            // pointer to fast reduction data
    vect2 g;            // pointer to base point (x, y, affine)
    vect a;             // pointer to a or: 0, 1, -3
    vect b;             // pointer to b or: 0 .. ACL_MAX_B (see acl_config.h)
    vect n;             // order of base point
    size_t ln;          // length of order in 32-bit words
    uint h;             // cofactor - currently hasno effect
    void *f;            // pointers to field specific ecc functions
} ecc_t;

/* ECC function struct */
typedef struct {
    bool_t (*chk)(vect, vect, ecc_t *);   // add point compression/decompression
    void (*dbl)(vect, vect, ecc_t *);
    void (*add)(vect, vect, vect, ecc_t *);
    void (*aff)(vect, vect, ecc_t *);
    void (*p2str)(bytes, vect, bool_t, vect, ecc_t *);
    bool_t (*str2p)(vect, bytes, vect, ecc_t *);
} ecc_func_t;

/* ECC curve type flags */
#define ECC_P 0     // curve is over GF(p)
#define ECC_2 1     // curve is over GF(2)
#define ECC_K 2     // koblitz - currently has no effect
#define ECC_A 4     // almost prime - used for acl_secp112r1 and acl_secp112r2
                    // the modulus c->m = the almost prime,
                    // c->m+len = the real prime.
                    // the reduction polynomial is for the almost prime.

#define ECC_F_MASK 1    // field mask
#define ECC_K_MASK 2    // koblitz mask
#define ECC_A_MASK 4    // almost prime mask

/* SECG-recommended curves */
extern const ecc_t                acl_secp112r1, acl_secp112r2;
extern const ecc_t acl_secp128r1, acl_secp128r2;
extern const ecc_t acl_secp160k1, acl_secp160r1, acl_secp160r2;
extern const ecc_t acl_secp192k1, acl_secp192r1;
extern const ecc_t acl_secp224k1, acl_secp224r1;
extern const ecc_t acl_secp256k1, acl_secp256r1;
extern const ecc_t                acl_secp384r1;
extern const ecc_t                acl_secp521r1;

extern const ecc_t                acl_sect113r1, acl_sect113r2;
extern const ecc_t                acl_sect131r1, acl_sect131r2;
extern const ecc_t acl_sect163k1, acl_sect163r1, acl_sect163r2;
extern const ecc_t                acl_sect193r1, acl_sect193r2;
extern const ecc_t acl_sect233k1, acl_sect233r1;
extern const ecc_t acl_sect239k1;
extern const ecc_t acl_sect283k1, acl_sect283r1;
extern const ecc_t acl_sect409k1, acl_sect409r1;
extern const ecc_t acl_sect571k1, acl_sect571r1;

/* NIST-recommended curves */
#define P_192 acl_secp192r1
#define P_224 acl_secp224r1
#define P_256 acl_secp256r1
#define P_384 acl_secp384r1
#define P_521 acl_secp521r1

#define K_163 acl_sect163k1
#define K_233 acl_sect233k1
#define K_283 acl_sect283k1
#define K_409 acl_sect409k1
#define K_571 acl_sect571k1

#define B_163 acl_sect163r2
#define B_233 acl_sect233r1
#define B_283 acl_sect283r1
#define B_409 acl_sect409r1
#define B_571 acl_sect571r1

/* ECC function sets */
extern const ecc_func_t acl_p_ecc_func;
extern const ecc_func_t acl_2_ecc_func;

/* ECC points are represented thus:
   Jacobian/Lopez-Dahab coordinates: vect3 (x, y, z)
   Affine coordinates:               vect2 (x, y)      */

/* ECC arithmetic - GF(p) */
bool_t acl_p_ecc_chk(vect2 a, vect4 tmp, ecc_t *c);   // is point a on curve c?
void acl_p_ecc_add(vect3 a, vect2 b, vect5 tmp, ecc_t *c); // a = a + b
void acl_p_ecc_dbl(vect3 a, vect4 tmp, ecc_t *c);       // a = a + a
void acl_p_ecc_aff(vect3 a, vect4 tmp, ecc_t *c);       // (x,y,z) -> (x',y')

/* ECC arithmetic - GF(2^m) */
bool_t acl_2_ecc_chk(vect2 a, vect4 tmp, ecc_t *c);   // is point a on curve c?
void acl_2_ecc_add(vect3 a, vect2 b, vect5 tmp, ecc_t *c); // a = a + b
void acl_2_ecc_dbl(vect3 a, vect4 tmp, ecc_t *c);       // a = a + a
void acl_2_ecc_aff(vect3 a, vect5 tmp, ecc_t *c);       // a(x,y,z) -> a(x',y')

/* ECC arithmetic - generic */
void acl_ecc_pro(vect3 a, vect2 b, size_t len);         // b(x,y) -> a(x,y,1)
void acl_ecc_neg(vect3 a, ecc_t *c);                    // a = -a
void acl_ecc_pre(vectN pre, vect2 p, uint w, uint s, vect8 tmp, ecc_t *c);
                                                    // precomputation (comb)
void acl_ecc_mul(vect3 res, vect p, vect q, uint w,uint s, vect k, vect l, \
                 size_t len_kl, vect5 tmp, ecc_t *c);
                                            // linear combination of two points
                                            //
/* ECC point <-> string conversion */
void acl_p_ecc_p2str(bytes str, vect2 a, bool_t comp, vect tmp, ecc_t *c);
                                // point a(x,y) -> string(comp/no comp)
void acl_2_ecc_p2str(bytes str, vect2 a, bool_t comp, vect5 tmp, ecc_t *c);
                                // point a(x,y) -> string(comp/no comp)
bool_t acl_p_ecc_str2p(vect2 a, bytes str, vect9 tmp, ecc_t *c);
                                // string(comp/no comp) -> point a(x,y)
bool_t acl_2_ecc_str2p(vect2 a, bytes str, vect6 tmp, ecc_t *c);
                                // string(comp/no comp) -> point a(x,y)

/* ECC protocols */
void acl_ecdsa_gen(vect r, vect s, vect e, size_t len_e, vect dA, \
                   vectN base, uint wi, uint sp, \
                   prng rnd_strong, vect9 tmp, ecc_t *c);
                                                    // generate ecdsa signature
bool_t acl_ecdsa_ver(vect r, vect s, vect e, size_t len_e, vectN qA, \
                     vectN base, uint wi, uint sp, vect10 tmp, ecc_t *c);
                                                    // verify ecdsa signature

/* Macros to make the ECC arithmetic more readable */
#define acl_ecc_chk(a, b, c) ((ecc_func_t *) c->f)->chk(a, b, c)
#define acl_ecc_dbl(a, b, c) ((ecc_func_t *) c->f)->dbl(a, b, c)
#define acl_ecc_add(a, b, c, d) ((ecc_func_t *) d->f)->add(a, b, c, d)
#define acl_ecc_aff(a, b, c) ((ecc_func_t *) c->f)->aff(a, b, c)
#define acl_ecc_p2str(a, b, c, d, e) ((ecc_func_t *) e->f)->p2str(a, b, c, d, e)
#define acl_ecc_str2p(a, b, c, d) ((ecc_func_t *) d->f)->str2p(a, b, c, d)

#endif
