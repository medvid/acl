function inv_s_box = s_box_inversion (s_box)
for i = 1 : 256
    inv_s_box(s_box(i) + 1) = i - 1;
end
