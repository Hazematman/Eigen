#version 330

smooth in vec4 f_vertColour;
smooth in vec2 f_vertTexCoord;

uniform sampler2D texMap;

out vec4 outColour;

void main() {
    vec4 texColour = texture(texMap, vec2(f_vertTexCoord.x, 1-f_vertTexCoord));
    outColour = f_vertColour*texColour;
}
