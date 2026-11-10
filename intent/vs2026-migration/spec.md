# Spec: vs2026-migration

Author: Ryu Jahyeon
Status: approved (사용자, 2026-11-10) · **소급 작성** — 구현·머지(PR #1) 후 기록으로 남김

## 요구사항

| ID | 요구사항 | 판정 방법 | 결과 |
|---|---|---|---|
| R1 | Client·MFC_000 의 `Debug|Win32`·`Release|Win32` 가 VS2026(v145)에서 오류 없이 빌드된다 | MSBuild 종료 코드 0 | 충족 |
| R2 | DirectX SDK (June 2010) 설치 없이 빌드된다 | DXSDK 미설치 PC 에서 R1 | 충족 |
| R3 | 산출물은 `Exe\<플랫폼>\<구성>\<프로젝트>\`, 중간 파일은 `Temp\<플랫폼>\<프로젝트>\<구성>\` 에만 생긴다 | 빌드 후 `git status` 에 산출물 없음 | 충족 |
| R4 | 각 앱 출력 폴더에 실행에 필요한 DLL 이 함께 놓인다 | 출력 폴더 목록 | 충족 (d3dx9, Debug.dll, fmodexL.dll) |
| R5 | 게임·툴 동작이 바뀌지 않는다 | 작업 디렉터리 `Client/`·`MFC_000/` 에서 실행 스모크 | 충족 (창 응답·텍스처 로드, 오류 메시지박스 없음) |
| R6 | 코드 변경은 새 컴파일러에서 빌드되게 하는 최소한만 | diff 검토 | 충족 (`Debug.hpp` 에 `#include <string>` 1줄) |

## 설계

- **출력 경로는 `Directory.Build.props` 한 곳** — 프로젝트×구성마다 같은 값을 반복하지 않는다(DRY). 새 구성·프로젝트에 자동 적용.
  `$(SolutionDir)` 대신 `$(MSBuildThisFileDirectory)` 를 써서 vcxproj 를 단독 빌드해도 같은 경로가 나온다.
- **D3DX 는 NuGet `Microsoft.DXSDK.D3DX`(packages.config)** — MS 공식 재배포 패키지. C++ 프로젝트는 PackageReference 를
  지원하지 않아 packages.config 를 쓴다. 패키지 targets 가 구성별 lib(`d3dx9.lib`/`d3dx9d.lib`)·include·DLL 복사를 처리하므로
  vcxproj 의 `d3dx9.lib` 직접 지정은 제거(디버그 lib 폴더엔 `d3dx9.lib` 가 없어 링크 오류가 남).
- **플랫폼은 Win32 유지** — x64 는 별도 인텐트(YAGNI: 이번 목표는 "현재 PC 에서 빌드").
- **Post-Build 로 런타임 DLL 복사** — 앱별 출력 폴더가 실행 단위가 되도록.
- 마이그레이션 중 드러난 **기존 결함**(Release 의 `dxguid`/`dinput8` 누락)은 빌드 성공에 필요하므로 범위에 포함.

## flag된 우려사항

| 무엇이 | 어느 기준에 | 해소 |
|---|---|---|
| `Debug.dll` 이 VS2010 디버그 CRT(`MSVCR100D.dll`)에 의존 — 일반 PC 에서 실행 불가 | 배포 가능성 | 이번 범위 밖 → [debug-dll-removal](../debug-dll-removal/spec.md) |
| `ConformanceMode` 꺼짐(`/permissive`) — 비표준 코드 허용 | 코드 품질 | 수용. `const-tchar` 인텐트에서 켬 |
| 리소스가 작업 디렉터리 기준 상대 경로 — exe 더블클릭 시 실패 | 사용성 | 수용. `resource-path` 백로그 |
| NuGet 이 쓰지 않는 `d3dx10/11`, `D3DCompiler_43.dll` 도 복사 | 배포 크기 | 수용(동작 영향 없음) |
