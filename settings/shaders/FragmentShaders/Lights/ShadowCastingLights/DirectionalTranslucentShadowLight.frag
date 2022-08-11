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

uniform sampler2D mTranslucentShadowMap;
uniform sampler2D mSpecularTranslucentMap;
uniform sampler2D mDiffuseTranslucentMap;


uniform vec2 mTextureSize;

uniform highp mat4 mLightSpaceMatrix;
uniform float mMaxBias;

layout (location = 0) out vec4 FragColor;

void main() {
    vec2 TexCoord = gl_FragCoord.xy / mTextureSize; // Calculate position in coordinate textures for the texture we are processing

    vec3 WorldPos = texture(mPositionMap, TexCoord).xyz;
    vec3 Normal = texture(mNormalMap, TexCoord).xyz; // Get data from the textures
    float shininess = length(Normal);

    vec3 DiffuseColor = mUseDiffuse ? texture(mDiffuseColorMap, TexCoord).xyz : vec3(0.0, 0.0, 0.0);

    // Diffuse is the default color any other has to be especific
    vec3 AmbientColor = mUseAmbient ? texture(mAmbientColorMap, TexCoord).xyz : DiffuseColor;
    vec3 ReflectiveColor = mUseReflective ? texture(mReflectiveColorMap, TexCoord).xyz : DiffuseColor;

    // Fun with vectors
    vec3 normNormal = normalize(Normal);
    vec3 lightDir = normalize(-mLight.direction);
    vec3 viewDir = normalize(mViewPos - WorldPos);
    vec3 halfwayDir = normalize(lightDir + viewDir); 

    // ambient
    vec3 ambient = mLight.ambient;
    
    // diffuse 
    float diff = max(dot(normNormal, lightDir), 0.0);
    vec3 diffuse = mLight.diffuse * diff;

    // specular
    float spec = pow(max(dot(normNormal, halfwayDir), 0.0), shininess);
    vec3 specular = mLight.specular * spec;  

    vec4 fragLightPos = mLightSpaceMatrix * vec4(WorldPos, 1.0);

    // calculate shadow
    /////////////////////////////////////////////////////////////////

    // perform perspective divide
    vec3 projCoords = fragLightPos.xyz / fragLightPos.w;

    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    // bias deals with shadow acne 
    float cosTheta = clamp( dot(normNormal, lightDir), 0, 1);
    float bias = 0.0005*tan( acos( cosTheta ) );
    bias = clamp(bias, 0,mMaxBias); //0.0001
    float shadow = 0.0;

    vec2 texelSize = 1.0 / textureSize(mShadowMap, 0);
    float depthDiff = (currentDepth - bias) ;
    for(int x = -1; (projCoords.z <= 1.0) && x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            // check whether current frag pos is in shadow
            float pcfDepth = texture(mShadowMap, projCoords.xy + vec2(x, y) * texelSize).z; 
            shadow += ( depthDiff > pcfDepth ? 1.0 : 0.0);        
        }    
    }
    shadow /= 9.0;
    ////////////////////////////////////////////////////////////////////////////////////////////

    // get translucent filter

    float pcfDepth = texture( mTranslucentShadowMap, projCoords.xy ).z; 
    bool check = (depthDiff > pcfDepth);

    specular = check ? specular * texture(mSpecularTranslucentMap, projCoords.xy).xyz  : specular;
    diffuse = check ? diffuse * texture(mDiffuseTranslucentMap, projCoords.xy).xyz : diffuse;
        
    vec3 result = (AmbientColor * ambient) + (1.0 - shadow) * ( (DiffuseColor * diffuse) + (ReflectiveColor * specular) );
    FragColor = vec4(result, 1.0);
}