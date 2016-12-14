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
varying vec3 fragNormal;

uniform highp mat4 viewProjection;
uniform highp mat4 modelMatrix;
uniform highp mat3 normalMatrix;

uniform light lights[1];

uniform sampler2D diffuseMap;
uniform material_struct material;

void main()
{
    light defaultLight = lights[0];
    
    highp vec3 eyePosition = vec3(viewProjection * modelMatrix * fragPosition);
    highp vec3 eyeNormal = normalize(normalMatrix * fragNormal);
    highp vec3 eyeLightVec = vec3(viewProjection * vec4(defaultLight.position,1.0)) - vec3(eyePosition);
    highp float eyeLightDistance = distance(eyeLightVec,vec3(0,0,0));
    
    highp float brightness = max(0.0, defaultLight.intensity * dot(eyeNormal, normalize(eyeLightVec)));

    
    highp vec4 surfaceColor = texture2D(diffuseMap, fragTexcoord) + material.diffuse;
    highp vec3 ambientColor = material.ambient.rgb * surfaceColor.rgb;

    highp vec3 eyeReverseVec = normalize(vec3(0.0,0.0,0.0) - eyePosition);
    highp vec3 reflectLightVec = reflect(-normalize(eyeLightVec),eyeNormal);
    highp float cosAlpha = max(0.0 , dot( eyeReverseVec, reflectLightVec ));
    highp vec3 specularColor = material.specular.rgb * defaultLight.color.rgb * pow(cosAlpha,2.0) / pow(eyeLightDistance, 2.0);
    
    highp vec3 finalColor = defaultLight.color.rgb * surfaceColor.rgb * brightness / pow(eyeLightDistance, 2.0) + ambientColor + specularColor;
    gl_FragColor = vec4(finalColor, surfaceColor.a);
}
