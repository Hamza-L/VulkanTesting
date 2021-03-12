#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 position; //signals that the data comes from a vertex buffer
layout(location = 1) in vec3 inColour;
layout(location = 2) in vec3 norm;

layout(location = 0) out vec3 fragColour;
layout(location = 1) out vec3 normalForFP;
layout(location = 2) out vec3 positionForFP;

mat4 V = {
vec4(1.0f,0.0f,0.0f,0.0f),
vec4(0.0f,1.0f,0.0f,0.0f),
vec4(0.0f,0.0f,1.0f,0.0f),
vec4(0.0f,0.0f,-2.5f,1.0f),
};

mat4 P = {
vec4(1.0f,0.0f,0.0f,0.0f),
vec4(0.0f,1.0f,0.0f,0.0f),
vec4(0.0f,0.0f,-2.0f,-1.0f),
vec4(0.0f,0.0f,-3.0f,1.0f),
};

void main() {
    gl_Position = P * V * vec4(0.75f*position.x, -position.y, position.z, 1.0);
    fragColour = inColour;

    vec4 tempPos = V * vec4(0.75f*position.x, -position.y, position.z, 1.0);
    vec4 tempNorm = V * vec4(norm.x, -norm.y, norm.z, 0.0f);
    positionForFP = tempPos.xyz;
    normalForFP = normalize(tempNorm.xyz);
}
