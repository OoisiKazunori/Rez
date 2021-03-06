#include"../ShaderHeader/BasicShaderHeader.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 PSmain(VSOutput input) : SV_TARGET
{
    float3 light = normalize(float3(1, -1, 1)); //右下奥向きのライト
	//環境光
    float diffuse = saturate(dot(-light, input.normal)); //diffuseを[0,1]の範囲にClampする
    float brightness = diffuse + 0.3f;
	//texture
    float4 texColor = float4(tex.Sample(smp, input.uv));
    return float4(texColor.rgb * brightness, texColor.a);
	//return float4(input.uv, 1, 1);
}