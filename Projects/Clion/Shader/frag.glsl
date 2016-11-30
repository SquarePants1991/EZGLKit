//
//  Shader.fsh
//  OpenESRD
//
//  Created by wangyang on 2016/9/30.
//  Copyright © 2016年 wangyang. All rights reserved.
#version 330 core

precision highp float;

#ifdef ES
#define OUT varying
#define outColor gl_FragColor
#define tex2D(map, uv) texture2D(map, uv)
#else
#define OUT in
out vec4 fragColor;
#define outColor fragColor
#define tex2D(map, uv) texture(map, uv)
#endif

out float fragmentdepth;

struct light {
    vec3 position;
    vec4 color;
    float intensity;
};

struct material_struct {
    vec4 diffuse;
    vec4 ambient;
    vec4 specular;
};

OUT highp vec4 fragPosition;
OUT lowp vec2 fragTexcoord;
OUT highp vec3 fragNormal;
OUT highp vec3 fragTangent;
OUT highp vec3 fragBitangent;

uniform highp mat4 viewProjection;
uniform highp mat4 lightViewProjection;
uniform highp mat4 modelMatrix;
uniform highp mat3 normalMatrix;

uniform light lights[3];
uniform mediump int lightNum;

uniform sampler2D ambientMap;
uniform sampler2D diffuseMap;
uniform sampler2D shadowMap;
uniform sampler2D normalMap;
uniform sampler2D specularMap;
uniform material_struct material;

uniform int renderShadow;

void pointLight(
                in vec3 normal,
                out vec4 ambient,
                out vec4 diffuse,
                out vec4 specular,
                in vec3 vp,
                in vec3 eye,
                in vec4 lightAmbient,
                in vec4 lightDiffuse,
                in vec4 lightSpecular
                ) {
    ambient = lightAmbient;
    vec3 halfVector = normalize(vp + eye);
    float shininess = 20.0;
    float nDotViewPosition = clamp(dot(normal,vp),0.0,1.0);
    diffuse = lightDiffuse * nDotViewPosition;
    float nDotViewHalfVector = clamp(dot(normal,halfVector),0.0,1.0);
    float powerFactor = max(0.0, pow(nDotViewHalfVector,shininess));
    specular = lightSpecular * powerFactor / 10.0;
}

void transformNormal(in vec3 position, in vec3 normal,out vec3 newNormal)
{
    highp vec3 normalTarget = position + normalize(normal);
    highp vec3 mMatrixNormalTarget = (modelMatrix * vec4(normalTarget, 1.0)).xyz;
    highp vec3 mMatrixPosition = (modelMatrix * vec4(position, 1.0)).xyz;
    newNormal = normalize(mMatrixNormalTarget - mMatrixPosition);
}

const float PackUpscale = 256. / 255.; // fraction -> 0..1 (including 1)
const float UnpackDownscale = 255. / 256.; // 0..1 -> fraction (excluding 1)

const vec3 PackFactors = vec3( 256. * 256. * 256., 256. * 256.,  256. );
const vec4 UnpackFactors = UnpackDownscale / vec4( PackFactors, 1. );

const float ShiftRight8 = 1. / 256.;

vec4 pack (float v)
{
    vec4 r = vec4( fract( v * PackFactors ), v );
    r.yzw -= r.xyz * ShiftRight8; // tidy overflow
    return r * PackUpscale;
}

float unpack (vec4 v)
{
    return dot( v, UnpackFactors );
}

vec4 renderAsShadow() {
    fragmentdepth = gl_FragCoord.z;
    highp vec4 v_v4TexCoord = viewProjection * modelMatrix * fragPosition;
    highp vec3 lightPosition = lights[0].position;
    float normalizedDistance  = distance(v_v4TexCoord.xyz,lightPosition.xyz);
    return pack(v_v4TexCoord.z);
}

