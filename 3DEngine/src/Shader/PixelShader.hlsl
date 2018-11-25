
struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 worldPos : WORLD;
    float4 normal : NORMAL;
    float2 uv : UV;
};

float4 main(VS_OUTPUT input) : SV_TARGET
{
    return float4(1, 0, 0, 1);

}