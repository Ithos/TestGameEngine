#version 330 core

uniform float mThresholdAlphaValue;
uniform float mGlobalAlphaValue;

// Fragment shader that stores depth as a color
layout (location = 0) out vec4 FragColor;

void main()
{
	
	if(mGlobalAlphaValue <= mThresholdAlphaValue) discard;

	FragColor = vec4(0.0f, 0.0f, gl_FragCoord.z, 0.0f);

	//Debug code to make depth visible
	//float hyper = (101 * (1-gl_FragCoord.z)) / (100 * (1 - gl_FragCoord.z) + 1);
	//FragColor = vec4(hyper, hyper, hyper, 1);
}