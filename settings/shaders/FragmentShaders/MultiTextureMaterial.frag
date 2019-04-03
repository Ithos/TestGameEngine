#version 330 core

uniform sampler2D textureAmbient;
uniform sampler2D textureDiffuse;
uniform sampler2D textureReflective;
uniform sampler2D textureEmissive;

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
	FragAmbientColor = vec4(texture(textureAmbient, fragTexCoord).xyz, 1.0);
	FragDiffuseColor = vec4(texture(textureDiffuse, fragTexCoord).xyz, 1.0);
	FragReflectiveColor = vec4(texture(textureReflective, fragTexCoord).xyz, 1.0);
	FragEmissiveColor = vec4(texture(textureEmissive, fragTexCoord).xyz, 1.0);
	WorldPosOut = vec4(fragNormalPos, 1.0);
    NormalOut = vec4(normalize(fragNormal), 1.0);
    TexCoordOut = vec4(fragTexCoord, 0.0, 1.0);
}