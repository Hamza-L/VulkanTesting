#version 450
#extension GL_ARB_separate_shader_objects : enable

vec3 inPosition[3] = vec3[](
vec3(0.5f,0.0f,0.0f),
vec3(-0.5f,0.0f,0.0f),
vec3(0.0f,-0.5f,0.0f)
);

void main() {
    gl_Position = vec4(inPosition[gl_VertexIndex], 1.0);
}
