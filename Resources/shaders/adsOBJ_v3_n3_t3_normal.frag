#version 400 core //compatibility

struct LightInfo {
	vec4 position;			//Light position   X,Y,Z, directional=0 OR positional=1
	vec4 ambient;			//Light ambient color
	vec4 diffuse;			//Light diffuse color
	vec4 specular;			//Light specular color
	vec4 spotDirection;		//Specifies light direction
	int spotExponent;		//Specifies intensity distribution of spotlight
	float spotCutoff;			//Specifies maximum spread angle of spotlight (180 = off).
};

struct MaterialInfo {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float transparency;
	float shininess;
	int illumination;
	int hasDiffuseTexture;
	int diffuseTexture;
};

layout(location = 0) out vec4 FragColor;

uniform LightInfo light;
uniform MaterialInfo material;
//uniform sampler2D tex1;
uniform sampler2D Tex1;
uniform sampler2D Tex2;

//The prefix ec means Eye Coordinates in the Eye Coordinate System
in vec4 ecPosition;			
in vec3 ecLightDir;
in vec3 ecNormal;
in vec3 ecViewDir;
in vec2 ecTextCoord;


vec4 t1;
vec4 t2;
float lambert;
vec4  getTexture(vec3 N) {
		float textMixFactor = abs(dot(vec3(0,0,1.0),N));
		if(textMixFactor > 0.3) {
			return t1;
		}
		else if(textMixFactor > 0.7) {
			return t2;
		}
		else 
			return mix(t1, t2, lambert);
}

void main()
{
	t1 = texture2D( Tex1, ecTextCoord ).rgba;
	t2 = texture2D( Tex2, ecTextCoord ).rgba;
	vec3 N = normalize(ecNormal);
	vec3 L = normalize(ecLightDir);
	vec3 E = normalize(ecViewDir);
	lambert =  abs(dot(L, N));
	float lambertText;
	vec4 ambient = light.ambient * material.ambient;
	vec4 specular = vec4(0,0,0,1);
	vec4 diffuse;
	diffuse =  light.diffuse * lambert * material.diffuse;
	//vec3 R = 2 * dot(N,L) * N - L;
	vec4 finalColor = getTexture(N);
	if(material.transparency == 1) {
		if(lambert > 0.0 ) {
			vec3 R = normalize(reflect(-L, N));		
			//specular = pow(max(dot(R, E),0.0),20.0) * vec4(1,1,1,1);
		}
	}
	else {
			vec3 R = normalize(reflect(-L, N));		
			//specular = pow(max(dot(R, E),0.0),20.0) * vec4(1,1,1,1);
			diffuse =  light.diffuse * lambert * material.diffuse * vec4(1,1,1,0.4);
	}
	
	FragColor =  (ambient+diffuse + specular) + finalColor;
	FragColor.a = 1 - material.transparency;
}
