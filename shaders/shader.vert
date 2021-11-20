#version 400

layout(location=0) in vec4 in_Position;
layout(location=1) in vec4 in_Color;
layout(location=2) in vec2 texCoord;

out vec4 ex_Color;
out vec2 tex_Coord;

uniform mat4 normMatrix;
uniform int target;



void main() {

  gl_Position = normMatrix * in_Position;
  ex_Color = in_Color;

  if (target == 1) {
    tex_Coord = vec2(-0.1f + texCoord.x, texCoord.y);
  } else {
    tex_Coord = vec2(texCoord.x, texCoord.y);
  }
}
