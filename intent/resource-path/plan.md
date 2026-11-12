# Plan: resource-path

Author: Ryu Jahyeon
Status: draft

<!-- 입력: spec.md. 경로는 2D-Portfolio/ 기준. -->

## 영향받는 파일

| 파일 | 할 일 |
|---|---|
| `Headers/Function.h` | `SetResourceWorkDir`(R1) |
| `Client/Client.cpp` | `_tWinMain` 맨 앞 호출(R2) |
| `MFC_000/MFC_000.cpp` | 스모크 분기 뒤 호출(R3) |
| `CLAUDE.md`, `docs/FIX-GUIDE.md` | R6, 「다음 순서」 정리 |

## 작업 순서

1. 함수·호출 → Win32 Debug 빌드·스모크 → exe 폴더를 작업 디렉터리로 Client 스모크·툴 일반 실행(R4) → 커밋 `fix(resource-path): 작업 디렉터리가 달라도 리소스 폴더를 찾아 맞춘다`
2. 4개 구성 빌드(R5) → 문서 → PR → 머지.

## 테스트

- R4: `cd Exe/<플랫폼>/<구성>/Client && ./Client.exe -smoke` → 0, 로그는 `Client/smoke.log`(작업 디렉터리가 바뀐 뒤 열리므로).
  툴: PowerShell `Start-Process -WorkingDirectory <exe 폴더>` → 8초 뒤 살아 있고 창 제목이 오류 상자가 아님.
