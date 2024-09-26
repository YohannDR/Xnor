#version 460 core


layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;  


out VS_OUT
{
    vec2 texCoords;
} vs_out;

uniform mat4 projection;
uniform mat4 model;

void main()
{
   vs_out.texCoords = aTexCoords;
   gl_Position = projection  * model * vec4(aPos, 1.0);
}
