#version 330 core

#include <vtx_base.glsl>
#include <vtx_cluster.glsl>
#include <vtx_clip_plane.glsl>
#include <vtx_border_pass.glsl>

void main()
{
    vec4 pos = processCluster(position);
    passValueToFragShader(pos);
    clipPlane(pos);

    borderVertexPass(pos);
    simpleVertexPass(pos);
}
