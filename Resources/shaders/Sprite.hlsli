cbuffer cbuff0 : register(b0) {
    float4 color;
	matrix mat;
    matrix uv;
}

struct VSOutput {
	float4 svPos : SV_POSITION;
	float2 uv : TEXCOORD;
};