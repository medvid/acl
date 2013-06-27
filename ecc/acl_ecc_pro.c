// b(x,y) -> a(x,y,1)    copy a point in affine to projective coordinates

#include "acl.h"

void acl_ecc_pro(vect3 a, vect2 b, size_t len)
{
    acl_mov(a, b, 2*len);
    acl_mov32(a + 2*len, 1, len);
}
