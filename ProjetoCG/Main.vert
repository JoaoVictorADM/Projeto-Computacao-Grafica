#version 450

in layout (location=0) vec3 vertexIn;

uniform mat4 Matrix;

void main(){
	gl_Position =  Matrix * vec4(vertexIn, 1.0);
}

