#version 330 core

uniform vec3 mColor;

uniform float mThresholdAlphaValue;
uniform float mGlobalAlphaValue;

// Fragment shader that stores depth as a color
layout (location = 0) out vec4 FragSpecularColor;
layout (location = 1) out vec4 FragDiffuseColor;

void main()
{
	if(mGlobalAlphaValue <= mThresholdAlphaValue) discard;

	FragSpecularColor = vec4(mColor, 1.0f);
	FragDiffuseColor = vec4(mColor, 1.0f);
}