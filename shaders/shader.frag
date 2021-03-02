#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 pos;

layout(location = 0) out vec4 outColor;



vec2 cx_mul(vec2 a, vec2 b) {
    return vec2(a.x*b.x - a.y*b.y, a.x*b.y + a.y*b.x);
}

vec2 cx_sqr(vec2 a) {
    float x2 = a.x*a.x;
    float y2 = a.y*a.y;
    float xy = a.x*a.y;
    return vec2(x2 - y2, xy + xy);
}

void main() {
    //vec2 c = 0.05f*vec2(pos.x*2.0f,pos.y)+vec2(-1.1f,0.23f);
    vec2 c = 0.001f*vec2(pos.x*2.0f,pos.y)+vec2(-1.11f,0.231f);
    vec2 z = vec2(0.0f);
    float limit = 0.0f;
    float maxIter = 1500.0f;

    for(int i=0; i<maxIter; i++){
        z = cx_sqr(z) + c;
        if(dot(z,z)>100){
            break;
        }
        limit = limit + 1.0f;
    }

    //outColor = vec4(0.0f,0.0f , 0.0f, 1.0f);

    if(limit==maxIter){
        outColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    } else {
        outColor = vec4((limit+10.0f)/maxIter, (1.1f*limit/maxIter), (2*limit/maxIter), 1.0f);
        outColor = pow(outColor, vec4(1.5f));
    }
}
