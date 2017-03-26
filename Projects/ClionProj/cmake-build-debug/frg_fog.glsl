uniform float frogStart;
uniform float frogEnd;
uniform vec4 frogColor;

float frogFactor(float start, float end) {
    highp vec4 pos = viewProjection * modelMatrix * fragPosition;
    float distanceToCamera = distance(pos.xyz,vec3(0,0,0));
    float factor = clamp((end - distanceToCamera) / (end - start),0.0,1.0);
    factor = 1.0 - smoothstep(start,end,distanceToCamera);
    return factor;
}

vec4 caculateColorWithFrog(vec4 originColor) {
    float factor = frogFactor(frogStart, frogEnd);
    return vec4((originColor * factor + frogColor * (1.0 - factor)).rgb,1.0);
}
