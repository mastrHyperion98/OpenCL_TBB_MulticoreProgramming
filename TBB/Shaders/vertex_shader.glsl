#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
uniform mat4 orthoMatrix = mat4(1.0);

out vec3 vertexColor;
void main()
{
   vertexColor = aColor;
   gl_Position = orthoMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}