float shadowValue(float bias,out vec4 shadowColor) {
    highp vec4 lightMVPPosition = lightViewProjection * modelMatrix * fragPosition;
    lightMVPPosition = lightMVPPosition / lightMVPPosition.w;
    lightMVPPosition = lightMVPPosition * 0.5 + 0.5;
    float nearestDepth = tex2D(shadowMap, lightMVPPosition.st).x;
    float shadow = 1.0;

    vec2 poissonDisk[4] = vec2[](
      vec2( -0.94201624, -0.39906216 ),
      vec2( 0.94558609, -0.76890725 ),
      vec2( -0.094184101, -0.92938870 ),
      vec2( 0.34495938, 0.29387760 )
    );

shadowColor = tex2D(shadowMap, fragTexcoord);//vec4(nearestDepth,nearestDepth,nearestDepth,1.0);
    for (int i=0;i<4;i++){
      if ( tex2D( shadowMap, lightMVPPosition.st  + poissonDisk[i]/20000.0).x  <  lightMVPPosition.z - bias) {
        shadow -= 0.15;
      }
    }
    return shadow;
}

vec4 render() {
 // 扰动法向量
    highp vec3 rgb = tex2D( normalMap, fragTexcoord ).rgb;
    highp float tnx = 2.0 * (rgb.r - 0.5);
    highp float tny = 2.0 * (rgb.g - 0.5);
    highp float tnz = 2.0 * (rgb.b - 0.5);
#ifdef Use_BumpMap
    highp vec3 textureNormal = normalize(vec3(tnx, tny, tnz));
#else
    highp vec3 textureNormal;
    transformNormal(fragPosition.xyz, fragNormal, textureNormal);
#endif

    // 计算TBN变换矩阵
    highp vec3 tbnNormal, tbnTangent, tbnBitangent;
    transformNormal(fragPosition.xyz, fragNormal, tbnNormal);
    transformNormal(fragPosition.xyz, fragTangent, tbnTangent);
    transformNormal(fragPosition.xyz, fragBitangent, tbnBitangent);
//    tbnBitangent = normalize(cross(tbnTangent, tbnNormal));
    highp mat3 tbn = mat3(tbnTangent.x, tbnBitangent.x, tbnNormal.x,
                          tbnTangent.y, tbnBitangent.y, tbnNormal.y,
                          tbnTangent.z, tbnBitangent.z, tbnNormal.z);

    highp vec3 mMatrixPosition = (modelMatrix * fragPosition).xyz;

    // 计算表面点到摄像机的向量
    highp vec3 eye;
    eye = normalize(vec3(0.0, 0.0, 10.0) - mMatrixPosition);
#ifdef Use_BumpMap
    eye = normalize(tbn * eye);
#endif

    highp vec4 sum_ambient = vec4(0.0,0.0,0.0,0.0), sum_diffuse = vec4(0.0,0.0,0.0,0.0), sum_specular = vec4(0.0,0.0,0.0,0.0);
    for (int i=0; i< lightNum; i++) {
        // 计算表面点到光源的向量
        light defaultLight = lights[i];
        highp vec3 vp = normalize(defaultLight.position - mMatrixPosition);
#ifdef Use_BumpMap
        vp = normalize(tbn * vp);
#endif

        // 计算光源
        highp vec4 ambient, diffuse, specular;
#ifdef Use_SpecularMap
        highp vec4 specularColor = tex2D(specularMap, fragTexcoord);
#else
        highp vec4 specularColor = material.specular;
#endif
        pointLight(textureNormal,ambient,diffuse,specular,vp,eye,material.ambient,defaultLight.color,vec4(1.0,1.0,1.0,1.0));

        sum_ambient = material.ambient;
        sum_diffuse = sum_diffuse + diffuse;
        sum_specular = sum_specular + specular;
    }

    highp vec3 lightPosition = lights[0].position;
    highp vec3 vp = normalize(lightPosition - mMatrixPosition);
    float cosTheta = clamp(dot(textureNormal, vp),0,1);
    float bias = 0.005*tan(acos(cosTheta));
    bias = clamp(bias, 0,0.01);

    vec4 shadowColor;
    float shadow = shadowValue(bias, shadowColor);
    return shadowColor;

    highp vec4 finalColor = tex2D(diffuseMap, fragTexcoord) + material.diffuse;
    return (finalColor * sum_diffuse + finalColor * sum_ambient + finalColor * sum_specular) * shadow;

}

//#define Use_BumpMap

void main()
{
    if (renderShadow == 1) {
        renderAsShadow();
    } else {
        outColor = render();
    }
}
