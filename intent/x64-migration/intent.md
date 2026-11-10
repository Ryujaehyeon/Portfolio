# Intent: x64 플랫폼 전환

Author: Ryu Jahyeon
Status: draft

## Problem
솔루션이 `Win32` 플랫폼만 가진다. 32비트 앱은 주소 공간이 4GB(대형 주소 미지정 시 2GB)로 제한되고,
최신 툴체인·라이브러리(DirectX 11/12 전환 포함)는 x64 를 기본으로 한다. 포트폴리오로서도 x64 빌드가 기본이 돼야 한다.

## Proposed outcome
- Client·MFC_000 이 `Debug|x64`·`Release|x64` 로 빌드·실행된다(소리·맵 포함).
- 산출물은 기존 규칙대로 `Exe\x64\<구성>\<프로젝트>\`.
- 플랫폼마다 맞는 런타임 DLL(FMOD 32/64)이 자동으로 배치된다.

## Affected users and systems
사용자: 이 repo 개발자
시스템: `MyProgram.sln`, `Client.vcxproj`, `MFC_000.vcxproj`(플랫폼 구성·Post-Build)

## Constraints
반드시 지킬 것: 게임 동작·데이터 파일(`Map.dat`) 호환 유지.
선행 조건: **`debug-dll-removal` 완료** — 32비트 전용 `Debug.lib`/`Debug.dll`(소스 없음)이 x64 링크를 막는 유일한 요소.
범위 밖: C4267 경고 정리(→ `warning-cleanup`), DirectX 버전 전환.

## 사전 조사 결과 (2026-11-10, 임시 worktree 에서 x64 시험 빌드·실행 후 폐기)

| 구성 요소 | x64 | 근거 |
|---|---|---|
| `Debug.lib`/`Debug.dll` | **불가** | PE32(x86), 소스 없음. 링크 시 LNK4272 + `DebugMsgw` LNK2019 |
| FMOD Ex | 가능 | `fmodex64.dll`(PE32+) + `fmod/lib/fmodex64_vc.lib`. `GameSound.hpp:5-6` 이 `_WIN64` 에서 이미 선택 |
| D3DX9 (NuGet) | 가능 | 패키지에 `lib/x64`·`bin/x64` 포함, `$(Platform)` 으로 자동 선택 |
| d3d9 / dinput8 / dxguid / MFC | 가능 | Windows SDK·VS 기본 제공. MFC_000 은 x64 빌드 바로 성공 |
| 코드 | 가능 | 새 경고는 C4267 13곳뿐 — 전부 `int n = container.size();` 형태(값이 작아 무해) |
| 데이터 | 호환 | `TILE`(`Struct.h:216`)·`OBJINFO`(`Struct.h:12`)에 포인터 없음 → 크기 동일. `Map.dat` 12800B = 800타일×16B |

시험: `DebugMsg` 1곳을 `OutputDebugStringW` 로 대체하고 `fmodex64.dll` 을 배치 →
x64 Client Debug·Release 모두 창 응답, 메모리 120~136MB(텍스처 로드).

## Open questions
1. **Win32 를 남길지**: 남기면 두 플랫폼을 계속 빌드·검증해야 한다. 지우면 단순해지지만 32비트 FMOD 로깅판(`fmodexL`)을 잃는다.
   권장: 전환 직후엔 둘 다 두고, x64 검증이 끝나면 별도 커밋으로 Win32 제거 여부 결정.
2. **기본 플랫폼**: VS 에서 처음 열 때 선택될 플랫폼을 x64 로 할지(구성 관리자에서 x64 를 활성으로 저장).
