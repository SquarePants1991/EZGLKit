uniform int receiveShadow;

void caculateShadow(out float out_shadow, out vec4 shadowColor) {
    if (receiveShadow == 0) {
        out_shadow = 1.0;
        return;
    }
    light defaultLight = lights[0];
    highp vec3 vp;
    if (defaultLight.type == LightType_Point) {
       vp = normalize(defaultLight.position - modelMatrixPosition);
    } else if (defaultLight.type == LightType_Direction) {
       vp = normalize(defaultLight.position);
    }
    float bias = 0.005*tan(acos(dot(fragNormal, -vp)));
    bias = clamp(bias, 0.005, 0.02);
    highp vec4 lightMVPPosition = lightViewProjection[0] * modelMatrix * fragPosition;
    lightMVPPosition = lightMVPPosition / lightMVPPosition.w;
    lightMVPPosition = lightMVPPosition * 0.5 + 0.5;
    float nearestDepth = tex2D(shadowMap[0], lightMVPPosition.st).x;
    float shadow = 0.0;
    if (lightMVPPosition.z > 1.0) {
        out_shadow = 1.0;
        return;
    }
//#ifndef ES
    // 由于OpenGLES2不支持textureSize，所以暂时写死
    vec2 texelSize = 1.0 / vec2(1024.0, 1024.0);
    int blurSize = 1;
    for(int x = -blurSize; x <= blurSize; ++x)
    {
        for(int y = -blurSize; y <= blurSize; ++y)
        {
            float pcfDepth = tex2D(shadowMap[0], lightMVPPosition.xy + vec2(x, y) * texelSize).r;
            shadow += lightMVPPosition.z - bias > pcfDepth ? 0.6 : 0.0;
        }
    }
    shadow /= 9.0;
//#endif
    shadowColor = tex2D(shadowMap[0], lightMVPPosition.xy);
    out_shadow = 1.0 - shadow;
}
