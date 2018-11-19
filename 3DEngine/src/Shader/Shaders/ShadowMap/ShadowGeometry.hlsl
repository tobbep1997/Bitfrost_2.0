#include "../StaticConstantBuffers.hlsli"

cbuffer LIGHT_INDEX : register(b13)
{
    uint4 LIGHT_POS;    
    uint4 sides[8];
};

struct GSOutput
{
	float4 pos : SV_POSITION;
	uint RTIndex : SV_RenderTarGetArrayIndex;
};

[maxvertexcount(6 * 3)]
void main(
	triangle float4 input[3] : SV_POSITION,
	inout TriangleStream< GSOutput > output
)
{
    for (int tarGetMatrix = 0; tarGetMatrix < numberOfViewProjection[LIGHT_POS.x].x; tarGetMatrix++)
    {
        if(sides[tarGetMatrix].x == 0)
            continue;
        GSOutput element;
        element.RTIndex = tarGetMatrix;

        for (int vertex = 0; vertex < 3; vertex++)
        {
            element.pos = mul(input[vertex], lightViewProjection[LIGHT_POS.x][tarGetMatrix]);
            output.Append(element);
        }			
		output.RestartStrip();
    }
}