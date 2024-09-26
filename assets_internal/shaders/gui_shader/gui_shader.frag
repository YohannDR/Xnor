#version 460 core

out vec4 FragColor;

in VS_OUT 
{
    vec2 texCoords;
} fs_in;

uniform sampler2D uiTexture;

void main()
{
    vec4 texture = texture(uiTexture,fs_in.texCoords); 
    vec3 color = texture.xyz;
    color = pow(color,vec3(2.2));
    FragColor = vec4(color, texture.w); 
}
