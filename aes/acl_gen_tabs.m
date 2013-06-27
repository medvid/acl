function gen_tabs()
% script generating AES tables (forward, inverse)
% this code is taken from the following matlab implementation of aes:
% http://buchholz.hs-bremen.de/aes/aes.htm

[s_box, i_box] = s_box_gen;
mod_pol = bin2dec ('100011011');

% rcon
fid = fopen('acl_aes_rcon.txt', 'wt');
fprintf(fid, '@ aes rcon\n');
h = 1;
for i = 1:14
    fprintf(fid, '.byte 0x%02x\n', h);
    h = poly_mult (h, 2, mod_pol);
end
status = fclose(fid);

% forward sbox
fid = fopen('acl_aes_fwd_sbox.txt', 'wt');
fprintf(fid, '@ aes forward sbox\n');
for i = 1:64
    fprintf(fid, '.byte 0x%02x, 0x%02x, 0x%02x, 0x%02x\n', s_box(4*i-3), s_box(4*i-2),
    s_box(4*i-1), s_box(4*i));
end
status = fclose(fid);

% inverse sbox
fid = fopen('acl_aes_inv_sbox.txt', 'wt');
fprintf(fid, '@ aes inverse sbox\n');
for i = 1:64
    fprintf(fid, '.byte 0x%02x, 0x%02x, 0x%02x, 0x%02x\n', i_box(4*i-3), i_box(4*i-2),
    i_box(4*i-1), i_box(4*i));
end
status = fclose(fid);

% forward table
fid = fopen('acl_aes_fwd_table.txt', 'wt');
fprintf(fid, '@ aes forward table (sub, mix)\n');
for i = 1:256
    h = s_box(i);
    r2 = poly_mult(h, 2, mod_pol);
    r3 = poly_mult(h, 3, mod_pol);
    fprintf(fid, '.int 0x%02x%02x%02x%02x\n', r3, h, h, r2);
end
status = fclose(fid);

% inverse table
fid = fopen('acl_aes_inv_table.txt', 'wt');
fprintf(fid, '@ aes inverse table (sub, mix)\n');
for i = 1:256
    h = i_box(i);
    rb = poly_mult(h, 11, mod_pol);
    rd = poly_mult(h, 13, mod_pol);
    r9 = poly_mult(h, 9, mod_pol);
    re = poly_mult(h, 14, mod_pol);
    fprintf(fid, '.int 0x%02x%02x%02x%02x\n', rb, rd, r9, re);
end
status = fclose(fid);
