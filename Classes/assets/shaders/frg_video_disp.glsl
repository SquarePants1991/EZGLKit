#define Use_BumpMap
#define ES

#include <frg_base.glsl>
#include <frg_projector.glsl>
#include <frg_light.glsl>
#include <frg_fog.glsl>
#include <frg_shadow.glsl>
#include <frg_terrain.glsl>

void main()
{
    frag_base();
    int matId = int(fragMatID);
    vec4 Y_planeColor = tex2D(materials[matId].diffuseMap, fragTexcoord);
    vec4 CbCr_planeColor = tex2D(materials[matId].normalMap, fragTexcoord);
    
    float Cb, Cr, Y;
    float R ,G, B;
    Y = Y_planeColor.r * 255.0;
    Cb = CbCr_planeColor.r * 255.0 - 128.0;
    Cr = CbCr_planeColor.a * 255.0 - 128.0;
    
    R = 1.402 * Cr + Y;
    G = -0.344 * Cb - 0.714 * Cr + Y;
    B = 1.772 * Cb + Y;
    
    
    vec4 videoColor = vec4(R / 255.0, G / 255.0, B / 255.0, 1.0);
    vec4 featureColor = tex2D(materials[matId].specularMap, fragTexcoord);
    if (featureColor.a == 1.0) {
        outColor = videoColor;
    } else {
        outColor = videoColor;
    }
}
