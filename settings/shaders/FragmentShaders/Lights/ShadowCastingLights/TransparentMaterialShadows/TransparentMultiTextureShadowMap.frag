#version 330 core

uniform sampler2D textureAmbient;
uniform sampler2D textureDiffuse;
uniform sampler2D textureReflective;
uniform sampler2D textureEmissive;

uniform float mGlobalAlphaValue;
uniform float mThresholdAlphaValue;

in vec2 fragTexCoord;

// Fragment shader that stores depth as a color
layout (location = 0) out vec4 FragColor;

void main()
{
	vec4 ambient = texture(textureAmbient, fragTexCoord);
	vec4 diffuse = texture(textureDiffuse, fragTexCoord);
	vec4 reflective = texture(textureReflective, fragTexCoord);
	vec4 emissive = texture(textureEmissive, fragTexCoord);

	float ambientAlpha = ambient.w * mGlobalAlphaValue;
	float diffuseAlpha = diffuse.w * mGlobalAlphaValue;
	float reflectiveAlpha = reflective.w * mGlobalAlphaValue;
	float emissiveAlpha = emissive.w * mGlobalAlphaValue;

	if(ambientAlpha <= mThresholdAlphaValue && diffuseAlpha <= mThresholdAlphaValue && reflectiveAlpha <= mThresholdAlphaValue && emissiveAlpha <= mThresholdAlphaValue ) discard;

	FragColor = vec4(gl_FragCoord.x, gl_FragCoord.y, gl_FragCoord.z, 0);

	//Debug code to make depth visible
	//float hyper = (101 * (1-gl_FragCoord.z)) / (100 * (1 - gl_FragCoord.z) + 1);
	//FragColor = vec4(hyper, hyper, hyper, 1);
}