SamplerState defaultSampler;
Texture2D diffuseTexture : register(t1);

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 worldPos : WORLD;
    float4 normal : NORMAL;
    float3x3 TBN : TBN;
    float2 uv : UV;

    float4 color : COLOR;
    int4 info : INFO;
};

float4 main(VS_OUTPUT input) : SV_TARGET
{
    return float4(1, 0, 0, 1);
    return diffuseTexture.Sample(defaultSampler, input.uv);

}