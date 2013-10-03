// test some ECC functions

#include "test.h"
#include "stdio.h"
#include "timer.h"

#define LEN 18      // the biggest curve (acl_sect571r1) needs 18 32-bit words
                    // of storage for each field element

uint tmp[(LEN+1)*10];
uint a[LEN*3];
uint b[LEN*3];
uint d[LEN*3];
uint dd[LEN+1];
uint pre[2*LEN*16];
char str[320];

// for each SECG curve, the following "ecc_comp_list" arrays contain compressed
//   base point representations
//   (taken from SEC 2: Recommended Elliptic Curve Domain Parameters)
// the library contains its own representation of the base points
// the library base points are compressed and compared to the official ones
// also, the official compressed points are decompressed
//   and compared with the library ones
// this way, both the compression and decompression routines are tested

#define CURVES 33
const ecc_t *ecc_list[] = {
                    &acl_secp112r1, &acl_secp112r2,
                    &acl_secp128r1, &acl_secp128r2,
    &acl_secp160k1, &acl_secp160r1, &acl_secp160r2,
    &acl_secp192k1, &acl_secp192r1,
    &acl_secp224k1, &acl_secp224r1,
    &acl_secp256k1, &acl_secp256r1,
                    &acl_secp384r1,
                    &acl_secp521r1,
                    &acl_sect113r1, &acl_sect113r2,
                    &acl_sect131r1, &acl_sect131r2,
    &acl_sect163k1, &acl_sect163r1, &acl_sect163r2,
                    &acl_sect193r1, &acl_sect193r2,
    &acl_sect233k1, &acl_sect233r1,
    &acl_sect239k1,
    &acl_sect283k1, &acl_sect283r1,
    &acl_sect409k1, &acl_sect409r1,
    &acl_sect571k1, &acl_sect571r1
};

