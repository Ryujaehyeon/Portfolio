// 스프라이트 정점 셰이더(intent/render-guide). 이 파일은 UTF-8, BOM 없이 저장한다 — fxc 는 BOM 을 읽지 못한다.

// [3장] 화면 픽셀 좌표 → 클립 좌표로 바꾸는 직교 투영 행렬. C++ 의 XMMATRIX 처럼 행 우선으로 읽는다
cbuffer ProjectionBuffer : register(b0)
{
	row_major float4x4 g_matProjection;
};

// [2장] 정점 하나에 담기는 것 — C++ 의 정점 구조체·입력 레이아웃과 순서·형식이 같아야 한다
struct VS_INPUT
{
	float2 vPos : POSITION;
	float2 vUV : TEXCOORD0;
	float4 vColor : COLOR0;
};

// [2장] 래스터라이저와 픽셀 셰이더로 넘기는 것. SV_POSITION 은 클립 좌표(x·y 가 -1~1 인 화면)
struct VS_OUTPUT
{
	float4 vPos : SV_POSITION;
	float2 vUV : TEXCOORD0;
	float4 vColor : COLOR0;
};

VS_OUTPUT main(VS_INPUT tIn)
{
	VS_OUTPUT tOut;
	tOut.vPos = mul(float4(tIn.vPos, 0.f, 1.f), g_matProjection);	// [3장] 위치는 화면 픽셀 좌표
	tOut.vUV = tIn.vUV;
	tOut.vColor = tIn.vColor;
	return tOut;
}
