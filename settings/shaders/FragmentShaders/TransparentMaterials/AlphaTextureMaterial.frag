#version 330 core

uniform sampler2D textureColor;

uniform float mGlobalAlphaValue;
uniform float mThresholdAlphaValue;
uniform float mShininess;

in vec3 fragNormal;
in vec3 fragPos;
in vec2 fragTexCoord;
in vec3 fragNormalPos;

layout (location = 0) out vec4 FragAmbientColor;
layout (location = 1) out vec4 FragDiffuseColor;
layout (location = 2) out vec4 FragReflectiveColor;
layout (location = 3) out vec4 FragEmissiveColor;
layout (location = 4) out vec4 WorldPosOut;
layout (location = 5) out vec4 NormalOut; 
layout (location = 6) out vec4 TexCoordOut;

void main(void)
{
	vec4 texColor = texture(textureColor, fragTexCoord);
	float texAlpha = texColor.w * mGlobalAlphaValue;
	if(texAlpha <= mThresholdAlphaValue) discard;

	FragAmbientColor = vec4(texColor.xyz, texAlpha);
	FragDiffuseColor = vec4(texColor.xyz, texAlpha);
	FragReflectiveColor = vec4(texColor.xyz, texAlpha);
	FragEmissiveColor = vec4(0.0, 0.0, 0.0, texAlpha);
	WorldPosOut = vec4(fragNormalPos, 1.0);
    NormalOut = vec4( mShininess * normalize(fragNormal), 1.0);
    TexCoordOut = vec4(fragTexCoord, 0.0, 1.0);
}