// M V P
uniform highp vec3 cameraPosition;
uniform highp mat4 viewProjection;
uniform highp mat4 modelMatrix;
uniform highp mat3 normalMatrix;

uniform light lights[6];
uniform mediump int lightNum;
uniform sampler2D shadowMap[6];
uniform highp mat4 lightViewProjection[6];

uniform sampler2D ambientMap;
uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D specularMap;

uniform material_struct material;

uniform int renderShadow;
uniform int renderBorder;
uniform vec4 borderColor;
uniform float borderWidth;

uniform int onlyUseColorAttrib;


uniform int enableTerrainMix;

uniform mat4 additionMatrix;
uniform int useAdditionMatrix;