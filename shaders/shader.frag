#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragColour;
layout(location = 1) in vec3 normalForFP;
layout(location = 2) in vec3 positionForFP;

layout(location = 0) out vec4 outColor;

vec3 eye = normalize(vec3(0.0f, 0.0f, 2.5f));
vec3 lightPos = vec3(1.0f, -3.0f, 2.0f);

void main() {
    vec3 lightDirection = normalize( lightPos - positionForFP );
    vec3 viewDirection = normalize(- positionForFP );
    vec3 halfVector = normalize( lightDirection + viewDirection);

    float diffuse = max(0.0f,dot(eye, normalForFP));
    float specular = max( 0.0f, dot( normalForFP, halfVector ) );

    if (diffuse == 0.0) {
        specular = 0.0;
    } else {
        specular = pow( specular, 8.0f );
    }

    vec3 scatteredLight = fragColour * diffuse;
    vec3 reflectedLight = vec3(1.0f,1.0f,1.0f) * specular;

    outColor = vec4(min( scatteredLight + reflectedLight, vec3(1,1,1)), 1.0);
}
