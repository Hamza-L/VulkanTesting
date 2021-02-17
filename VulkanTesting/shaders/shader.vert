#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec3 fragColor;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColour;

void main() {
    gl_Position = vec4(inPosition.x,-inPosition.y,inPosition.z, 1.0);
    fragColor = inColour;
}
