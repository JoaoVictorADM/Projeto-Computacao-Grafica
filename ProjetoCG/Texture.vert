#version 450

uniform mat4 Matrix;

in layout (location=0) vec3 vertexIn;
in layout (location=1) vec3 colorIn;
in layout (location=2) vec2 texcoord;

out vec4 colorOut; 
out vec2 Texcoord;

void main(){
	gl_Position =  Matrix * vec4(vertexIn, 1.0);
	colorOut = vec4(colorIn, 1.0f);
	Texcoord = texcoord;
}

