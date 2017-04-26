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

//The prefix ec means Eye Coordinates in the Eye Coordinate System
in vec4 ecPosition;			
in vec3 ecLightDir;
in vec3 ecNormal;
in vec3 ecViewDir;

void main()
{
	float alpha = 0.7;
	float beta = 0.7;
	vec3 N = normalize(ecNormal);
	vec3 L = normalize(ecLightDir);
	vec3 E = normalize(ecViewDir);
	float lambert =  dot(L, N);
	vec4 ambient = light.ambient * material.ambient;
	vec4 specular = vec4(0,0,0,1);
	vec4 diffuse;
	vec4 kcool = vec4(0,0,1,1);
	vec4 kwarm = vec4(0,1,0,1);

	if(lambert > 0 ) {
		vec3 R = normalize(reflect(-E, N));		
		specular = pow(max(dot(R, E),0.0), material.shininess) * light.specular * material.specular;
	}
	float gooch = dot(E, N);
	if (gooch > -0.2 && gooch < 0.2){
		diffuse = ambient = specular = vec4(0, 0, 0, 1);
	}
	else {
		vec4 kcdiff = kcool + alpha * material.diffuse * light.diffuse;
		vec4 kwdiff = kwarm + beta  * material.diffuse * light.diffuse;
		diffuse = ((1  + dot(N, -E))  / 2.0) * kcdiff + (1 -  (1 + dot(N, -E)  / 2.0 )) * kwdiff;
	}

	FragColor =  ambient+diffuse + specular ;
}
