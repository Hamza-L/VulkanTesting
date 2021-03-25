#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragColour;
layout(location = 1) in vec3 normalForFP;
layout(location = 2) in vec3 positionForFP;
layout(location = 3) in vec3 lightPos;

layout(location = 0) out vec4 outColor;

void main() {
    vec3 lightDirection = normalize(lightPos - positionForFP );
    vec3 viewDirection = normalize(-positionForFP );
    vec3 halfVector = normalize( lightDirection + viewDirection);

    float diffuse = max(0.0f,dot( normalForFP, lightDirection));
    float specular = max(0.0f,dot( normalForFP, halfVector ) );

    if (diffuse == 0.0) {
        specular = 0.0;
    } else {
        specular = pow( specular, 16.0f );
    }

    vec3 scatteredLight = fragColour * diffuse;
    vec3 reflectedLight = vec3(1.0f,1.0f,1.0f) * specular;
    vec3 ambientLight = fragColour * 0.01f;

     outColor = vec4(min( ambientLight + scatteredLight + reflectedLight, vec3(1,1,1)), 1.0);
    //outColor = vec4(normalForFP, 1.0);
    //outColor = vec4(fragColour, 1.0f);
    //outColor = vec4(scatteredLight + reflectedLight,1.0f);
}
