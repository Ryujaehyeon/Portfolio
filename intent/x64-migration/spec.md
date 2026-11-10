# Spec: x64-migration

Author: Ryu Jahyeon
Status: draft

## 요구사항

| ID | 요구사항 | 판정 방법 |
|---|---|---|
| R1 | 솔루션에 `Debug|x64`·`Release|x64` 가 있고, Client·MFC_000 모두 4개 구성(Win32·x64 × Debug·Release)이 빌드된다 | MSBuild 4회 종료 코드 0 |
| R2 | x64 산출물은 64비트 이미지다 | `file Exe/x64/*/Client/Client.exe` → `PE32+` |
| R3 | 출력 폴더에 **플랫폼에 맞는 FMOD DLL 하나만** 있다: Win32 → `fmodexL.dll`, x64 → `fmodex64.dll` | 출력 폴더 목록 |
| R4 | x64 Client 실행: 로고→메뉴→스테이지, 배경음 재생, 맵 타일 표시, 몬스터·스킬 동작, 오류 메시지박스 없음 | 수동 실행 |
| R5 | x64 MFC_000 이 32비트 시절 저장된 `Client/Map.dat` 를 열 수 있다 | 툴 「타일 불러오기」 |
| R6 | Win32 회귀 없음 | Win32 로 R4 반복 |
| R7 | 새 경고는 x64 의 C4267(약 13곳)뿐, 포인터 잘림 경고(C4311·C4312·C4302) 0 | 빌드 로그 |
| R8 | CLAUDE.md 빌드 섹션에 x64 명령·경로·플랫폼별 DLL 이 반영된다 | 문서 검토 |

선행 조건: [debug-dll-removal](../debug-dll-removal/spec.md) 머지 — 32비트 전용 `Debug.lib` 가 유일한 차단 요소.

## 설계

- **x64 구성은 VS 구성 관리자로 Win32 를 복제** — 손으로 조건 블록을 복사하다 XML 을 깨는 위험(MSB4025)을 피한다.
  복제 결과는 Win32 와 블록 수가 같아야 한다(WIP 커밋 `6c6b963` 에서 두 프로젝트 모두 10:10 확인).
- **FMOD DLL 선택은 `Client.vcxproj` 의 `$(FmodDll)` 속성** — 플랫폼 조건 속성 2줄 + Post-Build 1줄.
  `Directory.Build.props` 에 두지 않는다: 그 파일은 모든 프로젝트 공통(출력 경로) 책임이고 FMOD 는 Client 전용(SRP).
- **코드 변경 없음** — `GameSound.hpp:5-8` 이 `_WIN64` 분기로 이미 lib 를 고르고, `MFC_000/stdafx.h:52` 가 amd64 매니페스트를 이미 가진다.
  D3DX NuGet 은 `$(Platform)` 으로 x64 lib·DLL 을 자동 선택, `Directory.Build.props` 경로는 이미 `$(Platform)` 포함.
- **데이터 포맷 유지** — `TILE`(`Struct.h:216`)·`OBJINFO`(`Struct.h:12`)에 포인터·`size_t` 가 없어 x86·x64 크기가 같다.
  `Map.dat` 12,800B = 800타일 × 16B 로 확인.
- **Win32 는 당분간 유지** — 두 플랫폼을 비교 검증할 수 있게. 제거 여부는 x64 검증 후 별도 결정(YAGNI 의 반대편: 되돌리기 쉬운 쪽).

사전 근거(2026-11-10): 임시 worktree 에서 x64 시험 빌드·실행 — `DebugMsg` 1곳 대체 + `fmodex64.dll` 배치 시
Debug·Release 모두 창 응답, 텍스처 로드(120~136MB). MFC_000 은 수정 없이 x64 빌드 성공.

## flag된 우려사항

| 무엇이 | 어느 기준에 | 해소 |
|---|---|---|
| Win32 를 남길지 | 유지보수 비용 | **개발자 결정**. 권장: x64 검증 후 별도 커밋으로 결정 |
| 기본 플랫폼(처음 열 때 선택) | 사용성 | 활성 플랫폼은 `.vs/` 사용자 상태 — 커밋 대상 아님. 각자 구성 관리자에서 선택 |
| x64 는 FMOD 비로깅판(`fmodex64`), Win32 는 로깅판(`fmodexL`) | 동작 일관성 | 수용. API 동일, 로그만 차이 |
| 이후 `TILE`/`OBJINFO` 에 포인터·`size_t` 추가 시 플랫폼 간 `.dat` 호환 깨짐 | 데이터 호환 | CLAUDE.md 「건드리면 안 되는 부분」 구조체 경고 유지 |
| WIP 브랜치와 `debug-dll-removal` 이 같은 vcxproj·sln 수정 | 작업 충돌 | `debug-dll-removal` 머지 후 `git rebase master`, 충돌은 Post-Build 줄 위주 |
| 구성 관리자 복제는 Win32 의 잘못된 설정도 복제 | 정확성 | plan 1단계 확인 항목(라이브러리 목록 육안 확인) |
