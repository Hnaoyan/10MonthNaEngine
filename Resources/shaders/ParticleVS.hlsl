//#include "Particle.hlsli"

//VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD, uint instanceId : SV_InstanceID)
//{
//    //
//    float4 worldNormal = normalize(mul(float4(normal, 0), gTransform[instanceId]));
//    float4 worldPos = mul(pos, gTransform[instanceId]);

//    VSOutput output;
//    output.svPos = mul(pos, mul(gTransform[instanceId], mul(view, projection)));

//    output.worldPos = worldPos;
//    output.normal = worldNormal.xyz;
//    output.uv = uv;

//    return output;
//}