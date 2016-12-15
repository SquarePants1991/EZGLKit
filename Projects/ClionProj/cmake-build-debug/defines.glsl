#version 330 core

precision highp float;

#ifdef ES
    #define OUT varying
    #define VertIN attribute
    #define VertOUT varying
    #define outColor gl_FragColor
    #define tex2D(map, uv) texture2D(map, uv)
#else
    #define OUT in
    #define VertIN in
    #define VertOUT out
    out vec4 GLfragColor;
    #define outColor GLfragColor
    #define tex2D(map, uv) texture(map, uv)
#endif

out float outDepth;

struct light {
    vec3 position;
    vec4 color;
    float intensity;
    float intensityFallOff;
};

struct material_struct {
    vec4 diffuse;
    vec4 ambient;
    vec4 specular;
};

struct projector_struct {
    mat4 viewProjection;
};