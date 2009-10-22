vs_1_1
dcl_position v0
dcl_color v1
dcl_texcoord v2

m4x4 r0, v0, c4
m4x4 r1, v0, c8
mul r0, r0, v2.xxxx
mul r1, r1, v2.yyyy
add r0, r0, r1

m4x4 oPos, r0, c0
mov oD0, v1