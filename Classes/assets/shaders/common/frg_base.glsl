#include <defines.glsl>
OUT highp vec4 fragPosition;
OUT lowp vec2 fragTexcoord;
OUT highp vec3 fragNormal;
OUT highp vec3 fragTangent;
OUT highp vec3 fragBitangent;
OUT highp vec4 fragColor;
#include <cm_uniforms.glsl>

// vars
#ifdef Use_BumpMap
mat3 tbnMatrix;
#endif
vec3 modelMatrixPosition = (modelMatrix * fragPosition).xyz;

#include <frg_camera.glsl>
#include <frg_normal.glsl>


vec3 surfaceNormal = caculateNormal();
vec3 surfaceToEyeVec = caculateSurfaceToEyeVec();
