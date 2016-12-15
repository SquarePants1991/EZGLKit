#include <vtx_base.glsl>
#include <vtx_clip_plane.glsl>
#include <vtx_border_pass.glsl>

void main()
{
    passValueToFragShader();
    clipPlane();

    borderVertexPass();
    simpleVertexPass();
}