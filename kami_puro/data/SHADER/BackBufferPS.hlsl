//==========================================================
// �萔���W�X�^ CPU������ύX�ł��鐔���Ȃ�����
//==========================================================
sampler toonRenderSampler;
sampler fadeRenderSampler;
sampler lightBloomRenderSampler;
sampler uiRenderSampler;

//**********************************************************
// �G���g���|�C���g
//**********************************************************
float4 main(float4 diffuse : COLOR0, float2 uv : TEXCOORD0) :COLOR0
{
	float4 color = (tex2D(toonRenderSampler, uv))
				+ tex2D(lightBloomRenderSampler, uv)
				+ tex2D(uiRenderSampler, uv)
				+ tex2D(fadeRenderSampler, uv);
	return color;
}

//EOF