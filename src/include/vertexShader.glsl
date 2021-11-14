#version 410

layout (location=0) in vec3 position;
//layout (location=1) in vec3 color;
//layout (location=2) in vec3 texture;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 camera_matrix;

out vec4 varyingColor;
out vec3 tc;

void main(void) {
   tc = position;
   gl_Position = camera_matrix * proj_matrix * mv_matrix * vec4(position, 1.0);
   varyingColor = vec4(position, 1.0) * 0.5 + vec4(0.5, 0.5, 0.5, 0.5);
}