#version 330 core

uniform sampler2D textureAmbient;
uniform sampler2D textureDiffuse;
uniform sampler2D textureReflective;
uniform sampler2D textureEmissive;
uniform sampler2D normalMapTexture;

uniform float mGlobalAlphaValue;
uniform float mThresholdAlphaValue;
uniform float mShininess;

in vec3 fragNormal;
in vec3 fragPos;
in vec2 fragTexCoord;
in vec3 fragNormalPos;
in vec3 fragTangent;

layout (location = 0) out vec4 FragAmbientColor;
layout (location = 1) out vec4 FragDiffuseColor;
layout (location = 2) out vec4 FragReflectiveColor;
layout (location = 3) out vec4 FragEmissiveColor;
layout (location = 4) out vec4 WorldPosOut;
layout (location = 5) out vec4 NormalOut; 
layout (location = 6) out vec4 TexCoordOut;

void main(void)
{
	vec4 ambient = texture(textureAmbient, fragTexCoord);
	vec4 diffuse = texture(textureDiffuse, fragTexCoord);
	vec4 reflective = texture(textureReflective, fragTexCoord);
	vec4 emissive = texture(textureEmissive, fragTexCoord);

	float ambientAlpha = ambient.w * mGlobalAlphaValue;
	float diffuseAlpha = diffuse.w * mGlobalAlphaValue;
	float reflectiveAlpha = reflective.w * mGlobalAlphaValue;
	float emissiveAlpha = emissive.w * mGlobalAlphaValue;

	if(ambientAlpha <= mThresholdAlphaValue && diffuseAlpha <= mThresholdAlphaValue && reflectiveAlpha <= mThresholdAlphaValue && emissiveAlpha <= mThresholdAlphaValue ) discard;

	vec3 bitangent=cross(fragNormal,fragTangent);
	mat3 inverseSceneTangentMatrix=mat3(fragTangent.x,fragTangent.y,fragTangent.z,bitangent.x,bitangent.y,bitangent.z,fragNormal.x,fragNormal.y,fragNormal.z);

	FragAmbientColor = vec4(ambient.xyz, ambientAlpha);
	FragDiffuseColor = vec4(diffuse.xyz, diffuseAlpha);
	FragReflectiveColor = vec4(reflective.xyz, reflectiveAlpha);
	FragEmissiveColor = vec4(emissive.xyz, emissiveAlpha);
	WorldPosOut = vec4(fragNormalPos, 1.0);
    NormalOut = vec4( mShininess * normalize( inverseSceneTangentMatrix * ( (2 * texture(normalMapTexture, fragTexCoord).xyz) - vec3(1.0, 1.0, 1.0) ) ), 1.0); 
    TexCoordOut = vec4(fragTexCoord, 0.0, 0.0, 1.0);
}