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

//struct VS_out{};

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec3 VertexTexCoords;
layout(location = 3) in vec3 VertexTSTangent;
layout(location = 4) in vec3 VertexTSBitangent;
layout(location = 5) in vec3 VertexTSNormal;

uniform mat4 PMatrix;		//Camera projection matrix
uniform mat4 VMatrix;		//Camera view matrix
uniform mat4 MVMatrix;		//VMatrix * Model matrix
uniform mat3 NormalMatrix;	//MVMatrix ... -> converted into normal matrix (inverse transpose operation)
uniform mat4 TMatrix;
uniform mat4 MMatrix;

uniform LightInfo light;
uniform MaterialInfo material;

//The prefix ec means Eye Coordinates in the Eye Coordinate System
/*
out vec4 ecPosition;
out vec3 ecLightDir;
out vec3 ecNormal;
out vec3 ecViewDir;
out vec2 ecTexCoords;
out float texMixFactor;
out vec3 TsT;
out vec3 TsB;
out vec3 TsN;
*/

out VS_out{

 vec3 ecLightDir;
 vec3 ecNormal;
 vec3 ecViewDir;
 vec2 ecTexCoords;

}vs_out;

void main()
{	
 vec3 TsT;
 vec3 TsB;
 vec3 TsN;


	vs_out.ecTexCoords = VertexTexCoords.xy;

	vec4 Position = MVMatrix * vec4(VertexPosition, 1.0);
	vec3 LightDir = vec3( (VMatrix * light.position) - Position);
	vec3 Normal = NormalMatrix * VertexNormal;
	vec3 ViewDir = -vec3(Position);
	
   
	vec3 T = vec3(MVMatrix * vec4(VertexTSTangent,0));
	vec3 B = vec3(MVMatrix * vec4(VertexTSBitangent,0));
	vec3 N = vec3(MVMatrix * vec4(VertexTSNormal,0));



	vs_out.ecLightDir = vec3(dot(T, LightDir),dot(B,LightDir),dot(N,LightDir));
	vs_out.ecViewDir = vec3(dot(T, ViewDir),dot(B,ViewDir),dot(N,ViewDir));
	vs_out.ecNormal = vec3(dot(T, Normal),dot(B,Normal),dot(N,Normal));

	gl_Position = PMatrix * Position;

}