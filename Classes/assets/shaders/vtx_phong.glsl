#version 330 core

#include <vtx_with_cluster.glsl>
#include <vtx_clip_plane.glsl>
#include <vtx_border_pass.glsl>

void main()
{
    vec4 positionOut;
    vec3 normalOut;
    processCluster(position, normal, positionOut, normalOut);
    passValueToFragShader(positionOut, normalOut);
    clipPlane(positionOut);

    borderVertexPass(positionOut);
    simpleVertexPass(positionOut);
}
