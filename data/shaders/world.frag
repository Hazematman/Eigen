#version 330

smooth in vec4 f_vertColour;
smooth in vec2 f_vertTexCoord;

uniform sampler2D texMap;

out vec4 outColour;

void main() {
    vec4 texColour = texture(texMap, vec2(f_vertTexCoord.x, f_vertTexCoord.y));
    if(texColour.a == 0) {
        discard;
    }
    outColour = (1-gl_FragCoord.z)*f_vertColour*texColour + (gl_FragCoord.z)*vec4(0.0,0.0,0.0,1.0);

}
