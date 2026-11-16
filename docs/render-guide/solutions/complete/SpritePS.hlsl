// 스프라이트 픽셀 셰이더(intent/render-guide). 이 파일은 UTF-8, BOM 없이 저장한다 — fxc 는 BOM 을 읽지 못한다.

// [4장] 그릴 텍스처(t0)와 읽는 방법(s0) — C++ 에서 PSSetShaderResources·PSSetSamplers 로 연결한다
Texture2D g_Texture : register(t0);
SamplerState g_Sampler : register(s0);

// [2장] 정점 셰이더의 출력과 같은 모양
struct PS_INPUT
{
	float4 vPos : SV_POSITION;
	float2 vUV : TEXCOORD0;
	float4 vColor : COLOR0;
};

float4 main(PS_INPUT tIn) : SV_TARGET
{
	return g_Texture.Sample(g_Sampler, tIn.vUV) * tIn.vColor;	// [4장] UV 위치의 텍스처 색 × [5장] 스프라이트 색(반투명 등)
}
