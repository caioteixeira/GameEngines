// We want to use row major matrices
#pragma pack_matrix(row_major)

cbuffer PER_CAMERA : register(b0)
{
	float4x4 projMatrix;
	float3 worldCameraPos;
}

cbuffer PER_OBJECT : register(b1)
{
	float4x4 worldMatrix;
}

struct PointLightData {
	float3 diffuseColor;
	float3 specularColor;
	float3 position;
	float specularPower;
	float innerRadius;
	float outerRadius;
	bool enabled;
};

cbuffer LIGHTING : register(b2)
{
	float3 ambientLight;
	PointLightData pointLights[8];
}

SamplerState DefaultSampler : register(s0);
Texture2D DiffuseTexture : register(t0);