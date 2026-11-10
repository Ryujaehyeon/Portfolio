# Plan: vs2026-migration

Author: Ryu Jahyeon
Status: approved (사용자, 2026-11-10 대화 승인) · 구현 완료

## 영향받는 파일

- `2D-Portfolio/MyProgram.sln` — 헤더를 Format Version 12.00 / Visual Studio Version 18 로.
- `2D-Portfolio/Directory.Build.props` (신규) — 모든 프로젝트·구성의 OutDir/IntDir 를 한 곳에서 지정.
- `2D-Portfolio/Client/Client.vcxproj`, `2D-Portfolio/MFC_000/MFC_000.vcxproj`
  - `ToolsVersion="4.0"` 제거, `VCProjectVersion 18.0`, `WindowsTargetPlatformVersion 10.0`, `PlatformToolset v145`.
  - DXSDK June 2010 하드코딩 `IncludePath`/`LibraryPath` 제거 → NuGet `Microsoft.DXSDK.D3DX 9.29.952.8` import.
  - `d3dx9.lib` 직접 링크 제거 — 패키지가 구성에 맞는 `d3dx9.lib`/`d3dx9d.lib` 를 넣는다.
  - Client: Release 구성에 빠져 있던 `dxguid.lib;dinput8.lib` 추가(기존 결함, Release 링크 실패 원인).
  - Client: Post-Build 로 `Debug.dll`, `fmodexL.dll` 을 OutDir 로 복사.
- `2D-Portfolio/{Client,MFC_000}/packages.config` (신규).
- `2D-Portfolio/Headers/Debug.hpp` — `#include <string>` 추가. 최신 STL 은 `<string>` 을 간접 포함하지 않아 MFC_000 Debug 가 C2039.
- `.gitignore` — `Exe/`, `Temp/`, `packages/`, `.vs/`, `*.user`.

## 작업 순서

1. 하네스 도입분 커밋 분리 → `git log` 로 확인.
2. Directory.Build.props·packages.config·vcxproj·sln 수정 → `git diff` 검토.
3. NuGet 복원 → `packages/Microsoft.DXSDK.D3DX.9.29.952.8/build/native/*.targets` 존재 확인.
4. Debug·Release 빌드 → 오류 0, 종료 코드 0.
5. 실행 스모크: 작업 디렉터리 `Client/`·`MFC_000/` 에서 각 exe 를 8초 실행 → 응답 중, 오류 메시지박스 없음.

## 테스트

```
MSBuild.exe 2D-Portfolio/MyProgram.sln -t:restore -p:RestorePackagesConfig=true
MSBuild.exe 2D-Portfolio/MyProgram.sln -p:Configuration=Debug   -p:Platform=Win32 -m
MSBuild.exe 2D-Portfolio/MyProgram.sln -p:Configuration=Release -p:Platform=Win32 -m
```
기대: 두 구성 모두 종료 코드 0, `Exe/Win32/<구성>/{Client,MFC_000}/*.exe` 생성.

결과(2026-11-10): Debug·Release 모두 종료 코드 0. 경고 약 280건(C4244·C4002·C4838 등, 기존 코드) — 백로그.
스모크: Client(Debug/Release) 창 응답·메모리 약 70MB(텍스처 로드), MFC_000 "제목 없음 - MFC_000" 창 응답.

## 리스크

- 리소스는 작업 디렉터리 기준 상대 경로(`../Resource/...`, `Map.dat`)라 `Exe\...\Client.exe` 를 더블클릭하면
  리소스를 못 찾는다. VS 디버거는 작업 디렉터리 기본값 `$(ProjectDir)` 이라 정상. → 백로그.
- NuGet 패키지가 쓰지 않는 `d3dx10/11`, `D3DCompiler_43.dll` 도 복사한다. 동작 영향 없음.
- `fmodexL.dll` 은 FMOD 로깅 빌드다(Win32 에서 `fmodexL_vc.lib` 링크). 기존 동작 그대로 유지.
