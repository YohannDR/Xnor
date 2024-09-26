#version 460 core

out vec4 FragColor;

in vec2 texCoords;

uniform sampler2D bufferTextureId;

void main()
{
    vec3 color = texture(bufferTextureId, texCoords).rgb;

    FragColor = vec4(color, 1.0);
}
