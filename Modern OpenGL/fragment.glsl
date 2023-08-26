#version 330 core
in vec2 fTexCoord;
in vec3 fColor;

out vec4 FragColor;

uniform sampler2D dirt_texture;
uniform sampler2D grass_texture;

void main()
{
	FragColor = vec4(fColor, 1.0);
	// FragColor = mix(texture(dirt_texture, fTexCoord), texture(grass_texture, fTexCoord), 0.4);
}