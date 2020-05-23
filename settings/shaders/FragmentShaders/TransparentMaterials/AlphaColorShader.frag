#version 330 core

in vec3 fragNormal;
in vec3 fragPos;
in vec2 fragTexCoord;
in vec3 fragNormalPos;

in lowp vec3 mAmbientColor;
in lowp vec3 mDiffuseColor;
in lowp vec3 mReflectiveColor;
in lowp vec3 mEmissiveColor;

uniform float mGlobalAlphaValue;
uniform float mThresholdAlphaValue;
uniform float mShininess;

layout (location = 0) out vec4 FragAmbientColor;
layout (location = 1) out vec4 FragDiffuseColor;
layout (location = 2) out vec4 FragReflectiveColor;
layout (location = 3) out vec4 FragEmissiveColor;
layout (location = 4) out vec4 WorldPosOut;
layout (location = 5) out vec4 NormalOut; 
layout (location = 6) out vec4 TexCoordOut;

void main() {

	if(mGlobalAlphaValue <= mThresholdAlphaValue) discard;
 
   	FragAmbientColor =vec4( mAmbientColor, mGlobalAlphaValue);
   	FragDiffuseColor = vec4(mDiffuseColor, mGlobalAlphaValue);
   	FragReflectiveColor = vec4(mReflectiveColor, mGlobalAlphaValue);
   	FragEmissiveColor = vec4(mEmissiveColor, mGlobalAlphaValue);
	WorldPosOut = vec4(fragNormalPos, 1.0);
    NormalOut = vec4( mShininess * normalize(fragNormal), 1.0);
    TexCoordOut = vec4(fragTexCoord, 0.0, 1.0);
}