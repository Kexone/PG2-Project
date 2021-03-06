#version 400 core //compatibility

struct LightInfo {
	vec4 position;			//Light position   X,Y,Z, directional=0 OR positional=1
	vec4 ambient;			//Light ambient color
	vec4 diffuse;			//Light diffuse color
	vec4 specular;			//Light specular color
	vec4 spotDirection;		//Specifies light direction
	int spotExponent;		//Specifies intensity distribution of spotlighit
	float spotCutoff;		//Specifies maximum spread angle of spotlight (180 = off).
};

struct MaterialInfo {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;	
	float transparency;
	float shininess;
	int illumination;
	int hasDiffuseTexture;
};

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec3 VertexTexCoords;
layout(location = 3) in vec3 VertexTStangent;
layout(location = 4) in vec3 VertexTSbitangent;
layout(location = 5) in vec3 VertexTSnormal;

uniform mat4 PMatrix;		//Camera projection matrix
uniform mat4 VMatrix;		//Camera view matrix
uniform mat4 MMatrix;		//Model matrix
uniform mat4 MVMatrix;		//VMatrix * Model matrix
uniform mat3 NormalMatrix;	//MVMatrix ... -> converted into normal matrix (inverse transpose operation)
uniform mat3 MNormalMatrix;	//MMatrix ... -> converted into normal matrix (inverse transpose operation)
uniform mat4 TMatrix;

uniform LightInfo light;
uniform MaterialInfo material;

//The prefix ec means Eye Coordinates in the Eye Coordinate System
out vec4 ecPosition;
out vec3 ecLightDir;
out vec3 ecNormal;
out vec3 ecViewDir;

// Tangent space
out VS_OUT {
	vec3 tsLightDir;
	vec3 tsNormal;
	vec3 tsViewDir;
	vec2 tsTexCoords;
} vs_out;

void main()
{
	ecPosition = MVMatrix * vec4(VertexPosition, 1.0);
	ecLightDir = vec3( (VMatrix * light.position) - ecPosition);
	ecNormal = NormalMatrix * VertexNormal;
	ecViewDir = -vec3(ecPosition);

	gl_Position = PMatrix * ecPosition;
}