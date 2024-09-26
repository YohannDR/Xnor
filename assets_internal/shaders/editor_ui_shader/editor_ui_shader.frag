#version 460 core

out vec4 FragColor;

in VS_OUT 
{
    vec2 texCoords;
} fs_in;

uniform sampler2D uiTexture;

void main()
{
    FragColor = vec4(texture(uiTexture, fs_in.texCoords));
}
