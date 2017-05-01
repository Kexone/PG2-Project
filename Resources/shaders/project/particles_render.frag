#version 400 core

in vec2 TexCoords;
in vec4 ParticleColor;
layout(location = 0) out vec4 FragColor;

uniform sampler2D sprite;

void main()
{
    vec4 texColor = texture(sprite, TexCoords);
	texColor.a = min(1, length(texColor.xyz));
    FragColor =  (texColor * ParticleColor);

}  