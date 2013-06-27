function ab = poly_mult (a, b, mod_pol)
ab = 0;
for i_bit = 1 : 8
    if bitget (a, i_bit)
        b_shift = bitshift (b, i_bit - 1);
        ab = bitxor (ab, b_shift);
end
end
for i_bit = 16 : -1 : 9
    if bitget (ab, i_bit)
        mod_pol_shift = bitshift (mod_pol, i_bit - 9);
        ab = bitxor (ab, mod_pol_shift);
end
end
