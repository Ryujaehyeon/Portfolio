# Plan: source-utf8

Author: Ryu Jahyeon
Status: approved (사용자, 2026-11-10 대화 승인) · 구현 완료

## 영향받는 파일

- `2D-Portfolio/{Client,MFC_000,Headers}/*.{cpp,h}` 중 CP949 70개 + `MFC_000/ReadMe.txt` — CP949 → UTF-8 BOM.
- `.editorconfig` (신규) — `[*.{c,cpp,h,hpp,inl}] charset = utf-8-bom`.
- `CLAUDE.md` — 코드 관례의 인코딩 규칙 갱신.

## 작업 순서

1. 추적 중인 텍스트 파일을 엄격 디코딩으로 분류 → CP949 72, ASCII 85, UTF-16 4, UTF-8 BOM 1.
   UTF-8(BOM 없음)과 섞인 파일 없음 확인.
2. 기준선: `/Brepro /INCREMENTAL:NO` 결정적 리빌드 2회 → Debug·Release 의 exe 4개 해시가 같음(결정성) 확인.
3. 변환: 파일별 cp949 strict 디코드 → BOM + UTF-8 기록. UTF-8 → cp949 되돌림이 원본 바이트와 같은지 assert.
4. 같은 결정적 리빌드 → exe 4개 해시가 기준선과 같아야 함.

## 테스트

```
_CL_=/Brepro _LINK_="/Brepro /INCREMENTAL:NO" (WSLENV 로 전달)
MSBuild.exe 2D-Portfolio/MyProgram.sln -t:Rebuild -p:Configuration={Debug,Release} -p:Platform=Win32
sha256sum 2D-Portfolio/Exe/Win32/*/*/*.exe
```
결과(2026-11-10): 71개 변환, 왕복 검사 전부 통과, diff 는 줄 수 동일(+1482/−1482).
변환 전후 exe 4개 해시 **완전 일치**, C4819 0건 → 리터럴 포함 컴파일 결과 불변.

## 리스크

- BOM 을 모르는 오래된 도구가 첫 줄에서 `﻿` 를 이상한 문자로 볼 수 있다 — MSVC·VS·GitHub 는 문제없다.
- 이후 BOM 없이 저장된 새 파일이 생기면 다시 CP949 로 읽힌다 → `.editorconfig` 와 CLAUDE.md 규칙으로 예방.
