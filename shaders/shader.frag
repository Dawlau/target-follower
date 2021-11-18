#version 400

in vec4 ex_Color;
in vec2 tex_Coord;
out vec4 out_Color;

uniform sampler2D Texture;
uniform bool target;

void main() {

  out_Color = texture(Texture, tex_Coord);
  // out_Color = ex_Color;
}