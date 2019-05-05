#version 330 core
layout (location = 0) in vec4 posAttr; // the position variable has attribute position 0

uniform highp mat4 modelViewProjectionMatrix;

void main(){
	gl_Position = modelViewProjectionMatrix * posAttr;
}