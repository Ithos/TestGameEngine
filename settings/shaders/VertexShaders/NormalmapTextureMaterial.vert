#version 330 core
layout (location = 0) in vec4 posAttr; // the position variable has attribute position 0
layout (location = 1) in vec2 TexCoord;
layout (location = 2) in vec3 aNormal;

uniform highp mat4 modelViewProjectionMatrix;
uniform highp mat4 modelViewMatrix;
uniform highp mat4 modelMatrix;

out vec3 fragNormal;
out vec3 fragPos;
out vec2 fragTexCoord;
out vec3 fragNormalPos;
out vec3 fragTangent;

void main(void)
{
	gl_Position = modelViewProjectionMatrix * posAttr;
	
	fragPos = vec3(modelViewMatrix * posAttr);
	fragNormal = (modelMatrix * vec4(aNormal,0.0)).xyz;//Transformamos el valor del vector normal del espacio tridimensional al espacio de la ventana
	fragTexCoord = TexCoord;
	fragNormalPos = (modelMatrix * vec4(posAttr.x, posAttr.y, posAttr.z, 1.0)).xyz;

	vec3 tangent;
	vec3 v1=cross(aNormal,vec3(0.0,0.0,1.0));
	vec3 v2=cross(aNormal,vec3(0.0,1.0,0.0));
	if(length(v1)>length(v2)) tangent=v1;
	else tangent=v2; 

	fragTangent = normalize(modelMatrix*vec4(tangent, 0.0)).xyz;
}