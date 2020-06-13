#version 330 core

uniform sampler2D textureDiffuse;
uniform sampler2D textureReflective;

uniform float mGlobalAlphaValue;
uniform float mThresholdAlphaValue;

in vec2 fragTexCoord;

// Fragment shader that stores depth as a color
layout (location = 0) out vec4 FragSpecularColor;
layout (location = 1) out vec4 FragDiffuseColor;

void main()
{
	vec4 diffuse = texture(textureDiffuse, fragTexCoord);
	vec4 reflective = texture(textureReflective, fragTexCoord);

	float diffuseAlpha = diffuse.w * mGlobalAlphaValue;
	float reflectiveAlpha = reflective.w * mGlobalAlphaValue;

	if( diffuseAlpha <= mThresholdAlphaValue && reflectiveAlpha <= mThresholdAlphaValue ) discard;

	FragSpecularColor = vec4(reflective.xyz, 1.0f);
	FragDiffuseColor = vec4(diffuse.xyz, 1.0f);
}