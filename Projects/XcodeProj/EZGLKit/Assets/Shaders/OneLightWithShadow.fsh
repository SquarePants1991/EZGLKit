//
//  Shader.fsh
//  OpenESRD
//
//  Created by wangyang on 2016/9/30.
//  Copyright © 2016年 wangyang. All rights reserved.
//

precision highp float;

varying highp vec4 frag_position;
varying highp vec3 frag_normal;
varying lowp vec2 frag_uv;

uniform sampler2D diffuseMap;
uniform sampler2D shadowMap;

uniform highp mat4 lightViewProjection;
uniform highp mat4 viewProjection;
uniform highp mat4 modelMatrix;
uniform highp mat3 normalMatrix;

uniform highp vec4 ambient;
uniform highp vec4 diffuse;
uniform highp vec4 specular;

uniform highp vec4 lightColor;
uniform highp vec3 lightPosition;
uniform highp float lightBrightness;
uniform lowp int renderAsShadow;

vec4 pack (float dis)
{
    float zsbf = floor(dis);
    float xsbf = fract(dis);
    xsbf = floor(xsbf * 1024.0);
    float hzsbf = floor(zsbf / 256.0);
    float lzsbf = mod(zsbf,256.0);
    float hxsbf = floor(xsbf/32.0);
    float lxsbf = mod(xsbf,32.0);
    float r = hzsbf / 256.0;
    float g = lzsbf / 256.0;
    float b = hxsbf / 32.0;
    float a = lxsbf / 32.0;
    return vec4(r,g,b,a);
}

float unpack (vec4 color)
{
    return color.r * 256.0 * 256.0 + color.g * 256.0 + color.b + color.a / 32.0;
}

void main()
{
    const highp mat4 biasMat = mat4(0.5, 0.0, 0.0, 0.0,
                              0.0, 0.5, 0.0, 0.0,
                              0.0, 0.0, 1.0, 0.0,
                              0.5, 0.5, 0.0, 1.0);
    
    if ( renderAsShadow > 0 ) {
        highp vec4 v_v4TexCoord =  modelMatrix * frag_position;
        float normalizedDistance  = distance(frag_position.xyz,lightPosition.xyz) ;//v_v4TexCoord.z / v_v4TexCoord.w;
//        normalizedDistance = (normalizedDistance + 1.0) / 2.0;
        gl_FragColor = pack(normalizedDistance);
    } else {
        highp vec4 v_v4TexCoord = lightViewProjection * modelMatrix * frag_position;
        highp vec4 shadowMapPosition = v_v4TexCoord / v_v4TexCoord.w;
        float s = (shadowMapPosition.s + 1.0) /2.0;
        float t = (shadowMapPosition.t + 1.0) /2.0;
        float distanceFromLight = unpack(texture2D(shadowMap, vec2(s,t)));
        float currentDistance = distance(frag_position.xyz,lightPosition.xyz);
        
        float shadow = 1.0;
        if (s >= 0.0 && s <=1.0 &&
            t >= 0.0 && t <=1.0 ) {
            if ( distanceFromLight <= currentDistance - 2.8 ) {
                shadow = 0.5;
            }
        }
        
        float bias = 0.0005;
        
        //1.0 = not in shadow (fragmant is closer to light than the value stored in shadow map)
        //0.0 = in shadow
//        float shadowSample = float(distanceFromLight > shadowMapPosition.z);
//        float shadow = 1.0;
//        if (v_v4TexCoord.w > 0.0) {
//            shadow = shadowSample;
//            shadow = shadow * 0.8 + 0.2;
//        }
        
        
        
        highp vec3 eyePosition = vec3(viewProjection * modelMatrix * frag_position);
        highp vec3 eyeNormal = normalize(normalMatrix * frag_normal);
        highp vec3 eyeLightVec = vec3(viewProjection * vec4(lightPosition,1.0)) - vec3(eyePosition);
        highp float eyeLightDistance = distance(eyeLightVec,vec3(0,0,0));
        
        highp float brightness = max(0.0, 1000.0 * lightBrightness * dot(eyeNormal, normalize(eyeLightVec)));
        
        
        highp vec4 surfaceColor = texture2D( diffuseMap,frag_uv ) + diffuse;
        highp vec3 ambientColor = ambient.rgb * surfaceColor.rgb;
        
        highp vec3 eyeReverseVec = normalize(vec3(0.0,0.0,0.0) - eyePosition);
        highp vec3 reflectLightVec = reflect(-normalize(eyeLightVec),eyeNormal);
        highp float cosAlpha = max(0.0 , dot( eyeReverseVec, reflectLightVec ));
        highp vec3 specularColor = specular.rgb * lightColor.rgb * pow(cosAlpha,2.0) / pow(eyeLightDistance, 2.0);
        
        highp vec3 finalColor = lightColor.rgb * surfaceColor.rgb * brightness / pow(eyeLightDistance, 2.0) + ambientColor + specularColor;
        gl_FragColor = vec4(finalColor * shadow,surfaceColor.a);
    }
}
