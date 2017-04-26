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
};

layout(location = 0) out vec4 FragColor;

uniform LightInfo light;
uniform MaterialInfo material;
//uniform sampler2D diffuseTexture;
uniform mat4 TMatrix;
uniform sampler2D tex1;
uniform sampler2D tex2;

//The prefix ec means Eye Coordinates in the Eye Coordinate System
in vec4 ecPosition;			
in vec3 ecLightDir;
in vec3 ecNormal;
in vec3 ecViewDir;
in vec2 ecTexCoords;
in float texMixFactor;
void main()
{

	vec3 N = normalize(ecNormal);
	vec3 L = normalize(ecLightDir);

	float lambert = dot(N, L);

	vec4 t1 = texture2D(tex1, ecTexCoords);
	vec4 t2 = texture2D(tex2, ecTexCoords);
	
	vec4 color = mix(t2,t1,lambert).rgba;


	FragColor = material.ambient * light.ambient;
	
	if (lambert > 0.0) {
		vec3 E = normalize(ecViewDir);
        vec3 R = normalize(reflect(-L, N));

				vec4 t1 = texture2D(tex1, ecTexCoords).rgba;
		vec4 t2 = texture2D(tex2, ecTexCoords).rgba;
		vec4 texFinal;

		if (texMixFactor < 0.3) {
			texFinal = t2;
		} else if (texMixFactor > 0.7) {
			texFinal = t1;
		} else {
			float factor = (texMixFactor - 0.3) / (0.7 - 0.3);
			texFinal = mix(t2, t1, factor);
}

		vec4 diffuse = lambert * texFinal * light.diffuse;
        vec4 specular = pow(max(dot(R, E), 0.0), material.shininess) * light.specular * material.specular;

		FragColor += diffuse + specular;
	}
}