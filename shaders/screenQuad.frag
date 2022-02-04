#version 410 core

out vec4 fColor;
in vec2 fTexCoords;

uniform sampler2D depthMap;

void main() 
{    
    vec4 colorFromTexture = texture(depthMap, fTexCoords);
    if(colorFromTexture.a < 0.4)
		discard;
   // fColor = vec4(vec3(texture(depthMap, fTexCoords).r), 1.0f);
    fColor = vec4(fTexCoords, 0.0f, 1.0f);
}
