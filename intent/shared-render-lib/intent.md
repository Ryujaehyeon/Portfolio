# Intent: Client·툴 렌더링 코드를 공용 정적 라이브러리로

Author: Ryu Jahyeon
Status: draft

## Problem
같은 이름의 렌더링 클래스 5쌍(`CDevice`·`CTexture`·`CTextureMgr`·`CSingleTexture`·`CMultiTexture`)이 Client 와 툴(MFC_000)에 따로 있고 내용이 갈라졌다.
한쪽을 고치면 다른 쪽을 따로 고쳐야 하고(이번 세션의 `wstring` 키·멤버 초기화도 두 번), DirectX 를 올릴 때 두 번 옮겨야 한다.
근거: [legacy-audit/review.md](../legacy-audit/review.md) 9번(「DX 전환 전에 공용 정적 라이브러리로」), 2026-11-12 `master`(`47a56ab`) 비교. 경로는 `2D-Portfolio/` 기준.

- 다른 줄 수(Client/툴): `Device.cpp` 64, `TextureMgr.cpp` 47, `MultiTexture.cpp` 39, `SingleTexture.cpp` 10, 헤더 6~14 — `Texture.cpp` 만 같다.
- 차이의 성격:
  - **텍스처 키**: Client 는 두 단계(`ObjKey` → `StateKey` → 장), 툴은 한 단계(`ObjKey` → 장). Client 쪽이 상위 집합.
  - **`CDevice`**: Client 에만 DirectInput(키보드·마우스)이 들어 있고 창은 전역 `g_hWnd`·창 모드 인자, 툴은 인자 `hWnd`·항상 창 모드. 깊이 렌더 상태는 Client 만, 화면 지우는 색은 Client 검정·툴 파랑.
  - **컬러키**(여러 장 텍스처의 투명색): Client 검정, 툴 흰색.
  - 툴 사본은 텍스처 해제 때 `pTexture->Release()` 를 빠뜨린다(누수).

## Proposed outcome
- 솔루션에 정적 라이브러리 프로젝트 `Engine` 이 생기고, 5개 클래스는 그곳에 한 벌만 있다. Client·툴이 링크한다.
- `CDevice` 는 그리기만 한다 — DirectInput 은 Client 전용 클래스로 옮긴다(다음 `input-unify` 의 출발점).
- 두 앱의 동작은 그대로다: 같은 투명색·지우는 색·깊이 설정, 같은 텍스처를 같은 키로 찾는다.
- 4개 구성 빌드 경고 0, 스모크·일반 실행 PASS.

## Affected users and systems
사용자: 이 repo 개발자(렌더링 수정·DX 전환을 한 곳에서). 게임·툴 사용자에게 보이는 변화 없음
시스템: 새 `Engine/`(vcxproj·packages.config·stdafx·5개 클래스), `MyProgram.sln`, Client·툴 vcxproj·filters, Client 의 입력 사용처, 툴의 텍스처·장치 호출부, CLAUDE.md·`docs/ARCHITECTURE.md`

## Constraints
반드시 지킬 것:
- 동작 불변(위). 4개 구성 × 두 앱. 경고 0·`/WX`·`/permissive-`·출력 경로(`Directory.Build.props`)·NuGet D3DX 관례를 `Engine` 에도.
- 런타임 라이브러리는 세 프로젝트 모두 기본값(`/MD` 계열)으로 같게.

범위 밖:
- 입력 처리 통합(DirectInput + `GetAsyncKeyState`) → `input-unify`. 이번에는 DirectInput 을 Client 전용 클래스로 옮기기만 한다.
- DirectX 버전 전환, 렌더링 기능 변경, 툴 텍스처 키의 의미 변경(툴은 상태 키에 오브젝트 키와 같은 문자열을 쓴다).

## Open questions
없음 — 사용자 결정(2026-11-12): 정적 라이브러리 프로젝트. 텍스처 API 는 Client 의 두 단계 키로 통일, DirectInput 은 Client 전용 입력 클래스로(기본안).
