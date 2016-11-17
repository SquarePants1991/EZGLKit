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

uniform light lights[3];
uniform mediump int lightNum;

uniform highp vec3 cameraPosition;

uniform sampler2D ambientMap;
uniform sampler2D diffuseMap;
uniform sampler2D shadowMap;
uniform sampler2D normalMap;
uniform sampler2D specularMap;
uniform material_struct material;

float toon(in float val) {
    if (val < 0.1) {
        return 0.0;
    } else if (val < 0.9) {
        return 0.5;
    } else {
        return 1.0;
    }
    return 0.0;
}

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
    float nDotViewPosition = toon(clamp(dot(normal,vp),0.0,1.0));
    diffuse = lightDiffuse * nDotViewPosition;
    float nDotViewHalfVector = toon(clamp(dot(normal,halfVector),0.0,1.0));
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

void main()
{
    // 扰动法向量
    highp vec3 rgb = texture2D( normalMap, fragTexcoord ).rgb;
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
    eye = normalize(cameraPosition - mMatrixPosition);
#ifdef Use_BumpMap
    eye = normalize(tbn * eye);
#endif
    
    if (dot(textureNormal, normalize(eye)) < 0.2) {
        gl_FragColor = vec4(0.0,0.0,0.0,0.0);
        return;
    }
    
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
        highp vec4 specularColor = texture2D(specularMap, fragTexcoord);
#else
        highp vec4 specularColor = material.specular;
#endif
        pointLight(textureNormal,ambient,diffuse,specular,vp,eye,material.ambient,defaultLight.color,vec4(1.0,1.0,1.0,1.0));
        
        sum_ambient = material.ambient;
        sum_diffuse = sum_diffuse + diffuse;
        sum_specular = sum_specular + specular;
    }
    
    highp vec4 finalColor = texture2D(diffuseMap, fragTexcoord);
    gl_FragColor = finalColor * sum_diffuse + finalColor * sum_ambient + finalColor * sum_specular;
}
