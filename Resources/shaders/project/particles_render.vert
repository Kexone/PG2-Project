#version 400 core

layout (location = 0) in vec2 vertexPosition; // <vec2 position, vec2 texCoords>
layout (location = 1) in vec2 uv; 

out vec2 TexCoords;
out vec4 ParticleColor;

uniform mat4 PMatrix;		//Camera projection matrix
uniform mat4 VMatrix;		//Camera view matrix
uniform mat4 MVMatrix;		//Camera view matrix
uniform vec3 offset;
uniform float life;
uniform vec3 color;

void main()
{
    vec3 scale = vec3(0.1, 0.1, 0.2);
    TexCoords = uv;

    float a = life;
    if(life > 0.7) {
    	a = abs(life - 1) * 3.33;
	}
    vec3 vpos = vec3(vertexPosition.x, 0, vertexPosition.y) - vec3(0.5, 0, 0.5);
    gl_Position = PMatrix * MVMatrix * vec4(vpos * (scale + a * 0.5)  + offset, 1);
	ParticleColor = vec4(color, a);
}