//07_02�X�v���C�g�p�ǉ�
#include "SpriteShaderHeader.hlsli"
//
VSOutput VSmain(float4 pos:POSITION,float2 uv : TEXCOORD)
{
	VSOutput output;//�s�N�Z���V�F�[�_�[�ɓn���l
	output.svpos = mul(mat, pos);//���W�ɍs�����Z 05_02�ǉ�
	output.uv = uv;
	return output;
}

//float4 VSmain(float4 pos :POSITION):SV_POSITION
//{
//	float4 a = {1,1,1,1};
//	return pos * a;
//}