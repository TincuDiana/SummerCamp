#version 410 core

in vec3 textureCoordinates;
out vec4 color;
in vec4 fPosEye;

uniform samplerCube skybox;
uniform float brightness;
uniform float fogIntensity;

float computeFog()
{
	float fogDensity = 0.01f;
	float fragmentDistance = length(fPosEye);
	float fogFactor = exp(-pow(fragmentDistance * fogDensity, 2))*pow (5,fogIntensity);
	return clamp(fogFactor, 0.0f, 1.0f);
}

void main()
{
    vec3 fogColor = vec3(0.3,0.3,0.3);
	
	float fogFactor = computeFog();
	vec3 fColor = mix(fogColor,vec3(0,0,0),fogFactor);
	
	if (fogIntensity==0)
	 color = vec4(texture(skybox, textureCoordinates)*brightness);
	else
	  color = vec4(texture(skybox, textureCoordinates*fColor)*brightness);
  
}
