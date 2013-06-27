function b_inv = find_inverse (b_in, mod_pol)
for i = 1 : 255
    prod = poly_mult (b_in, i, mod_pol);
    if prod == 1
        b_inv = i;
        break
end
end
