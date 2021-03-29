#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 position; //signals that the data comes from a vertex buffer
layout(location = 1) in vec3 inColour;
layout(location = 2) in vec3 norm;
layout(location = 3) in vec3 up;
layout(location = 4) in vec2 tex;

layout(set = 0, binding = 0) uniform UboVP { //fixed ubo
    mat4 V;
    mat4 P;
    vec4 lightPos;
}uboVP; //uniform across al vertex

layout(push_constant) uniform pushObj{
    mat4 M;
    mat4 MinvT;
}pObj;
//not in use. left for reference.
/*
layout(binding = 1) uniform Ubo{ //dynamic ubo that varies with each object
    mat4 M;
    mat4 MinvT;
}ubo;
*/

layout(location = 0) out vec3 fragColour;
layout(location = 1) out vec3 normalForFP;
layout(location = 2) out vec3 positionForFP;
layout(location = 3) out vec3 lightPos;
layout(location = 4) out vec2 fragtex;
layout(location = 5) out mat4 fragM;

void main() {
    gl_Position = uboVP.P * uboVP.V * pObj.M * vec4(position, 1.0);
    fragColour = inColour;
    fragtex = tex;
    //fragColour = vec3(uboVP.M[0][0],uboVP.M[1][1],uboVP.M[2][2]);
    //fragColour = vec3(1.0f,1.0f,1.0f);

    vec4 tempLPos = uboVP.V * uboVP.lightPos;
    lightPos = tempLPos.xyz;
    vec4 tempPos = uboVP.V * pObj.M * vec4(position, 1.0);
    positionForFP = tempPos.xyz;
    vec4 tempNorm = uboVP.V * pObj.MinvT * vec4(norm, 0.0f);
    normalForFP = normalize(tempNorm.xyz);

    //model reconstructed from up and normal vector
    vec3 u = cross(up,norm);
    mat4 tempM = mat4(
    vec4(u,0.0f),
    vec4(up,0.0f),
    vec4(norm,0.0f),
    vec4(0.0f,0.0f,0.0f,1.0f)
    );
    fragM = uboVP.V * pObj.MinvT * tempM;
}
