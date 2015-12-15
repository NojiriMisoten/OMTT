float4x4 gWVP;
float4x4 g_World;

float shiftV = 0.0f;

void main(
	in float3 inPos:POSITION0,
	in float3 inNormal : NORMAL0,
	in float2 inUV : TEXCOORD0,
	out float4 outPos : POSITION0,
	out float2 outUV : TEXCOORD0,
	out float4 outPosW : TEXCOORD1
	)
{
	outPos = mul(float4(inPos, 1.0f), gWVP);
	outPosW = mul(float4(inPos, 1.0f), g_World);;
	
	inUV.x += shiftV;
	outUV = inUV;
}
