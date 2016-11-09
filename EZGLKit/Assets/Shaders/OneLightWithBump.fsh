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

uniform sampler2D diffuseMap;
uniform sampler2D shadowMap;
uniform sampler2D normalMap;
uniform material_struct material;

void main()
{
    light defaultLight = lights[0];
    
    highp vec3 eyePosition = vec3(viewProjection * modelMatrix * fragPosition);
    
    highp vec3 tbnNormal = normalMatrix * normalize(fragNormal);
    highp vec3 tbnTangent = normalMatrix * normalize(fragTangent);
    highp vec3 tbnBitangent = normalMatrix * normalize(fragBitangent);
    highp mat3 tbn = (mat3(tbnTangent,tbnBitangent,tbnNormal));
    
//    highp vec3 eyeLightVec = tbn * vec3(viewProjection * vec4(defaultLight.position,1.0) - vec3(eyePosition));
    highp vec3 eyeLightVec = tbn * vec3(viewProjection * vec4(defaultLight.position,1.0)) - tbn * vec3(eyePosition);
    highp float eyeLightDistance = distance(eyeLightVec,vec3(0,0,0));
    
    highp vec3 rgb = texture2D( normalMap, fragTexcoord ).rgb;
    highp float tnx = rgb.r * 2.0 - 1.0;
    highp float tny = rgb.g * 2.0 - 1.0;
    highp float tnz = rgb.b * 2.0 - 1.0;
    
    highp vec3 textureNormal = normalize(vec3(tnx, tny, tnz));
    
    highp float brightness = max(0.0, defaultLight.intensity * dot(textureNormal, normalize(eyeLightVec)));

    
    highp vec4 surfaceColor = texture2D(diffuseMap, fragTexcoord);// + material.diffuse;
    highp vec3 ambientColor = material.ambient.rgb * surfaceColor.rgb;

    highp vec3 eyeReverseVec = tbn * normalize(vec3(0.0,0.0,0.0) - eyePosition);
    highp vec3 reflectLightVec = reflect(-normalize(eyeLightVec),textureNormal);
    highp float cosAlpha = max(0.0 , dot( eyeReverseVec, reflectLightVec ));
    highp vec3 specularColor = material.specular.rgb * defaultLight.color.rgb * pow(cosAlpha,2.0) / pow(eyeLightDistance, 2.0);
    
    highp vec3 finalColor = defaultLight.color.rgb * surfaceColor.rgb * brightness + ambientColor;// / pow(eyeLightDistance, 2.0);// + ambientColor + specularColor;
    gl_FragColor = vec4(finalColor, surfaceColor.a);
}
