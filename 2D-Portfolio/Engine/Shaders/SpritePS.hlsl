// 스프라이트 픽셀 셰이더(intent/render-guide). 이 파일은 UTF-8, BOM 없이 저장한다 — fxc 는 BOM 을 읽지 못한다.
// 뼈대: 컴파일만 되는 최소 내용. TODO(2장): 정점 색. TODO(4장): 텍스처 샘플링. TODO(5장): 색 곱
float4 main() : SV_TARGET
{
	return float4(1.f, 1.f, 1.f, 1.f);
}
