#version 150
#define PI 3.1415926535897932384626433832795

in vec4  position;
in vec4  color;
in float scale;
in float rotation;
out vec4 color_from_vshader;

void main() {

  vec2 origo = vec2(position.z, position.w);

  // 1. Scale transform
  vec2 scaled = vec2(position.x * scale, position.y * scale);

  // 2. Rotation transform
  float rotation_radians = (((int(rotation) % 360) / 360.0) * 2.0*PI) - PI;

  vec2 rotated = vec2(scaled.x * cos(rotation_radians) - scaled.y * sin(rotation_radians),
                      scaled.x * sin(rotation_radians) - scaled.y * cos(rotation_radians));

  // 3. World position transform
  vec4 worldified = vec4(rotated.x + origo.x,
                         rotated.y + origo.y, 0.0, 0.0);

  // Why doesn't this work
//  gl_Position = vec4(position.x, position.y, 0.0, 0.0);
  // when this works? 
  gl_Position = position;
	color_from_vshader = color;
}