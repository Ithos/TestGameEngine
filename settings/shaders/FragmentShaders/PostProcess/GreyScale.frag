#version 330 core

uniform sampler2D mTexture;
uniform vec2 mTextureSize;

layout (location = 0) out vec4 FragColor;

void main(){
	vec2 pos = gl_FragCoord.xy / mTextureSize;
	float intensity= dot(texture2D(mTexture,pos).rgb,vec3(0.29,0.59,0.12));//Get weighted color intensity
	FragColor=vec4(intensity,intensity,intensity,1.0);
}