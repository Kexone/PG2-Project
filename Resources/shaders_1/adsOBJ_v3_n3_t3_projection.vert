#version 400 core //compatibility

struct LightInfo {
	vec4 position;			//Light position   X,Y,Z, directional=0 OR positional=1
	vec4 ambient;			//Light ambient color
	vec4 diffuse;			//Light diffuse color
	vec4 specular;			//Light specular color
	vec4 spotDirection;		//Specifies light direction
	int spotExponent;		//Specifies intensity distribution of spotlighit
	float spotCutoff;			//Specifies maximum spread angle of spotlight (180 = off).
};

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec3 VertexTex;
layout(location = 3) in vec2 VertexCoords;

uniform mat4 PMatrix;		//Camera projection matrix
uniform mat4 VMatrix;		//Camera view matrix
uniform mat4 MVMatrix;		//VMatrix * Model matrix
uniform mat4 MMatrix;		// Matrix Model
uniform mat3 NormalMatrix;	//MVMatrix ... -> converted into normal matrix (inverse transpose operation)
uniform mat4 TMatrix;       //TMatrix



uniform LightInfo light;

//The prefix ec means Eye Coordinates in the Eye Coordinate System
out vec4 ecPosition;			
out vec3 ecLightDir;
out vec3 ecNormal;
out vec3 ecViewDir;
out vec2 ecTextCoord;

void main()
{
	//finalCoord = projectCoord.st /projectCoord.q;
	ecPosition = MVMatrix * vec4(VertexPosition,1);
	ecLightDir = vec3((VMatrix * light.position)- ecPosition);
	ecNormal = NormalMatrix * VertexNormal;
	ecViewDir = -vec3(ecPosition);
	//ecTextCoord = VertexCoords;
	vec4 projectVec = TMatrix * MMatrix  *vec4(VertexPosition, 1.0f);
	ecTextCoord = projectVec.st /projectVec.q;
	gl_Position = PMatrix * ecPosition;

	
}
