#version 460 core

out vec4 fragColor;

uniform samplerCube skybox;

in VS_OUT
{
    vec3 texCoords;
} fs_in;

void main()
{
    vec3 color = texture(skybox, fs_in.texCoords).rgb;
    fragColor =  vec4(pow(color, vec3(2.2)), 1.0);
}
