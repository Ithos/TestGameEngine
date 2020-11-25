#version 330 core

in lowp vec3 mDiffuseColor;
in lowp vec3 mReflectiveColor;

uniform float mGlobalAlphaValue;
uniform float mThresholdAlphaValue;

layout (location = 0) out vec4 FragDiffuseColor;
layout (location = 1) out vec4 FragReflectiveColor;

void main() {

	if(mGlobalAlphaValue <= mThresholdAlphaValue) discard;
 
 	vec3 ones = vec3(1.0f, 1.0f, 1.0f);
	vec3 diffDiffuse = ones - mDiffuseColor.xyz;
	vec3 diffReflective = ones - mReflectiveColor.xyz;
 
   	FragDiffuseColor = vec4(ones - mGlobalAlphaValue * diffDiffuse, mGlobalAlphaValue);
   	FragReflectiveColor = vec4(ones - mGlobalAlphaValue * diffReflective, mGlobalAlphaValue);
   	
}