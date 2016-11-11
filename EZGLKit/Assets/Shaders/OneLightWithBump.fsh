//
//  Shader.fsh
//  OpenESRD
//
//  Created by wangyang on 2016/9/30.
//  Copyright © 2016年 wangyang. All rights reserved.
//

precision highp float;

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

varying highp vec4 fragPosition;
varying lowp vec2 fragTexcoord;
varying highp vec3 fragNormal;
varying highp vec3 fragTangent;
varying highp vec3 fragBitangent;

uniform highp mat4 viewProjection;
uniform highp mat4 modelMatrix;
uniform highp mat3 normalMatrix;

uniform light lights[1];

uniform sampler2D ambientMap;
uniform sampler2D diffuseMap;
uniform sampler2D shadowMap;
uniform sampler2D normalMap;
uniform sampler2D specularMap;
uniform material_struct material;

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
    float shininess = 50.0;
    float nDotViewPosition = clamp(dot(normal,vp),0.0,1.0);
    diffuse = lightDiffuse * nDotViewPosition;
    float nDotViewHalfVector = clamp(dot(normal,halfVector),0.0,1.0);
    float powerFactor = max(0.0, pow(nDotViewHalfVector,shininess));
    specular = lightSpecular * powerFactor;
}

void transformNormal(in vec3 position, in vec3 normal,out vec3 newNormal)
{
    highp vec3 normalTarget = position + normalize(normal);
    highp vec3 mMatrixNormalTarget = (modelMatrix * vec4(normalTarget, 1.0)).xyz;
    highp vec3 mMatrixPosition = (modelMatrix * vec4(position, 1.0)).xyz;
    newNormal = normalize(mMatrixNormalTarget - mMatrixPosition);
}

void main()
{
    // 扰动法向量
    highp vec3 rgb = texture2D( normalMap, fragTexcoord ).rgb;
    highp float tnx = 2.0 * (rgb.r - 0.5);
    highp float tny = 2.0 * (rgb.g - 0.5);
    highp float tnz = 2.0 * (rgb.b - 0.5);
    highp vec3 textureNormal = normalize(vec3(tnx, tny, tnz));
//highp vec3 textureNormal = normalize(vec3(0.0, 0.0, 1.0));
    //    highp vec3 textureNormal;
//    transformNormal(fragPosition.xyz, fragNormal, textureNormal);
    
    // 计算TBN变换矩阵
    highp vec3 tbnNormal, tbnTangent, tbnBitangent;
    transformNormal(fragPosition.xyz, fragNormal, tbnNormal);
    transformNormal(fragPosition.xyz, fragTangent, tbnTangent);
    transformNormal(fragPosition.xyz, fragBitangent, tbnBitangent);
//    tbnBitangent = normalize(cross(tbnTangent, tbnNormal));
    highp mat3 tbn = mat3(tbnTangent.x, tbnBitangent.x, tbnNormal.x,
                          tbnTangent.y, tbnBitangent.y, tbnNormal.y,
                          tbnTangent.z, tbnBitangent.z, tbnNormal.z);

    // 计算表面点到光源的向量
    light defaultLight = lights[0];
    highp vec3 mMatrixPosition = (modelMatrix * fragPosition).xyz;
    highp vec3 vp = normalize(defaultLight.position - mMatrixPosition);
    vp = normalize(tbn * vp);

    // 计算表面点到摄像机的向量
    
    highp vec3 eye;
    eye = normalize(vec3(0.0, 0.0, 10.0) - mMatrixPosition);
    eye = normalize(tbn * eye);
    
    // 计算光源
    highp vec4 ambient, diffuse, specular;
    highp vec4 specularColor = texture2D(specularMap, fragTexcoord);
    pointLight(textureNormal,ambient,diffuse,specular,vp,eye,material.ambient,defaultLight.color,vec4(1.0,1.0,1.0,1.0));
    
    highp vec4 finalColor = texture2D(diffuseMap, fragTexcoord);
    gl_FragColor = finalColor * diffuse + finalColor * ambient + finalColor * specular;
    
//    gl_FragColor = vec4(fragBitangent,1.0);
}
