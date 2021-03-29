#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragColour;
layout(location = 1) in vec3 normalForFP;
layout(location = 2) in vec3 positionForFP;
layout(location = 3) in vec3 lightPos;
layout(location = 4) in vec2 fragtex;
layout(location = 5) in mat4 fragM;

layout(set = 1, binding = 0) uniform sampler2D texSampler;
layout(set = 1, binding = 1) uniform sampler2D normSampler;

layout(location = 0) out vec4 outColor;

void main() {
    vec4 colourTex = texture(texSampler, fragtex);

    vec4 normcolour = vec4(texture(normSampler, fragtex).xyz,0.5f);
    normcolour = (normcolour * 2.0f) - 1.0f;
    normcolour.y = -normcolour.y;
    normcolour = fragM * normcolour;
    normcolour = normalize(normcolour);

    vec3 lightDirection = normalize(lightPos - positionForFP );
    vec3 viewDirection = normalize(-positionForFP );
    vec3 halfVector = normalize( lightDirection + viewDirection);

    float diffuse = max(0.0f,dot( normcolour.xyz, lightDirection));
    float specular = max(0.0f,dot( normcolour.xyz, halfVector ) );

    if (diffuse == 0.0) {
        specular = 0.0;
    } else {
        specular = pow( specular, 32.0f );
    }

    vec3 scatteredLight = colourTex.xyz * diffuse;
    vec3 reflectedLight = vec3(1.0f,1.0f,1.0f) * specular;
    vec3 ambientLight = colourTex.xyz * 0.01f;

    outColor = vec4(min( ambientLight + scatteredLight + reflectedLight, vec3(1,1,1)), 1.0);
    //outColor = vec4(normcolour.xyz, 1.0);
    //outColor = texture(texSampler, fragtex);
    //outColor = vec4(normalForFP,1.0f);
}
