#version 330 core

uniform lowp vec3 mDiffuseColor;
uniform lowp vec3 mReflectiveColor;

uniform float mThresholdAlphaValue;
uniform float mGlobalAlphaValue;

// Fragment shader that stores depth as a color
layout (location = 0) out vec4 FragSpecularColor;
layout (location = 1) out vec4 FragDiffuseColor;

void main()
{
	if(mGlobalAlphaValue <= mThresholdAlphaValue) discard;

	FragSpecularColor = vec4(mReflectiveColor, 1.0f);
	FragDiffuseColor = vec4(mDiffuseColor, 1.0f);
}