#version 410 core

in vec3 fNormal;
in vec4 fPosEye;
in vec2 fTexCoords;
in vec4 fragPosLightSpace;
in vec3 fPosition;
in vec4 pozitiaLuminii;
in vec4 dirLuminii;
out vec4 fColor;

//lighting
uniform	vec3 lightDir;
uniform	vec3 lightColor;
uniform vec3 lightColorFelinar;
uniform float brightness;
uniform float brightnessFelinar;
uniform float fogIntensity;
//texture
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D shadowMap;
uniform vec3 pozitieFelinar;
uniform vec3 pozitieFelinar1;
uniform vec3 pozitieFelinar2;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform	mat3 normalMatrix;
uniform mat4 lightSpaceTrMatrix;

vec3 ambient;
vec3 eyeNormal;
vec3 diffuse;
vec3 specular;
vec3 ambientFelinar;
vec3 diffuseFelinar;
vec3 specularFelinar;

float specularStrength = 0.5f;
float ambientStrength = 0.2;
float spec;
float constant = 1.0f;
float linear = 0.014f;
float quadratic = 0.007f;
float constantFelinar = 1.0f;
float linearFelinar = 0.014f;
float quadraticFelinar = 0.007f;

vec3 computePointLight(vec3 lightCoord){
   vec4 fragPosEye = view * vec4(lightCoord, 1.0f);
   vec3 normalEye = normalize(normalMatrix * fNormal);

  //compute light direction
  vec3 lightDirN = normalize(fragPosEye.xyz - fPosEye.xyz);

  //light direction
  vec3 viewDirN = normalize(- fPosEye.xyz);

  //compute distance to light
  float dist = length(fragPosEye.xyz - fPosEye.xyz);
  //compute attenuation
  float att = 1.0f / (constant + linear * dist + quadratic * (dist*dist));

  //compute ambient light
  ambient = att * ambientStrength * lightColorFelinar*brightnessFelinar;
  //compute diffuse light
  diffuse = att * max(dot(normalEye, lightDirN), 0.0f) * lightColorFelinar*brightnessFelinar;

  vec3 reflectDir = reflect(-lightDirN, normalEye);
  float specCoeff = pow(max(dot(viewDirN, reflectDir), 0.0f), 32.0f);
  specular = att * specularStrength * specCoeff * lightColorFelinar*brightnessFelinar;
  
  vec3 color = min((ambient + diffuse) + specular, 1.0f);
  return color;
}


float computeShadow()
{
	vec4 colorFromTexture = vec4(texture(diffuseTexture, fTexCoords));
    if(colorFromTexture.a < 0.4)
		discard;

	// Check whether current frag pos is in shadow
	float bias = 0.005f;

	vec3 normalizedCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	normalizedCoords = normalizedCoords * 0.5 + 0.5;

	vec4 shadowFromTexture = texture(shadowMap, vec2 (normalizedCoords.x, normalizedCoords.y));
    if(shadowFromTexture.a < 0.4)
		discard;

	if (normalizedCoords.z > 1.0f)
		return 0.0f;
	
	float closestDepth = texture(shadowMap, normalizedCoords.xy).r;

	// Get depth of current fragment from light's perspective
	float currentDepth = normalizedCoords.z;
	currentDepth = currentDepth - bias;

	// Check whether current frag pos is in shadow
	float shadow = currentDepth > closestDepth ? 1.0f : 0.0f;
	return shadow;
}

void computeLightComponents()
{
	vec3 cameraPosEye = vec3(0.0f);//in eye coordinates, the viewer is situated at the origin
	
	//transform normal
	vec3 normalEye = normalize(fNormal);	
	eyeNormal=normalEye;

	//compute light direction
	vec3 lightDirN = normalize(lightDir);
	
	//compute view direction 
	vec3 viewDirN = normalize(cameraPosEye - fPosEye.xyz);
	
	//compute ambient light
	ambient = ambientStrength * lightColor;
	
	//compute diffuse light
	diffuse = max(dot(normalEye, lightDirN), 0.0f) * lightColor;
	
	//compute specular light
	vec3 reflection = reflect(-lightDirN, normalEye);
	
	float specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), 32.0f);
    spec=specCoeff;
	specular = specularStrength * specCoeff * lightColor;
}



float computeFog(vec3 diffuse)
{
	float fogDensity = 0.06f;
	float fragmentDistance = length(fPosEye);
	float fogFactor = exp(-pow(fragmentDistance * fogDensity, 2))*fogIntensity;
	
	return clamp(fogFactor, 0.0f, 1.0f);
}

void main() 
{
	//compute distance to light
    float dist = length(pozitiaLuminii- fPosEye);	
	//compute attenuation
	float att =  1.0f/(constant+linear*dist+ quadratic*dist*dist);
	
	vec4 colorFromTexture = vec4(texture(diffuseTexture, fTexCoords));
    if(colorFromTexture.a < 0.4)
		discard;

	float shadow = computeShadow();

	computeLightComponents();

	ambient *= vec3(texture(diffuseTexture, fTexCoords));
	diffuse *= vec3(texture(diffuseTexture, fTexCoords));
	specular *= vec3(texture(specularTexture, fTexCoords));

	ambient *= brightness;
	diffuse *= brightness;
	specular *= brightness;
		
	vec3 color = min((ambient + (1.0f - shadow)*diffuse) + (1.0f - shadow)*specular, 1.0f);

	//color+= rezF;
	
	color =  color + computePointLight(pozitieFelinar) + computePointLight(pozitieFelinar1) + computePointLight(pozitieFelinar2);

	if (fogIntensity!=0)
	{
		 float fogFactor = computeFog(diffuse);
		 vec3 fogColor = vec3(0.5f, 0.5f, 0.5f)*brightness;
		 color = mix(fogColor, color, fogFactor);
	}

	fColor = vec4(color, 1.0f);

}