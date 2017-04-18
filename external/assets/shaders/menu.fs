#version 430

uniform sampler2D menu;
in vec2 texCoord;

out vec4 FragColour;

void main() {
   FragColour = texture(menu,texCoord);
}
