# Plan: frame-index-bounds

Author: Ryu Jahyeon
Status: approved (사용자 지시 — 「진행해」, 2026-11-12)

<!-- 입력: spec.md. 경로는 2D-Portfolio/ 기준. -->

## 영향받는 파일

| 파일 | 할 일 |
|---|---|
| `Client/Obj.cpp` | `FrameMove` `>=`(R1) |
| `Headers/Function.h` | `ClampedAcosf`(R2) |
| `Client/{Player,Monster,BoneSpear,StageDynamic}.cpp` | `acosf(` → `ClampedAcosf(`(R2) |
| `Client/Player.cpp`·`BoneSpear.cpp` | `DirectAction` 비교 연산자(R3) |

## 작업 순서

1. R1~R3 수정 → Win32 Debug 빌드·스모크 → 커밋 `fix(frame-index-bounds): 프레임 번호가 텍스처 장 수를 넘지 않게`
2. 클린 4개 구성 빌드, 반복 스모크(R5), 툴·일반 실행 → 기록 → PR → 머지.

## 테스트

- R3: `DirectAction` 의 비교식을 집계해 `m_fChaterDirect >` 가 남지 않는지(`>=` 만), 오른쪽 범위가 `<= 360.f` 인지.
- R5: 반복 스모크.
