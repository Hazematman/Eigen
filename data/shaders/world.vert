#version 330

in vec3 vertPos;
in vec2 vertTexCoord;
in vec4 vertColour;

uniform mat4 trans;
uniform float shift;

smooth out vec4 f_vertColour;
smooth out vec2 f_vertTexCoord;

float pi = 3.14159;
float piO4 = pi/4;
void main() {
    vec4 newPos = trans*vec4(vertPos.x, vertPos.y, 0, 1);
    newPos.z = -(vertPos.z*sin(-piO4*newPos.y + piO4))/3.0;
    newPos.y = vertPos.z*cos(-piO4*newPos.y + piO4) - shift;
    gl_Position = newPos;
    f_vertColour = vertColour;
    f_vertTexCoord = vertTexCoord;
}
