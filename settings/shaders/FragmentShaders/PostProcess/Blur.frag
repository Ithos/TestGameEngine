#version 330 core

uniform sampler2D mTexture;
uniform vec2 mTextureSize;
uniform bool isVertical;//Usamos esta variable para saber si estamos realizando un  difuminado vertical u horizontal

layout (location = 0) out vec4 FragColor;

void main(){
	vec2 pos=gl_FragCoord.xy / mTextureSize;
	vec4 result=vec4(0.0, 0.0, 0.0, 0.0);
	
	float value[9];
	value[0]=0.0278;
	value[1]=0.0656;
	value[2]=0.121;
	value[3]=0.1747;
	value[4]=0.1974;
	value[5]=0.1747;
	value[6]=0.121;
	value[7]=0.0656;
	value[8]=0.0278;

	// We are using a 2 step approach for efficiency
	if(isVertical == true ){
		vec2 curPos=vec2(pos.x, pos.y - (4.0 / mTextureSize.y));
		for(int i = 0 ; i < 9; i++){
			result+=texture2D(mTexture,curPos) * value[i];
			curPos.y += (1 / mTextureSize.y);
		}
		//result = (texture2D(mTexture,pos) + vec4(1.0,1.0, 1.0, 1.0));
	}else{
		vec2 curPos=vec2(pos.x - (4.0 / mTextureSize.x), pos.y);
		for(int i = 0; i < 9; i++){
			result+=texture2D(mTexture, curPos) * value[i];
			curPos.x += (1 / mTextureSize.x);
		}
		//result = (texture2D(mTexture,pos) + vec4(0.0, 0.0, 0.5, 1.0));
	}
	
	FragColor=vec4(result.rgb,1.0);

}
