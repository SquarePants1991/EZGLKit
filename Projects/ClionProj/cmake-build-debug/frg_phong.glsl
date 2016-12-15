#include <frg_base.glsl>
#include <frg_projector.glsl>
#include <frg_light.glsl>
#include <frg_normal.glsl>
#include <frg_fog.glsl>
#include <frg_shadow.glsl>
#include <frg_terrain.glsl>


#ifdef Use_BumpMap
mat3 tbnMatrix;
#endif
vec3 mMatrixPosition;

vec3 caculateNormal() {
 // 扰动法向量
    highp vec3 rgb = tex2D( normalMap, fragTexcoord ).rgb;
    highp float tnx = 2.0 * (rgb.r - 0.5);
    highp float tny = 2.0 * (rgb.g - 0.5);
    highp float tnz = 2.0 * (rgb.b - 0.5);
#ifdef Use_BumpMap
    highp vec3 textureNormal = normalize(vec3(tnx, tny, tnz));
 // 计算TBN变换矩阵
    highp vec3 tbnNormal, tbnTangent, tbnBitangent;
    transformNormal(fragPosition.xyz, fragNormal, tbnNormal);
    transformNormal(fragPosition.xyz, fragTangent, tbnTangent);
    transformNormal(fragPosition.xyz, fragBitangent, tbnBitangent);
    tbnMatrix = mat3(tbnTangent.x, tbnBitangent.x, tbnNormal.x,
                          tbnTangent.y, tbnBitangent.y, tbnNormal.y,
                          tbnTangent.z, tbnBitangent.z, tbnNormal.z);
#else
    highp vec3 textureNormal;
    transformNormal(fragPosition.xyz, fragNormal, textureNormal);
#endif
    return textureNormal;
}

vec3 caculateSurfaceToEyeVec() {
    // 计算表面点到摄像机的向量
    highp vec3 eye;
    eye = normalize(cameraPosition - mMatrixPosition);
#ifdef Use_BumpMap
    eye = normalize(tbnMatrix * eye);
#endif
    return eye;
}

void caculateLights(vec3 normal,vec3 eyeVec, out vec4 out_ambient, out vec4 out_diffuse, out vec4 out_specular) {
    highp vec4 sum_ambient = vec4(0.0,0.0,0.0,0.0), sum_diffuse = vec4(0.0,0.0,0.0,0.0), sum_specular = vec4(0.0,0.0,0.0,0.0);
    for (int i=0; i< lightNum; i++) {
        // 计算表面点到光源的向量
        light defaultLight = lights[i];
        highp vec3 vp = normalize(defaultLight.position - mMatrixPosition);
    #ifdef Use_BumpMap
        vp = normalize(tbnMatrix * vp);
    #endif

        // 计算表面到光源的位置
        float lightDistance = distance(defaultLight.position,mMatrixPosition);

        // 计算光源
        highp vec4 ambient, diffuse, specular;
    #ifdef Use_SpecularMap
        highp vec4 specularColor = tex2D(specularMap, fragTexcoord);
    #else
        highp vec4 specularColor = material.specular;
    #endif
        pointLight(normal,ambient,diffuse,specular,vp,eyeVec,lightDistance,material.ambient,defaultLight.color,vec4(1.0,1.0,1.0,1.0));

        sum_ambient = ambient;
        sum_diffuse = sum_diffuse + diffuse;
        sum_specular = sum_specular + specular;
    }
    out_ambient = sum_ambient;
    out_diffuse = sum_diffuse;
    out_specular = sum_specular;
}

bool caculateProjector(out vec4 out_ambient, out vec4 out_diffuse, out vec4 out_specular) {
    for(int i=0;i<projectorNum;i++) {
        out_ambient += project(projectors[i]);
        out_diffuse += project(projectors[i]);
    }
    return projectorNum > 0;
}

vec4 surfaceColor() {
    if (enableTerrainMix == 0) {
        return tex2D(diffuseMap, fragTexcoord) + material.diffuse;
    } else {
       return terrainColor();
    }
}

vec4 renderPass_border() {
    return borderColor;
}

vec4 renderPass_color() {
//            float r = sqrt(pow(fragTexcoord.x - 0.5, 2.0) + pow(fragTexcoord.y- 0.5, 2.0));
//            outColor = vec4(fragColor.rgb, fragColor.a * cos(r * 3.14));
    highp vec4 finalColor = tex2D(diffuseMap, fragTexcoord);
    finalColor = fragColor * finalColor.a;//vec4(().rgb,finalColor.a);
    return finalColor;
}

vec4 renderPass_shadow_frog_light() {
    mMatrixPosition = (modelMatrix * fragPosition).xyz;
    vec3 normal = caculateNormal();
    vec3 eye = caculateSurfaceToEyeVec();
    float shadow = shadowValue();

    highp vec4 ambient, diffuse, specular;
    caculateLights(normal, eye, ambient, diffuse, specular);

    highp vec4 projector_ambient, projector_diffuse, projector_specular;
    if (caculateProjector(projector_ambient, projector_diffuse, projector_specular)) {
        ambient = ambient * 0.5 + projector_ambient * 0.5;
        diffuse = diffuse * 0.5 + projector_diffuse * 0.5;
        specular = specular * 0.5 + projector_specular * 0.5;
    }

    highp vec4 color = surfaceColor();
    highp vec4 outputColor = (color * diffuse + color * ambient + color * specular) * shadow;
    return caculateColorWithFrog(outputColor);
}

//#define Use_BumpMap

void main()
{
    if (renderBorder == 1) {
        outColor = renderPass_border();
    } else if (onlyUseColorAttrib == 1) {
        outColor = renderPass_color();
    } else {
        outColor = renderPass_shadow_frog_light();
    }
}
