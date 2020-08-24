#include <BasicShader.hlsli>

cbuffer ConstantBufferForLighting : register(b0)
{
	float3 lightPosition;
	float4 diffuseLightColor;
	float4 specularLightColor;
	float4 ambientLightColor;

	float3 cameraPosition;
	//マテリアル プロパティ
	float specularPower;
	float specularIntensity;
};

Texture2D diffuseTexture;
SamplerState diffuseTextureSampler;

float4 main(PixelShaderInput input) : SV_TARGET
{
	//ピクセル単位のディフューズを計算する
	float3 directionToLight = normalize(lightPosition - input.worldPosition);
	float diffuseIntensity = saturate(dot(directionToLight, input.worldNormal));
	float4 diffuse = diffuseLightColor * diffuseIntensity;

	//ピクセル単位の Phong 成分を計算する
	float3 reflectionVector = normalize(reflect(-directionToLight, input.worldNormal));
	float3 directionToCamera = normalize(cameraPosition - input.worldPosition);

	//スペキュラー成分を計算する
	float4 specular = specularLightColor * specularIntensity *
		pow(saturate(dot(reflectionVector, directionToCamera)),
			specularPower);

	float4 texel = diffuseTexture.Sample(diffuseTextureSampler, input.texCoord);

	float4 color = diffuse + ambientLightColor;
	color = color + specular;
	color.a = 1.0;
	color.xyz = color.xyz * texel.xyz;
	color.a = color.a * texel.a;

	return color;
}