#version 330 core
#define Use_BumpMap

#include <frg_base.glsl>
#include <frg_projector.glsl>
#include <frg_light.glsl>
#include <frg_fog.glsl>
#include <frg_shadow.glsl>
#include <frg_terrain.glsl>

uniform sampler2D dudvMap;
uniform sampler2D reflectionMap;
uniform sampler2D refractionMap;
uniform float time;

vec4 waterColor(
                in vec3 normalVec,
                in vec3 vp,
                in vec3 eye
                ) {
    float kDistortion = 0.015;
    float kReflection = 0.02;
    vec4 lightTanSpace = normalize(vec4(vp,1.0));
    vec4 distOffset = tex2D(dudvMap,fragTexcoord + vec2(time / 50.0)) * kDistortion;

    vec4 normal = vec4(normalVec,0.51);
    vec4 lightReflection = normalize(reflect(-1.0 * lightTanSpace, normal));
    vec4 invertedFresnel = vec4(dot(normal, lightReflection));
    vec4 fresnelTerm = 1.0 - invertedFresnel;


    vec4 dudvColor = tex2D(dudvMap, vec2(fragTexcoord + distOffset.xy));
    dudvColor = normalize(dudvColor * 2.0 - 1.0) * kReflection;

    highp vec4 projCoord = viewProjection * modelMatrix * fragPosition;
    projCoord = projCoord / projCoord.w;
    projCoord = (projCoord + 1.0) / 2.0;
    projCoord += dudvColor;
    projCoord = clamp(projCoord,0.001,0.999);
    vec4 baseColor = vec4(0.2,0.2,0.2,1.0);


    vec4 reflectionColor = mix(tex2D(reflectionMap,projCoord.xy),baseColor,0.3);
    reflectionColor *= fresnelTerm;

    vec4 refractionColor = tex2D(refractionMap, projCoord.xy);
    vec4 depthValue = vec4(0.1,0.1,0.1,1.0);
    vec4 invDepth = 1.0 - depthValue;
    refractionColor *= invertedFresnel * invDepth;
    refractionColor += baseColor * depthValue * invertedFresnel;
    return reflectionColor + refractionColor;
}

vec4 render() {
    light defaultLight = lights[0];
    highp vec3 vp;
    if (defaultLight.type == LightType_Point) {
        vp = normalize(defaultLight.position - modelMatrixPosition);
    } else if (defaultLight.type == LightType_Direction) {
        vp = normalize(defaultLight.position);
    }
    #ifdef Use_BumpMap
        vp = normalize(tbnMatrix * vp);
    #endif
    return waterColor(surfaceNormal,vp,surfaceToEyeVec);
}

void main()
{
    outColor = caculateColorWithFrog(render());
}
