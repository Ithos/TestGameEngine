#version 330 core

uniform sampler2D textureColor;

in vec2 fragTexCoord;

uniform float mGlobalAlphaValue;
uniform float mThresholdAlphaValue;

layout (location = 0) out vec4 FragDiffuseColor;
layout (location = 1) out vec4 FragReflectiveColor;

void main(void)
{
	vec4 texColor = texture(textureColor, fragTexCoord);
	float texAlpha = texColor.w * mGlobalAlphaValue;
	if(texAlpha <= mThresholdAlphaValue) discard;

	vec3 ones = vec3(1.0f, 1.0f, 1.0f);
	vec3 diff = ones - texColor.xyz;
	
	FragDiffuseColor = vec4(ones - texAlpha * diff, texAlpha);
	FragReflectiveColor = vec4(ones - texAlpha * diff, texAlpha);

}