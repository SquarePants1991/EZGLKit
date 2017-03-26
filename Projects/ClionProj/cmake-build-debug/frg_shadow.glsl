void caculateShadow(out float out_shadow) {
//    highp vec3 lightPosition = lights[0].position;
//    highp vec3 vp = normalize(lightPosition - mMatrixPosition);
//    float cosTheta = clamp(dot(normal, vp),0,1);
//    float bias = 0.005*tan(acos(cosTheta));
//    bias = clamp(bias, 0,0.01);
    float bias = 0.005;
    highp vec4 lightMVPPosition = lightViewProjection[0] * modelMatrix * fragPosition;
    lightMVPPosition = lightMVPPosition / lightMVPPosition.w;
    lightMVPPosition = lightMVPPosition * 0.5 + 0.5;
    float nearestDepth = tex2D(shadowMap[0], lightMVPPosition.st).x;
    float shadow = 0.0;
#ifndef ES
    vec2 texelSize = 1.0 / textureSize(shadowMap[0], 0);
    int blurSize = 1;
    for(int x = -blurSize; x <= blurSize; ++x)
    {
        for(int y = -blurSize; y <= blurSize; ++y)
        {
            float pcfDepth = tex2D(shadowMap[0], lightMVPPosition.xy + vec2(x, y) * texelSize).r;
            shadow += lightMVPPosition.z - bias > pcfDepth ? 0.8 : 0.0;
        }
    }
    shadow /= 9.0;
#endif

//    shadowColor = tex2D(shadowMap[0], fragTexcoord);
    out_shadow = 1.0 - shadow;
}
