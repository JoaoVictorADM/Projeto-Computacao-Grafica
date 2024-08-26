#version 450

in vec4 colorOut;
in vec2 Texcoord;

out vec4 fragColor;

uniform sampler2D tex;

void main(){
	fragColor = texture(tex, Texcoord) * colorOut;
}