#version 330 core

in vec3 vColor;
in vec2 vUV;

out vec4 FragColor;

uniform sampler2D uTexture;
uniform float uBlend;

void main()
{
    vec3 texColor = texture(uTexture, vUV).rgb;
    FragColor = vec4(mix(vColor, texColor, uBlend), 1.0);
}
