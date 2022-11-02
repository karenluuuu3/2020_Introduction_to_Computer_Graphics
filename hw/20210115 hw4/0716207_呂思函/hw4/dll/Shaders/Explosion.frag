#version 430

uniform sampler2D texture;

in vec2 TexCoords;
out vec4 fragcolor;

void main()
{
	fragcolor = texture2D(texture,TexCoords);
} 