const char *ecc_comp_list[] = {
    "0209487239995A5EE76B55F9C2F098",                       // acl_secp112r1
    "034BA30AB5E892B4E1649DD0928643",                       // acl_secp112r2
    "03161FF7528B899B2D0C28607CA52C5B86",                   // acl_secp128r1
    "027B6AA5D85E572983E6FB32A7CDEBC140",                   // acl_secp128r2
    "023B4C382CE37AA192A4019E763036F4F5DD4D7EBB",           // acl_secp160k1
    "024A96B5688EF573284664698968C38BB913CBFC82",           // acl_secp160r1
    "0252DCB034293A117E1F4FF11B30F7199D3144CE6D",           // acl_secp160r2
    "03DB4FF10EC057E9AE26B07D0280B7F4341DA5D1B1EAE06C7D",   // acl_secp192k1
    "03188DA80EB03090F67CBF20EB43A18800F4FF0AFD82FF1012",   // acl_secp192r1
    "03A1455B334DF099DF30FC28A169A467E9E47075A90F7E650E" \
      "B6B7A45C",                                           // acl_secp224k1,
    "02B70E0CBD6BB4BF7F321390B94A03C1D356C21122343280D6" \
      "115C1D21",                                           // acl_secp224r1
    "0279BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D9" \
      "59F2815B16F81798",                                   // acl_secp256k1
    "036B17D1F2E12C4247F8BCE6E563A440F277037D812DEB33A0" \
      "F4A13945D898C296",                                   // acl_secp256r1
    "03AA87CA22BE8B05378EB1C71EF320AD746E1D3B628BA79B98" \
      "59F741E082542A385502F25DBF55296C3A545E3872760AB7",   // acl_secp384r1
    "0200C6858E06B70404E9CD9E3ECB662395B4429C648139053F" \
      "B521F828AF606B4D3DBAA14B5E77EFE75928FE1DC127A2FF" \
      "A8DE3348B3C1856A429BF97E7E31C2E5BD66",               // acl_secp521r1
    "03009D73616F35F4AB1407D73562C10F",                     // acl_sect113r1
    "0301A57A6A7B26CA5EF52FCDB8164797",                     // acl_sect113r2
    "030081BAF91FDF9833C40F9C181343638399",                 // acl_sect131r1
    "030356DCD8F2F95031AD652D23951BB366A8",                 // acl_sect131r2
    "0302FE13C0537BBC11ACAA07D793DE4E6D5E5C94EEE8",         // acl_sect163k1
    "030369979697AB43897789566789567F787A7876A654",         // acl_sect163r1
    "0303F0EBA16286A2D57EA0991168D4994637E8343E36",         // acl_sect163r2
    "0301F481BC5F0FF84A74AD6CDF6FDEF4BF6179625372D8C0C5E1", // acl_sect193r1
    "0300D9B67D192E0367C803F39E1A7E82CA14A651350AAE617E8F", // acl_sect193r2
    "02017232BA853A7E731AF129F22FF4149563A419C26BF50A4C" \
      "9D6EEFAD6126",                                       // acl_sect233k1
    "0300FAC9DFCBAC8313BB2139F1BB755FEF65BC391F8B36F8F8" \
      "EB7371FD558B",                                       // acl_sect233r1
    "0329A0B6A887A983E9730988A68727A8B2D126C44CC2CC7B2A" \
      "6555193035DC",                                       // acl_sect239k1
    "020503213F78CA44883F1A3B8162F188E553CD265F23C1567A" \
      "16876913B0C2AC2458492836",                           // acl_sect283k1
    "0305F939258DB7DD90E1934F8C70B0DFEC2EED25B8557EAC9C" \
      "80E2E198F8CDBECD86B12053",                           // acl_sect283r1
    "030060F05F658F49C1AD3AB1890F7184210EFD0987E307C84C" \
      "27ACCFB8F9F67CC2C460189EB5AAAA62EE222EB1B35540CFE9" \
      "023746",                                             // acl_sect409k1
    "03015D4860D088DDB3496B0C6064756260441CDE4AF1771D4D" \
      "B01FFE5B34E59703DC255A868A1180515603AEAB60794E54BB" \
      "7996A7",                                             //acl_sect409r1
    "02026EB7A859923FBC82189631F8103FE4AC9CA2970012D5D4" \
      "6024804801841CA44370958493B205E647DA304DB4CEB08CBB" \
      "D1BA39494776FB988B47174DCA88C7E2945283A01C8972",     //acl_sect571k1
    "030303001D34B856296C16C0D40D3CD7750A93D1D2955FA80A" \
      "A5F40FC8DB7B2ABDBDE53950F4C0D293CDD711A35B67FB1499" \
      "AE60038614F1394ABFA3B4C850D927E1E7769C8EEC2D19"      //acl_sect571r1
};

static bool_t str_cmp(char *str1, char *str2) {
    while ((*str1) && (*str2)) {
        if (*str1++ != *str2++) {
            return TRUE;
        }
    }
    return FALSE;
}

