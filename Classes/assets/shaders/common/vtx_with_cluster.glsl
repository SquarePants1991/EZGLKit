#include <defines.glsl>
#include <vtx_uniforms.glsl>
#include <vtx_in_out.glsl>
#include <vtx_simple_pass.glsl>
#include <vtx_cluster.glsl>

void passValueToFragShader(vec4 positionIn, vec3 normalIn) {

    fragTexcoord = texcoord;
    fragNormal = normalIn;
    fragPosition = positionIn;// + vec4(normalize(fragNormal) * 1, 0.0);
    fragTangent = tangent;
    // TODO: why ???
    fragBitangent = bitangent;
    fragColor = color;
    fragMatID = matID;
}
