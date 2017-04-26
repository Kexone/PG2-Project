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
/*in vec4 ecPosition;			
in vec3 ecLightDir;
in vec3 ecNormal;
in vec3 ecViewDir;
in vec2 ecTexCoords;
in float texMixFactor;
*/

in VS_OUT{

 vec3 ecLightDir;
 vec3 ecNormal;
 vec3 ecViewDir;
 vec2 ecTexCoords;

}vs_in;


void main()
{

		vec3 TextureNorm = texture(tex1, vs_in.ecTexCoords).rgb * 2.0-vec3(1.0);
		vec4 texColor = vec4(1.0);

		if(material.hasDiffuseTexture!=0)
		{
			
			texColor = texColor * texture(tex2, vs_in.ecTexCoords);
		}


	vec3 N = normalize(TextureNorm + vs_in.ecNormal);

	vec3 L = normalize(vs_in.ecLightDir);

	float lambert = dot(N, L);

	vec4 t1 = texture2D(tex1, vs_in.ecTexCoords);

	FragColor = material.ambient * light.ambient;
	
	if (lambert > 0.0) {
		vec3 E = normalize(vs_in.ecViewDir);
        vec3 R = normalize(reflect(-L, N));

				vec4 t1 = texture2D(tex1, vs_in.ecTexCoords).rgba;
		
		

		vec4 diffuse = lambert *t1 * light.diffuse;
        vec4 specular = pow(max(dot(R, E), 0.0), material.shininess) * light.specular * material.specular;

		FragColor += diffuse + specular;
	}
} 