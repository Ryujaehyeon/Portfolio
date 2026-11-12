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
