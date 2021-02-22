#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 position; //signals that the data comes from a vertex buffer

void main() {
    gl_Position = vec4(position.x, -position.y, position.z, 1.0);
}
