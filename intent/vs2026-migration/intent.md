# Intent: VS2026 빌드 환경 전환

Author: Ryu Jahyeon
Status: approved (사용자, 2026-11-10 대화 승인)

## Problem
솔루션이 VS2010(v100) + DirectX SDK (June 2010) 기준이라 현재 PC(VS 18 / v145, DXSDK 미설치)에서
빌드되지 않는다. `Client\stdafx.h(27,10): error C1083: 'd3dx9.h'`. 빌드가 안 되니 이후 코드 리뷰·수정을
검증할 수단이 없다.

## Proposed outcome
- VS2026(v145)에서 Client·MFC_000 의 Debug|Win32·Release|Win32 가 별도 SDK 설치 없이 빌드된다.
- 산출물은 `Exe\<플랫폼>\<구성>\<프로젝트>\`, 중간 파일은 `Temp\<플랫폼>\<프로젝트>\<구성>\` 에 모인다.
- 각 앱 폴더에 실행에 필요한 DLL 이 함께 놓인다.

## Affected users and systems
사용자: 이 repo 개발자(본인)
시스템: `MyProgram.sln`, `Client.vcxproj`, `MFC_000.vcxproj`, 빌드 설정 파일

## Constraints
반드시 지킬 것: 게임 동작을 바꾸지 않는다. 코드 수정은 새 컴파일러에서 빌드되게 하는 최소한만.
범위 밖: DirectX 버전 전환(DX11/12), x64 플랫폼 추가, 경고 정리, `/permissive-` 적용,
exe 위치 기준 리소스 경로 처리, 커밋돼 있는 바이너리·`.vs/` 정리 → 리뷰 백로그로.

## Open questions
없음.
