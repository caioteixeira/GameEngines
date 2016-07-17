#include "Constants.hlsl"

// Input structs for vertex and pixel shader
struct VS_INPUT
{
	float3 mPos : POSITION;
	float3 mNormal : NORMAL;
	float2 mTex : TEXCOORD0;
};

struct PS_INPUT
{
	float4 mPos : SV_POSITION;
	float3 mNormal : NORMAL;
	float3 mWorldPosition : POSITION;
	float2 mTex: TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output;

	float4  temp = float4(input.mPos, 1);
	temp = mul(temp, worldMatrix);
	output.mPos = mul(temp, projMatrix);
	output.mTex = input.mTex;
	float4 tempNormal = float4(input.mNormal, 0);
	tempNormal = mul(input.mNormal, worldMatrix);
	//normalize(tempNormal);
	output.mNormal = tempNormal.xyz;
	output.mWorldPosition = temp.xyz;

	return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{
	// Texture color 
	float4 color = DiffuseTexture.Sample(DefaultSampler, input.mTex);

	//ambient light
	float3 phong = ambientLight;
	

	// Point light
	float3 N = normalize(input.mNormal);
	float3 V = worldCameraPos - input.mWorldPosition;
	V = normalize(V);

	for (int i = 0; i < 8; i++) 
	{
		if (!pointLights[i].enabled)
		{
			continue;
		}	

		float3 L = pointLights[i].position - input.mWorldPosition;
		L = normalize(L);
		float NdotL = dot(N, L);

		float3 R = reflect(-L, N);

		if (NdotL > 0) {
			/*Diffuse color*/
			float dist = distance(input.mWorldPosition, pointLights[i].position);
			float sstep = smoothstep(pointLights[i].innerRadius, pointLights[i].outerRadius, dist);

			float3 diffuseColor = lerp(pointLights[i].diffuseColor, float3(0.0f, 0.0f, 0.0f), sstep);
			phong += (diffuseColor * NdotL);

			/*Specular color*/
			float RdotV = dot(R, V);
			phong += pointLights[i].specularColor * pow(max(0.0f, RdotV), pointLights[i].specularPower);
		}
	}

	//saturate phong
	float4 finalPhong = float4(phong, 0.0f);

	return color * saturate(finalPhong);
}

