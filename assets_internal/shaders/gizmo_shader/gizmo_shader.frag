#version 460 core
#extension GL_NV_gpu_shader5 : require

out vec4 FragColor;

uniform vec3 color;

void main()
{
    FragColor = vec4(color, 1);
}
