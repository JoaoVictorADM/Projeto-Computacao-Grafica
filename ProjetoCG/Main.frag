#version 450

out vec4 fragColor;

uniform vec4 colorOut;

void main(){
	fragColor = colorOut;
}