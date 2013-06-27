#ifndef ACL_INT_H
#define ACL_INT_H

/* Macros to make the Montgomery arithmetic more readable */
#define acl_p_mul_mont(out, in1, in2) acl_p_mul(tmp, in1, in2, len); \
                                      acl_p_mont_red(out, tmp, m, m_inv, len)
#define acl_p_sqr_mont(out, in)       acl_p_sqr(tmp, in, len); \
                                      acl_p_mont_red(out, tmp, m, m_inv, len)

/* Macros to make the field arithmetic more readable */
#define acl_p_mul_sr(out, in1, in2) acl_p_mul(tmp, in1, in2, len); \
                                    acl_p_mod(out, tmp, 2*len, m, len)
#define acl_p_mul_fr(out, in1, in2) acl_p_mul(tmp, in1, in2, len); \
                                    acl_p_fr(out, tmp, fr, len)
#define acl_p_sqr_fr(out, in)       acl_p_sqr(tmp, in, len); \
                                    acl_p_fr(out, tmp, fr, len)
#define acl_2_mul_fr(out, in1, in2) acl_2_mul(tmp, in1, in2, len); \
                                    acl_2_fr(out, tmp, fr, len)
#define acl_2_sqr_fr(out, in)       acl_2_sqr(tmp, in, len); \
                                    acl_2_fr(out, tmp, fr, len)
#define xx a
#define xx1 a
#define xx2 b

#endif
