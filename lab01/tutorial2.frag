#version 150
// It was expressed that some drivers required this next line to function properly
precision highp float;
 
in  vec4 ex_Color;
out vec4 FragColor;

void main(void) {
    FragColor = vec4(ex_Color);
}