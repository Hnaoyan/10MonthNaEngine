#include "Obj.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    // テクスチャマッピング
    float4 texcolor = tex.Sample(smp, input.uv);
    // 透過処理
    if (texcolor.a <= 0.5)
    {
        discard;
    }
	
    const float shininess = 4.0f;
	// 頂点から視点へのベクトル
    float3 eyedir = normalize(cameraPos - input.worldPos.xyz);
	
    // 環境光
    float3 ambient = m_ambient;
	// シェーダーによる色
    float4 shadecolor = float4(ambientColor * ambient, m_alpha);
	
    // 平行光源
    for (int i = 0; i < DIRECTLIGHT_NUM; i++)
    {
        if (directLight[i].active)
        {
            float3 dotLightNormal = dot(-directLight[i].lightv, input.normal);
            
            float3 reflect = normalize(-directLight[i].lightv + 2 * dotLightNormal * input.normal);
            
            float3 diffuse = dotLightNormal * m_diffuse;
            
            float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
            
            shadecolor.rgb += (diffuse + specular) * directLight[i].lightcolor;
        }
    }
	
	// シェーディングの色で描画
    return shadecolor * texcolor;
}