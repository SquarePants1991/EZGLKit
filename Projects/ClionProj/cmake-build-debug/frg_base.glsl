#include <defines.glsl>
OUT highp vec4 fragPosition;
OUT lowp vec2 fragTexcoord;
OUT highp vec3 fragNormal;
OUT highp vec3 fragTangent;
OUT highp vec3 fragBitangent;
OUT highp vec4 fragColor;
OUT float fragMatID;
#include <cm_uniforms.glsl>

// vars
#ifdef Use_BumpMap
bool useBumpMap;
mat3 tbnMatrix;
#endif
vec3 modelMatrixPosition;

#include <frg_camera.glsl>
#include <frg_normal.glsl>


vec3 surfaceNormal;
vec3 surfaceToEyeVec;

void frag_base() {
    modelMatrixPosition = (modelMatrix * fragPosition).xyz;
    surfaceNormal = caculateNormal();
    surfaceToEyeVec = caculateSurfaceToEyeVec();
}

