// M V P
uniform highp vec3 cameraPosition;
uniform highp mat4 viewProjection;
uniform highp mat4 modelMatrix;
uniform highp mat3 normalMatrix;

uniform light lights[6];
uniform mediump int lightNum;
uniform sampler2D shadowMap[2];
uniform highp mat4 lightViewProjection[6];

uniform material_struct materials[2];

uniform int renderShadow;
uniform int renderBorder;
uniform vec4 borderColor;
uniform float borderWidth;

uniform int onlyUseColorAttrib;

uniform int isSky;

uniform int enableTerrainMix;

uniform mat4 additionMatrix;
uniform int useAdditionMatrix;

uniform float time;
