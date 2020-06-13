#version 330 core

uniform sampler2D textureColor;

uniform float mGlobalAlphaValue;
uniform float mThresholdAlphaValue;

in vec2 fragTexCoord;

// Fragment shader that stores depth as a color
layout (location = 0) out vec4 FragSpecularColor;
layout (location = 1) out vec4 FragDiffuseColor;

void main()
{
	vec4 texColor = texture(textureColor, fragTexCoord);
	float texAlpha = texColor.w * mGlobalAlphaValue;

	if(texAlpha <= mThresholdAlphaValue) discard;

	FragSpecularColor = vec4(texColor.xyz, 1.0f);
	FragDiffuseColor = vec4(texColor.xyz, 1.0f);
}