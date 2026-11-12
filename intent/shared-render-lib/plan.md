# Plan: shared-render-lib

Author: Ryu Jahyeon
Status: approved (사용자 결정 — 정적 라이브러리 프로젝트, 2026-11-12)

## 작업 순서

1. `Engine/` 생성 — vcxproj(Client.vcxproj 의 구성·설정을 본뜸, `ConfigurationType=StaticLibrary`)·filters·packages.config·stdafx.h/.cpp, 5개 클래스(Client 사본에서 R3 대로 고침). 솔루션에 추가.
2. Client — 사본 5쌍 삭제, `CInputDevice` 추가, 사용처·`CMainGame` 수정, 포함 경로 `../Engine`, 프로젝트 참조.
3. MFC_000 — 사본 5쌍 삭제, 호출부 10곳·`InitDevice`·`Render_Begin` 수정, 포함 경로, 프로젝트 참조.
4. Win32 Debug 빌드·스모크 → 커밋(단계별) → 클린 4개 구성 검증 → 문서 → PR → 머지.

## 테스트

R6 — CLAUDE.md 「빌드」·「테스트」·「실행」 절차. 툴은 `-smoke` 와 일반 실행(창 뜸).

## 구현 중 변경

- **컬러키 전달 방식** — spec R3 는 `InsertTexture` 의 마지막 인자였지만, 한 장 텍스처는 두 앱 모두 흰색으로 같고 여러 장 텍스처만 다르다.
  인자로 받으면 한 장 텍스처는 그 값을 무시해야 해서, `CTextureMgr::SetMultiColorKey(D3DCOLOR)`(기본 검정)로 앱이 한 번 정하게 했다. 툴은 `InitDevice` 직후 흰색으로.
- `Engine/TextureMgr.cpp` 가 Client 의 헤더 모음 `Include.h` 를 포함하던 것을 `SingleTexture.h`·`MultiTexture.h` 직접 포함으로 — Engine 이 Client 헤더에 기대지 않게.
- 커밋을 빌드 가능한 단위로 나눴다: ① Engine 생성 + Client 전환(툴은 아직 자기 사본), ② 툴 전환 + 사본 삭제.
- 옮긴 클래스만 담던 빈 필터 폴더(Client 3개, 툴 4개)를 정리했다.
- `CInputDevice` 는 새 클래스라 품질 규칙 적용 — 원래 확인하지 않던 DirectInput 생성 실패를 `E_FAIL` 로 돌려주고 `CMainGame` 이 「입력 장치 초기화 실패」로 알린다.

## 검증 결과 (2026-11-12)

| spec | 결과 |
|---|---|
| R1 Engine 프로젝트 | ✅ 정적 라이브러리 4개 구성, 두 앱이 프로젝트 참조로 링크(`Exe/<플랫폼>/<구성>/Engine/Engine.lib`) |
| R2 사본 삭제 | ✅ 5쌍이 `Engine/` 에만(Client 는 `git mv` 로 이력 보존) |
| R3·R4 API·`CInputDevice` | ✅ 코드 검토 |
| R5 동작 불변 | ✅ Client: 창 모드·`ZENABLE`·검정. 툴: 창 모드·파랑·흰색. 툴 화면 캡처로 타일·미니맵이 그려지는 것 확인 |
| R6 | ✅ 클린 4개 구성 경고 0·오류 0(세 프로젝트). Client 스모크 x64 Debug 20/20, Win32 Release 20/20, Win32 Debug 5/5, x64 Release 5/5. 툴 스모크 4개 구성 `tiles=800`. 일반 실행 |
| R7 문서 | ✅ CLAUDE.md(프로젝트 표·싱글턴·「공용 렌더링」), ARCHITECTURE.md(구조·빌드·런타임·툴·알려진 문제) |

- 게임 화면(Client)은 눈으로 확인하지 않았다 — 스모크가 장면·텍스처 조회·스킬까지 지나간다.
