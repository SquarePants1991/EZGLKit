uniform int receiveShadow;

float LinearizeDepth(float depth)
{
    float near_plane = 0.1;
    float far_plane = 1000;
    float z = depth * 2.0 - 1.0; // Back to NDC
    float ret = (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
    return ret / far_plane;
}

void caculateShadow(out float out_shadow, out vec4 shadowColor) {
    if (receiveShadow == 0) {
        out_shadow = 1.0;
        return;
    }
    light defaultLight = lights[0];

    highp vec4 lightMVPPosition = lightViewProjection[0] * modelMatrix * fragPosition;
    lightMVPPosition = lightMVPPosition / lightMVPPosition.w;
    lightMVPPosition = lightMVPPosition * 0.5 + 0.5;

    float depth = tex2D(shadowMap[0], lightMVPPosition.st).x;
    float currentDepth = lightMVPPosition.z;

    highp vec3 vp;

    if (defaultLight.type == LightType_Point) {
       vp = normalize(defaultLight.position - modelMatrixPosition);
       currentDepth = LinearizeDepth(currentDepth);
       depth = LinearizeDepth(depth);
    } else if (defaultLight.type == LightType_Direction) {
       vp = normalize(defaultLight.position);
    }

    float bias = 0.005*tan(acos(dot(fragNormal, -vp)));
    bias = clamp(bias, 0.0, 0.02);

    float nearestDepth = depth;
    float shadow = 0.0;
    if (currentDepth > 1.0) {
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
            if (defaultLight.type == LightType_Point) {
                pcfDepth = LinearizeDepth(pcfDepth);
            }
            shadow += currentDepth - bias > pcfDepth ? 0.6 : 0.0;
        }
    }
    shadow /= 9.0;
//#endif
//    shadowColor = vec4(vec3(depth), 1.0);
    out_shadow = 1.0 - shadow;
}
