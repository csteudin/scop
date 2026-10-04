#version 330 core

in vec3 vColor;
in vec2 vUV;

out vec4 FragColor;

uniform sampler2D uTexture;
uniform float uBlend;

void main()
{
    vec4 texColor = texture(uTexture, vUV);
    vec4 colorAsVec4 = vec4(vColor, 1.0);
    FragColor = mix(colorAsVec4, texColor, uBlend);
}
