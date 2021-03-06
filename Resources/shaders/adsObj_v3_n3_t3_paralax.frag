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

uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
uniform sampler2D depthTexture;



//The prefix ec means Eye Coordinates in the Eye Coordinate System
in vec4 ecPosition;		
in vec3 ecLightDir;
in vec3 ecNormal;
in vec3 ecViewDir;

// Tangent space
in VS_OUT {
	vec3 tsLightDir;
	vec3 tsNormal;
	vec3 tsViewDir;
	vec2 tsTexCoords;
} fs_in;

const float scale = 0.05;
const float numSteps = 10;
float step = 1.0 / numSteps;
const float bias = 0.05;

void main()
{
	vec3 E = normalize(fs_in.tsViewDir);
    vec2 dtex = - E.xy * scale / (numSteps * E.z);
    float effectHeight = 1.0;
    vec2 newTexCoords = fs_in.tsTexCoords;

	float height = scale * (1.0 - texture2D( depthTexture, newTexCoords).r) + bias;

	while (height < effectHeight) {
		effectHeight -= step;
		newTexCoords += dtex; 
		height = texture2D(depthTexture, newTexCoords).r; // get new height
	}

	vec2 delta = 0.5 * dtex;
	vec2 mid = newTexCoords - delta;
	for(int i = 0; i < 5;  i ++) {
		delta *= 0.5;
		if(texture2D(depthTexture, mid).r <effectHeight) {
			mid += delta;
		}
		else {
			mid -= delta;
		}

	}

	newTexCoords = mid;
	vec2 offset = (E.xy / E.z)  * height;
	
	vec4 texColor = vec4(1.0, 1.0, 1.0, 1.0);
	vec3 tsTextureNormal = texture2D(normalTexture, newTexCoords).rgb * 2.0 - vec3(1.0);
	texColor = texColor * texture2D(diffuseTexture, newTexCoords).rgba;
	FragColor = light.ambient * texColor * material.ambient;

	// Lambert test
	vec3 N = normalize(tsTextureNormal + fs_in.tsNormal);
	vec3 L = normalize(fs_in.tsLightDir);
	float lambert = max(dot(N, L), 0.0);

	if (lambert > 0.0) {
		FragColor += light.diffuse * texColor * lambert * material.diffuse;

		
    	vec3 R = normalize(2.0 * dot(N, fs_in.tsLightDir) * N - fs_in.tsLightDir);
        float specular = pow(max(dot(R, E), 0.0), material.shininess);

		FragColor += light.specular * texColor * specular * material.specular;
	}
}

/*
void main()
{
	float steps = 32;
	vec3 E = normalize(fs_in.tsViewDir);
	float step = 1.0/steps;

	float scale = 0.02;
	float bias = 0.01;
	float effectiveHeight = 1.0;
	vec2 dtex = - E.xy * scale / (steps * E.z);
	vec2 newTexCoords = fs_in.tsTexCoords;
	float height = texture(depthTexture, fs_in.tsTexCoords).r * scale + bias;
	vec2 paralaxOffset =1- ((E.xy / E.z) * texture(depthTexture, fs_in.tsTexCoords).r * scale + bias);
	
	while(height < effectiveHeight){
	effectiveHeight -= step;
	newTexCoords += dtex;
	height = 1- texture2D(depthTexture, newTexCoords).r;
	}

	vec2 delta = 0.5 * dtex;
	vec2 mid = newTexCoords - delta;

	for(int i = 0;i<5;i++){
	delta*=0.5;
	if(texture2D(depthTexture,mid).r<effectiveHeight){
		mid +=delta;
			}
	else mid-=delta;
	}

	newTexCoords = mid;
	vec2 TexCoords = fs_in.tsTexCoords + paralaxOffset;

	vec3 tsTextureNormal = texture(normalTexture, newTexCoords).rgb * 2.0 - vec3(1.0);
	vec4 texColor = vec4(1.0, 1.0, 1.0, 1.0);
	
	texColor = texColor * texture2D(diffuseTexture, newTexCoords).rgba;
	FragColor = light.ambient * texColor * material.ambient;

	// Lambert test
	vec3 N = normalize(tsTextureNormal + fs_in.tsNormal);
	vec3 L = normalize(fs_in.tsLightDir);
	float lambert = max(dot(N, L), 0.0);

	if (lambert > 0.0) {
		FragColor += light.diffuse * texColor * lambert * material.diffuse;

        vec3 R = normalize(2.0 * dot(N, fs_in.tsLightDir) * N - fs_in.tsLightDir);
        float specular = pow(max(dot(R, E), 0.0), material.shininess);

		FragColor += light.specular * texColor * specular * material.specular;
}
}
*/
/*

void main()
{
	vec3 E = normalize(fs_in.tsViewDir);
	float scale = 0.02;
	float bias = 0.01;
	vec2 paralaxOffset =1- ((E.xy / E.z) * texture(depthTexture, fs_in.tsTexCoords).r * scale + bias);
	vec2 TexCoords = fs_in.tsTexCoords + paralaxOffset;

	vec3 tsTextureNormal = texture(normalTexture, TexCoords).rgb * 2.0 - vec3(1.0);
	vec4 texColor = vec4(1.0, 1.0, 1.0, 1.0);
	
	texColor = texColor * texture2D(diffuseTexture, TexCoords).rgba;
	FragColor = light.ambient * texColor * material.ambient;

	// Lambert test
	vec3 N = normalize(tsTextureNormal + fs_in.tsNormal);
	vec3 L = normalize(fs_in.tsLightDir);
	float lambert = max(dot(N, L), 0.0);

	if (lambert > 0.0) {
		FragColor += light.diffuse * texColor * lambert * material.diffuse;

		vec3 E = normalize(fs_in.tsViewDir);
        vec3 R = normalize(2.0 * dot(N, fs_in.tsLightDir) * N - fs_in.tsLightDir);
        float specular = pow(max(dot(R, E), 0.0), material.shininess);

		FragColor += light.specular * texColor * specular * material.specular;
	}
}
*/