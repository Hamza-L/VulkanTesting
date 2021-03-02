#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 position; //signals that the data comes from a vertex buffer

layout(location = 0) out vec2 pos;

void main() {
    gl_Position = vec4(position.x, -position.y, 0.0f, 1.0f);
    pos = vec2(position.x,position.y);
}
