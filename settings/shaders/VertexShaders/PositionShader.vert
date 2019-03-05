#version 330

layout (location = 0) in vec3 posAttr; 

uniform mat4 mModelViewProjectionMatrix;

void main()
{          
    gl_Position = mModelViewProjectionMatrix * vec4(posAttr, 1.0);
}