// 스프라이트 정점 셰이더(intent/render-guide). 이 파일은 UTF-8, BOM 없이 저장한다 — fxc 는 BOM 을 읽지 못한다.
// 뼈대: 컴파일만 되는 최소 내용. TODO(2장): 입력·출력 구조체를 정하고 위치를 그대로 넘긴다. TODO(3장): 직교 투영. TODO(4장): UV
float4 main(float2 vPos : POSITION) : SV_POSITION
{
	return float4(vPos, 0.f, 1.f);
}
