#version 330 core

struct Attenuation
{
    float constant;
    float linear;
    float quadratic;
};

struct Light {  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;

    Attenuation attenuation;
};
  
uniform Light mLight;
uniform vec3 mViewPos;

uniform bool mUseDiffuse;
uniform bool mUseAmbient;
uniform bool mUseReflective;
uniform bool mUseEmissive;

uniform sampler2D mPositionMap;
uniform sampler2D mDiffuseColorMap;
uniform sampler2D mAmbientColorMap;
uniform sampler2D mReflectiveColorMap;
uniform sampler2D mEmissiveColorMap;
uniform sampler2D mNormalMap;

uniform vec2 mTextureSize;

layout (location = 0) out vec4 FragColor;

void main() {
    
    vec2 TexCoord = gl_FragCoord.xy / mTextureSize; // Calculate position in coordinate textures for the texture we are processing

    vec3 WorldPos = texture(mPositionMap, TexCoord).xyz;
    vec3 Normal = texture(mNormalMap, TexCoord).xyz; // Get data from the textures 

    vec3 DiffuseColor = vec3(0.0, 0.0, 0.0);

    if(mUseDiffuse)
    {
        DiffuseColor = texture(mDiffuseColorMap, TexCoord).xyz;
    }

    // Diffuse is the default color any other has to be especific
    vec3 AmbientColor = DiffuseColor;
    vec3 ReflectiveColor = DiffuseColor;
    vec3 EmissiveColor = vec3(0.0, 0.0, 0.0);

    if(mUseAmbient)
    {
        AmbientColor = texture(mAmbientColorMap, TexCoord).xyz;
    }

    if(mUseReflective)
    {
        ReflectiveColor = texture(mReflectiveColorMap, TexCoord).xyz;
    }

    if(mUseEmissive)
    {
        EmissiveColor = texture(mEmissiveColorMap, TexCoord).xyz;
    }

    // Fun with vectors
    Normal = normalize(Normal);
    vec3 lightDir = normalize(mLight.position - WorldPos);
    vec3 viewDir = normalize(mViewPos - WorldPos);
    vec3 halfwayDir = normalize(lightDir + viewDir); 

    // Attenuation
    float distance  = length(mLight.position - WorldPos);
    float lightAtt = 1.0/( mLight.attenuation.constant + mLight.attenuation.linear * distance + mLight.attenuation.quadratic * distance * distance);

    // ambient
    vec3 ambient = mLight.ambient;
    
    // diffuse 
    float diff = max(dot(Normal, lightDir), 0.0);
    vec3 diffuse = mLight.diffuse * diff;

    // specular
    vec3 reflectDir = reflect(-lightDir, Normal);  
    
    float spec = max(dot(Normal, halfwayDir), 0.0);
    vec3 specular = mLight.specular * spec; 
    
    vec3 result = (AmbientColor * ambient) + (DiffuseColor * diffuse) + (ReflectiveColor * specular);
    FragColor = vec4( (result * lightAtt) + EmissiveColor, 1.0);
}