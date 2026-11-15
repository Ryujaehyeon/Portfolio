# Intent: D3D11 + DirectXTK 백엔드(B)

Author: Ryu Jahyeon
Status: draft

> 렌더링 전환 로드맵의 3단계. 전체 흐름은 [RENDER-ROADMAP](../../docs/RENDER-ROADMAP.md).

## Problem

렌더링이 Direct3D 9 와 D3DX 에 묶여 있다.

- **D3DX 는 폐기된 라이브러리다.** Windows 8 SDK 부터 제공되지 않아, 지금은 NuGet(`Microsoft.DXSDK.D3DX`)으로 따로 받아 쓴다.
  `ID3DXSprite`·`D3DXCreateTextureFromFileEx` 를 대신할 공식 경로는 DirectXTK 와 WIC 다.
- **D3D11 을 배울 기준이 없다.** 로드맵 4단계에서 D3D11 스프라이트 렌더러(A)를 직접 구현할 계획이다.
  처음 구현할 때 가장 어려운 것은 「무엇이 맞는 결과인지」를 모르는 상태다. 같은 일을 하는 검증된 구현이 옆에 있어야 한다.

## Proposed outcome

- Client 를 `-d3d11` 로 실행하면 Direct3D 11 과 DirectXTK 의 `SpriteBatch` 로 그린다. 인자가 없으면 지금처럼 D3D9 로 그린다.
- D3D11 화면이 D3D9 화면과 같다. 텍스처 필터링에서 오는 작은 차이만 허용하고, 그 허용 범위를 수치로 정해 기록한다.
- 이 백엔드가 4단계(A)의 정답이자 비교 기준이 된다.
- 툴은 지금처럼 D3D9 로 그리고, D3D11·DirectXTK 를 링크하지 않는다.

## Affected users and systems

- 사용자: 이 repo 개발자. D3D11 경로를 쓸 수 있고, A 를 구현할 때 기준이 생긴다.
- 게임 사용자: 기본 동작은 변화 없음(`-d3d11` 을 줄 때만 바뀐다).
- 시스템: `Engine/` 에 D3D11 백엔드와 이미지 디코더, Client 의 백엔드 선택(`MainGame`·`Client.cpp`), NuGet(DirectXTK), vcxproj, CLAUDE.md·ARCHITECTURE.md.

## Constraints

반드시 지킬 것:
- 게임 코드는 고치지 않는다. 2단계의 인터페이스를 구현하기만 한다.
- `-d3d11` 스모크가 4개 구성에서 PASS. 경고 0.
- 경고를 끄지 않는다. DirectXTK 헤더에서 경고가 나면 사용자 결정을 받는다(CLAUDE.md 규칙상 `#pragma warning(disable)`·`/wd` 를 쓸 수 없다).

범위 밖:
- 전체 화면 모드(Client 는 창 모드만 쓴다).
- 툴의 D3D11 전환, D3D9 백엔드 제거.
- 수학 라이브러리 교체.

## Open questions

없음 — 로드맵에서 정했다(2026-11-12, B 로 먼저 만들고 A 는 그 옆에 직접 구현).
