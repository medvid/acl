function [s_box, inv_s_box] = s_box_gen
mod_pol = bin2dec ('100011011');
inverse(1) = 0;
for i = 1 : 255
    inverse(i + 1) = find_inverse (i, mod_pol);
end
for i = 1 : 256
    s_box(i) = aff_trans (inverse(i));
end
inv_s_box = s_box_inversion (s_box);
