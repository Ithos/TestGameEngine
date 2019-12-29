#version 330 core

// Fragment shader that stores depth as a color
layout (location = 0) out vec4 FragColor;

void main()
{
	FragColor = vec4(gl_FragCoord.x, gl_FragCoord.y, gl_FragCoord.z, 1);

	//Debug code to make depth visible
	//float hyper = (101 * (1-gl_FragCoord.z)) / (100 * (1 - gl_FragCoord.z) + 1);
	//FragColor = vec4(hyper, hyper, hyper, 1);
}