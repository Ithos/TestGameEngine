#version 330

layout (location = 0) in vec3 posAttr; 
layout (location = 1) in vec2 TexCoord;

uniform mat4 mModelViewProjectionMatrix;
out vec2 fragTexCoord;

void main()
{          
	fragTexCoord = TexCoord;
    gl_Position = mModelViewProjectionMatrix * vec4(posAttr, 1.0);
}