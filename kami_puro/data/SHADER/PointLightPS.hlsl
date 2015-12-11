
float4 worldLightPos[10];
float4 lightColor[10];
float attenuation0;
float attenuation1;
float attenuation2;
float light_num;
sampler texSampler;

// PS
float4 main(
	in float4 inWorldPos : TEXCOORD1,
	in float2 inTexCoord : TEXCOORD0) : COLOR0
{
	float4 outColor;
	float4 color = float4(0, 0, 0, 0);

		// 各ライトの計算
	for (int i = 0; i < light_num; i++){
		float d = distance(worldLightPos[i], inWorldPos);
		float a = attenuation0 + attenuation1 * d + attenuation2 * d * d;
		color = color + float4(lightColor[i].rgb / a, 1);
	}

	// ライトの色計算
	float4 ambient = float4(0.1f, 0.1f, 0.1f, 0.0f);
	outColor = color * tex2D(texSampler, inTexCoord) + ambient;

	return outColor;
}
