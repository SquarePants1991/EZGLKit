float frogFactor() {
    float start = 1000;
    float end = 1200;
    highp vec4 pos = viewProjection * modelMatrix * fragPosition;
    float distanceToCamera = distance(pos.xyz,vec3(0,0,0));
    float factor = clamp((end - distanceToCamera) / (end - start),0.0,1.0);
    factor = 1.0 - smoothstep(start,end,distanceToCamera);
    //factor = sin(factor * 3.14 / 2.0);
    return factor;
}
