#version 330

in vec2 vertPos;
in vec2 vertTexCoord;

uniform mat4 trans;
uniform float radius;
uniform float yPos;

smooth out vec2 f_vertTexCoord;

float pi = 3.14159;
float piO4 = pi/4;
void main() {
    vec4 newPos = trans*vec4(vertPos, 0.0, 1.0);
    newPos.z = -(radius*sin(-piO4*yPos + piO4))/3.0;
    gl_Position = newPos;

    f_vertTexCoord = vertTexCoord;
}
