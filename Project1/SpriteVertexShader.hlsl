//07_02スプライト用追加
#include "SpriteShaderHeader.hlsli"
//
VSOutput VSmain(float4 pos:POSITION,float2 uv : TEXCOORD)
{
	VSOutput output;//ピクセルシェーダーに渡す値
	output.svpos = mul(mat, pos);//座標に行列を乗算 05_02追加
	output.uv = uv;
	return output;
}

//float4 VSmain(float4 pos :POSITION):SV_POSITION
//{
//	float4 a = {1,1,1,1};
//	return pos * a;
//}