bool_t test_ecc(void) {

    ecc_t *c; int i, j, k; //uint h, avg1, avg2, avg3;

    for (j = 0; j < TEST_ECC_ITER; j++) {
        for(i = 0; i < CURVES; i++) {
            c = (ecc_t *) ecc_list[i];

            // print name of curve
            put_str("\n\n");
            put_str((char *) c->s);

#if 1       // test basic ecc operations

            // make sure that the base point lies on the curve
            if (!acl_ecc_chk(c->g, tmp, c)) {
                put_str(" chk");
                return TRUE;
            }

            // generate random point
            acl_prng_lc(tmp, c->ln);
            acl_p_mod(dd, tmp, c->ln, c->n, c->ln);
            acl_ecc_mul(d, c->g, 0, 1, 0, dd, 0, c->ln, tmp, c);

            // basic operations
            acl_mov32(a + 2*c->l, 0, c->l);         // a = point at infinity
            for (k = 0; k < 8; k++) {
                acl_ecc_add(a, d, tmp, c);
            }
            acl_ecc_aff(a, tmp, c);                 // a = a+d+d+...+d = 8d

            acl_ecc_pro(b, d, c->l);                // b = d
            for (k = 0; k < 3; k++) {
                acl_ecc_dbl(b, tmp, c);
            }
            acl_ecc_aff(b, tmp, c);                 // b = 2*2*2*b = 8d

            if (acl_cmp(a, b, 2*c->l)) {            // is a == b ?
                put_str(" add/mul");
                return TRUE;
            }

            // point to string with compression (base point)
            acl_ecc_p2str(str, c->g, 1, tmp, c);
            put_str("\nG = ");
            put_str(str);
            if (str_cmp(str, (char *) ecc_comp_list[i])) {
                put_str(" p2str w/ comp");
                return TRUE;
            }

            // string to point with compression (base point)
            if (!acl_ecc_str2p(a, str, tmp, c)) {
                put_str(" invalid");
            }
            if (acl_cmp(a, c->g, 2*c->l)) {
                put_str(" str2p w/ decomp");
                return TRUE;
            }

            // point to string conversion without compression (base point)
            acl_ecc_p2str(str, c->g, 0, tmp, c);
            put_str("\nG = ");
            put_str(str);

            // string to point conversion without compression (base point)
            if (!acl_ecc_str2p(a, str, tmp, c)) {
                put_str(" invalid");
            }
            if (acl_cmp(a, c->g, 2*c->l)) {
                put_str(" str2p w/o comp");
                return TRUE;
            }

            // point to string with compression (random point)
            acl_ecc_p2str(str, d, 1, tmp, c);
            put_str("\nD = ");
            put_str(str);

            // string to point with compression (random point)
            if (!acl_ecc_str2p(a, str, tmp, c)) {
                put_str(" invalid");
            }
            if (acl_cmp(a, d, 2*c->l)) {
                put_str(" str2p w/ comp");
                return TRUE;
            }

            // point to string conversion without compression (random point)
            acl_ecc_p2str(str, d, 0, tmp, c);
            put_str("\nD = ");
            put_str(str);

            // string to point conversion without compression (random point)
            if (!acl_ecc_str2p(a, str, tmp, c)) {
                put_str(" invalid");
            }
            if (acl_cmp(a, d, 2*c->l)) {
                put_str(" str2p w/o comp");
                return TRUE;
            }

#else       // this code was used to generate a table of field operation timings
            // (multiplication, fast reduction, inversion)

                //acl_mov32(dd, 1, c->l); // recover m from fr
                //k = 0;
                //while(c->fr[k]) {
                //    acl_bit_set(dd, c->fr[k]); k++;
                //}
                avg1 = 0; avg2 = 0; avg3 = 0;
                for (k = 0; k < 16; k++) {
                    acl_prng_lc(a, c->l);
                    acl_prng_lc(b, c->l);
                    acl_prng_lc(d, c->l);

                    restart_timer(0);
                    acl_p_mul(tmp, a, b, c->l);
                    //acl_2_mul(tmp, a, b, c->l);
                    h = stop_timer(0);
                    avg1 += h;
                    //put_val("\nm=", h);

                    restart_timer(0);
                    acl_p_fr(b, tmp, c->fr, c->l);
                    //acl_2_fr(b, tmp, c->fr, c->l);
                    h = stop_timer(0);
                    avg2 += h;
                    //put_val(" f=", h);

                    restart_timer(0);
                    acl_p_mod_inv(b, a, 0, c->m, tmp, c->l);
                    //acl_2_mod_inv(b, a, dd, tmp, c->l);
                    h = stop_timer(0);
                    avg3 += h;
                    //put_val(" i=", h);
            }
            put_val("\nm=", avg1 >> 4);
            put_val(" f=", avg2 >> 4);
            put_val(" i=", avg3 >> 4);
#endif
        }
    }
    return FALSE;
}
