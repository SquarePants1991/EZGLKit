
const float PackUpscale = 256. / 255.; // fraction -> 0..1 (including 1)
const float UnpackDownscale = 255. / 256.; // 0..1 -> fraction (excluding 1)

const vec3 PackFactors = vec3( 256. * 256. * 256., 256. * 256.,  256. );
const vec4 UnpackFactors = UnpackDownscale / vec4( PackFactors, 1. );

const float ShiftRight8 = 1. / 256.;

vec4 pack (float v)
{
    vec4 r = vec4( fract( v * PackFactors ), v );
    r.yzw -= r.xyz * ShiftRight8; // tidy overflow
    return r * PackUpscale;
}

float unpack (vec4 v)
{
    return dot( v, UnpackFactors );
}

vec4 renderAsShadow() {
    highp vec4 v_v4TexCoord = viewProjection * modelMatrix * fragPosition;
    highp vec3 lightPosition = lights[0].position;
    float normalizedDistance  = distance(v_v4TexCoord.xyz,lightPosition.xyz);
    return pack(v_v4TexCoord.z);
}