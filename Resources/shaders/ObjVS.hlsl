#include "obj.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	// 法線にワールド行列を適用
    float4 worldNormal = normalize(mul(float4(normal, 0), world));
    // ワールド行列による座標計算
    float4 worldPos = mul(pos, world);
	
    VSOutput output;// ピクセルシェーダに送る値
    output.svPos = mul(pos, mul(world, mul(view, projection)));
    
    output.worldPos = worldPos;
    output.normal = worldNormal.xyz;
    output.uv = uv;
	
	return output;
}