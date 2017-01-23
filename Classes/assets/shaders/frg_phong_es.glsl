#define Use_BumpMap
#define ES

#include <frg_base.glsl>
#include <frg_projector.glsl>
#include <frg_light.glsl>
#include <frg_fog.glsl>
#include <frg_shadow.glsl>
#include <frg_terrain.glsl>

vec4 surfaceColor() {
    if (enableTerrainMix == 0) {
        int matId = int(fragMatID);
        return tex2D(materials[matId].diffuseMap, fragTexcoord) + materials[matId].diffuse;
    } else {
       return terrainColor();
    }
}

vec4 renderPass_border() {
    return borderColor;
}

vec4 renderPass_color() {
    highp vec4 finalColor = tex2D(diffuseMap, fragTexcoord);
    finalColor = fragColor * finalColor.a;
    return finalColor;
}

vec4 renderPass_textureonly() {
    return tex2D(diffuseMap, fragTexcoord);
}

vec4 renderPass_shadow_frog_light() {
    float shadow;
    caculateShadow(shadow);

    highp vec4 ambient, diffuse, specular;
    caculateLights(surfaceNormal, surfaceToEyeVec, ambient, diffuse, specular);

    highp vec4 projector_ambient, projector_diffuse, projector_specular;
    if (caculateProjector(projector_ambient, projector_diffuse, projector_specular)) {
        diffuse = vec4((diffuse * 0.0 + projector_diffuse * 1.0).rgb, diffuse.a);
    }
    
//    return tex2D(normalMap, fragTexcoord);
    
    highp vec4 color = surfaceColor();
    highp vec4 outputColor = (color * diffuse + color * ambient + color * specular) * shadow;

    return caculateColorWithFrog(outputColor);
}

void main()
{
    frag_base();
    if (renderBorder == 1) {
        outColor = renderPass_border();
    } else if (onlyUseColorAttrib == 1) {
        outColor = renderPass_color();
    } else if (isSky == 1) {
        outColor = renderPass_textureonly();
    } else {
        outColor = renderPass_shadow_frog_light();
    }
}
