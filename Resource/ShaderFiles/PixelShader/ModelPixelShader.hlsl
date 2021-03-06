#include"../ShaderHeader/BasicShaderHeader.hlsli"

float4 PSmain(VSOutput input) : SV_TARGET
{

	float3 light = normalize(float3(1,-1,1));
	float brightness = dot(-light, input.normal);

	return float4(brightness, brightness, brightness, 1);
}