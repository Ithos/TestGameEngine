#version 330 core
struct Light {  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 direction;
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
uniform sampler2D mShadowMap;

uniform vec2 mTextureSize;

uniform highp mat4 mLightSpaceMatrix;
uniform float mMaxBias;

layout (location = 0) out vec4 FragColor;

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    if(projCoords.z > 1.0)
        return 0.0;

    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    // bias deals with shadow acne 
    float cosTheta = clamp( dot(normal, lightDir), 0, 1);
    float bias = 0.0005*tan( acos( cosTheta) );
    bias = clamp(bias, 0, mMaxBias); // 0.0001
    float shadow = 0.0;

    vec2 texelSize = 1.0 / textureSize(mShadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            // check whether current frag pos is in shadow
            float pcfDepth = texture(mShadowMap, projCoords.xy + vec2(x, y) * texelSize).z; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;

    return shadow;
}  

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

    if(mUseAmbient)
    {
        AmbientColor = texture(mAmbientColorMap, TexCoord).xyz;
    }

    if(mUseReflective)
    {
        ReflectiveColor = texture(mReflectiveColorMap, TexCoord).xyz;
    }

	// Fun with vectors
	Normal = normalize(Normal);
    vec3 lightDir = normalize(-mLight.direction);
    vec3 viewDir = normalize(mViewPos - WorldPos);
    vec3 halfwayDir = normalize(lightDir + viewDir); 

    // ambient
    vec3 ambient = mLight.ambient;
  	
    // diffuse 
    float diff = max(dot(Normal, lightDir), 0.0);
    vec3 diffuse = mLight.diffuse * diff;

    // specular
    float spec = max(dot(Normal, halfwayDir), 0.0);
    vec3 specular = mLight.specular * spec;  

    // calculate shadow
    float shadow = ShadowCalculation( mLightSpaceMatrix * vec4(WorldPos, 1.0), Normal,  lightDir);
        
    vec3 result = (AmbientColor * ambient) + (1.0 - shadow) * ( (DiffuseColor * diffuse) + (ReflectiveColor * specular) );
    FragColor = vec4(result, 1.0);
}