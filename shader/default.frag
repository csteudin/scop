#version 330 core

in vec3 vColor;
in vec2 vUV;
in vec3 vNormal; 

out vec4 FragColor;

uniform sampler2D uTexture;
uniform float uBlend;
uniform float uLight;


void main()
{
    vec3 texColor = texture(uTexture, vUV).rgb;
    
    vec3 base = mix(vColor, texColor, uBlend);

    vec3 n = vNormal;
    if (length(n) > 0.0)
        n = normalize(n);
    vec3 lightDir = normalize(vec3(0.4, 0.7, 1.0));
    float diff = abs(dot(n, lightDir));
    
    float shade = mix(1.0, 0.25 + 0.75 * diff, uLight);

    FragColor = vec4(base * shade, 1.0);
}
