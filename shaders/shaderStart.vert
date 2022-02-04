#version 410 core

layout(location=0) in vec3 vPosition;
layout(location=1) in vec3 vNormal;
layout(location=2) in vec2 vTexCoords;

out vec3 fNormal;
out vec4 fPosEye;
out vec3 fPosition;
out vec2 fTexCoords;
out vec4 fragPosLightSpace;
out vec4 pozitiaLuminii;
out vec4 pozitieFelinar;
out vec4 dirLuminii;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform	mat3 normalMatrix;
uniform mat4 lightSpaceTrMatrix;



void main() 
{
	fragPosLightSpace = lightSpaceTrMatrix * model * vec4(vPosition, 1.0f);

	//compute eye space coordinates
	//fPosEye = view * model * vec4(vPosition, 1.0f);
	//fNormal = normalize(normalMatrix * vNormal);
	//fTexCoords = vTexCoords;
	//gl_Position = projection * view * model * vec4(vPosition, 1.0f);

	//pozitiaLuminii = vec4(32.6966f, 2.65902f, 185.192f, 1.0f);
	//pozitiaLuminii = view*model*pozitiaLuminii;
    
	//pozitieFelinar = vec4(48.2474f, 4.59148f, 275.655f, 1.0f);
	//pozitieFelinar = view*model*pozitieFelinar;

	//dirLuminii = pozitiaLuminii;

	gl_Position = projection * view * model * vec4(vPosition, 1.0f);
	fPosEye = view * model * vec4(vPosition, 1.0f);
	fPosition = vPosition;
	fNormal = normalize(normalMatrix * vNormal);
	fTexCoords = vTexCoords;
	fragPosLightSpace = lightSpaceTrMatrix * model * vec4(vPosition, 1.0f);
    
}