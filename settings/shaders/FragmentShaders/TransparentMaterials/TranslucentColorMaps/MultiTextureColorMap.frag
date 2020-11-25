#version 330 core

uniform sampler2D textureDiffuse;
uniform sampler2D textureReflective;

in vec2 fragTexCoord;

uniform float mGlobalAlphaValue;
uniform float mThresholdAlphaValue;

layout (location = 0) out vec4 FragDiffuseColor;
layout (location = 1) out vec4 FragReflectiveColor;

void main(void)
{

	vec4 diffuse = texture(textureDiffuse, fragTexCoord);
	vec4 reflective = texture(textureReflective, fragTexCoord);

	float diffuseAlpha = diffuse.w * mGlobalAlphaValue;
	float reflectiveAlpha = reflective.w * mGlobalAlphaValue;

	if( diffuseAlpha <= mThresholdAlphaValue && reflectiveAlpha <= mThresholdAlphaValue ) discard;

	vec3 ones = vec3(1.0f, 1.0f, 1.0f);
	vec3 diffDiffuse = ones - diffuse.xyz;
	vec3 diffReflective = ones - reflective.xyz;

	FragDiffuseColor = vec4(ones - diffuseAlpha * diffDiffuse, diffuseAlpha);
	FragReflectiveColor = vec4(ones - reflectiveAlpha * diffReflective, reflectiveAlpha);
	
}