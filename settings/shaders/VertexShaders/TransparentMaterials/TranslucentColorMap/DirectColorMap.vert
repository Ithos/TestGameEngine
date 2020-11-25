#version 330 core

layout (location = 0) in vec4 posAttr; // the position variable has attribute position 0

uniform highp mat4 mModelViewProjectionMatrix;

uniform lowp vec3 diffuseColor;
uniform lowp vec3 reflectiveColor;

out lowp vec3 mDiffuseColor;
out lowp vec3 mReflectiveColor;

void main() {

	gl_Position = mModelViewProjectionMatrix * posAttr;

	mDiffuseColor = diffuseColor;
	mReflectiveColor = reflectiveColor;

}