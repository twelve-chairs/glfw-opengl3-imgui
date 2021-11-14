#version 410

in vec4 varyingColor;

in vec3 tc;
out vec4 color1;
out vec4 color2;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform int tile_scale;

vec3 checkerboard(vec3 tc) {
    int tileScale = tile_scale;
    float tile = mod(floor(tc.x * tileScale) + floor(tc.y * tileScale), 2.0);
    return tile * vec3(1, 1, 1);
}

void main(void) {
    color1 = varyingColor;
    color2 = vec4(checkerboard(tc), 1);
}