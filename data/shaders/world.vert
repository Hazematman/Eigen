#version 330

in vec3 vertPos;
in vec2 vertTexCoord;
in vec4 vertColour;

uniform mat4 trans;

smooth out vec4 f_vertColour;
smooth out vec2 f_vertTexCoord;

float pi = 3.14159;
float piO4 = pi/4;
void main() {
    vec4 newPos = trans*vec4(vertPos.x, vertPos.y, 0, 1);
    newPos.y = vertPos.z*cos(-piO4*newPos.y + piO4) - 1.0;
    gl_Position = newPos;
    //gl_Position = trans*vec4(vertPos.x, vertPos.y, 0, 1);
    f_vertColour = vertColour;
    f_vertTexCoord = vertTexCoord;
}
