function b_out = aff_trans (b_in)
mod_pol = bin2dec ('100000001');
mult_pol = bin2dec ('00011111');
add_pol = bin2dec ('01100011');
temp = poly_mult (b_in, mult_pol, mod_pol);
b_out = bitxor (temp, add_pol);
