#version 330 core

struct Light {  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
};

uniform Light mLight;
uniform vec3 mViewPos;

uniform bool mUseDiffuse;
uniform bool mUseAmbient;
uniform bool mUseReflective;

uniform sampler2D mPositionMap;
uniform sampler2D mDiffuseColorMap;
uniform sampler2D mAmbientColorMap;
uniform sampler2D mReflectiveColorMap;
uniform sampler2D mNormalMap;

uniform vec2 mTextureSize;

layout (location = 0) out vec4 FragColor;

void main() {
	vec2 TexCoord = gl_FragCoord.xy / mTextureSize; // Calculate position in coordinate textures for the texture we are processing

	vec3 WorldPos = texture(mPositionMap, TexCoord).xyz;
	vec3 Normal = texture(mNormalMap, TexCoord).xyz; // Get data from the textures

	vec3 DiffuseColor = mUseDiffuse ? texture(mDiffuseColorMap, TexCoord).xyz : vec3(0.0, 0.0, 0.0);

	// Diffuse is the default color any other has to be especific
	vec3 AmbientColor = mUseAmbient ? texture(mAmbientColorMap, TexCoord).xyz : DiffuseColor;
	vec3 ReflectiveColor = mUseReflective ? texture(mReflectiveColorMap, TexCoord).xyz : DiffuseColor;

	vec3 normNormal = normalize(Normal);

	vec3 lightDir = normalize(mLight.position - WorldPos);
    vec3 viewDir = normalize(mViewPos - WorldPos);
    vec3 halfwayDir = normalize(lightDir + viewDir); 

    // ambient
    vec3 ambient = mLight.ambient;
  	
    // diffuse 
    float diff = max(dot(normNormal, lightDir), 0.0);
    vec3 diffuse = mLight.diffuse * diff;
    
    // specular
    float spec = max(dot(normNormal, halfwayDir), 0.0);
    vec3 specular = mLight.specular * spec;  
        
    vec3 result = (AmbientColor * ambient) + (DiffuseColor * diffuse) + (ReflectiveColor * specular);
    FragColor = vec4(result, 1.0);
}