#include"../ShaderHeader/GPUParticleHeader.hlsli"

float4 PSmain(ColorOutPut input) : SV_TARGET
{
    return matrixData[input.id].color;
}