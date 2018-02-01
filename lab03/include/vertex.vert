#version 150

in vec4 position;
in vec4 color;

out vec4 dstColor;

uniform mat4 model;
uniform mat4 view;              //<-- 4x4 Transformation Matrices
uniform mat4 projection;

void main() {
	gl_Position = projection * view * model * position;   //<-- Apply transformation 
	dstColor = color;	
}