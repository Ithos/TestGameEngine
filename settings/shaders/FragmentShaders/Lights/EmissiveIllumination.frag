#version 330 core

uniform sampler2D mEmissiveColorMap;
uniform vec2 mTextureSize;
layout (location = 0) out vec4 FragColor;

void main() {
    vec2 TexCoord = gl_FragCoord.xy / mTextureSize; // Calculate position in coordinate textures for the texture we are processing
    FragColor = texture(mEmissiveColorMap, TexCoord);
}