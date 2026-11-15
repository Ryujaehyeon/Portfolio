# Intent: 렌더러를 인터페이스 뒤로 옮긴다

Author: Ryu Jahyeon
Status: draft

> 렌더링 전환 로드맵의 2단계. 전체 흐름은 [RENDER-ROADMAP](../../docs/RENDER-ROADMAP.md).

## Problem

게임 코드가 Direct3D 9 를 직접 다룬다. 그래서 그래픽 API 를 바꾸거나 하나 더 지원하려면 게임 코드 전체를 고쳐야 한다.
경로는 `2D-Portfolio/` 기준이다.

- **그리기 호출이 흩어져 있다.** 게임 객체가 `GET_SINGLE(CDevice)->GetSprite()` 로 `ID3DXSprite` 를 꺼내
  `SetTransform`·`Draw` 를 직접 부른다. Client 12개 파일 66곳, 툴 2개 파일 12곳이다.
- **텍스처 구조체가 D3D9 타입이다.** `TEXINFO` 는 `LPDIRECT3DTEXTURE9` 와 `D3DXIMAGE_INFO` 를 담고, 텍스처 클래스(`CSingleTexture`·`CMultiTexture`)가
  `D3DXCreateTextureFromFileEx` 를 직접 부른다.
- **D3D9 전용 설정이 앱에 있다.** Client 의 `CMainGame` 이 `GetDevice()->SetRenderState(D3DRS_ZENABLE, TRUE)` 를 직접 건다.

다음 단계에서 D3D11 백엔드를 두 개(B·A) 추가할 계획이다. 지금 구조 그대로면 같은 78곳을 백엔드마다 다시 고쳐야 한다.

## Proposed outcome

- 게임 코드(Client·툴)는 그래픽 API 를 모른다. 「프레임 시작·끝」, 「텍스처 만들기」, 「스프라이트 그리기」, 「백버퍼 저장」만 렌더러에 요청한다.
- 지금의 D3D9 코드는 그 인터페이스의 첫 구현(`CRendererD3D9`)이 된다.
- **화면은 픽셀 하나 다르지 않다.** 1단계(`render-capture`)의 비교 도구로 변경 전후 캡처가 완전히 같은지 확인한다.
- 다음 단계는 게임 코드를 고치지 않고 구현만 추가하면 된다.

## Affected users and systems

- 사용자: 이 repo 개발자. 백엔드를 추가할 때 게임 코드를 건드리지 않는다.
- 게임 사용자: 변화 없음.
- 시스템: `Engine/` 전체(`CDevice` → 렌더러), `Headers/Struct.h`(`TEXINFO`), Client 그리기 호출부 12개 파일과 `CMainGame`,
  툴의 `MFC_000View.cpp`·`MiniView.cpp`, CLAUDE.md·ARCHITECTURE.md.

## Constraints

반드시 지킬 것:
- 화면이 완전히 같다(캡처 비교, 허용 오차 0). 스모크·툴 스모크 PASS. 경고 0.
- 게임 로직은 바꾸지 않는다. 그리기 호출의 인자(행렬·중심점·원본 사각형·색)는 그대로 옮긴다.
- 수학 타입(`D3DXVECTOR3`·`D3DXMATRIX`)은 그대로 쓴다. 인터페이스도 이 타입을 받는다.
- `TEXINFO` 는 저장 파일(`*.dat`)에 쓰이지 않으므로 바꿔도 데이터 호환에 영향이 없다. 바꾸기 전에 이것을 다시 확인한다.

범위 밖:
- D3D11 구현(3·4단계).
- 수학 라이브러리 교체.
- 툴의 두 창 구조 변경.

## Open questions

없음 — 로드맵에서 정한 순서다(2026-11-12).
