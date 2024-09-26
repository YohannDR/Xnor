#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;  

out vec2 texCoords;

void main()
{
    texCoords = aTexCoords;
    gl_Position = mat4(1.0) * vec4(aPos,1.0);
}
