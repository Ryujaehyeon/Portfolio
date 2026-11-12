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

## 검증 결과 (2026-11-12)

| spec | 결과 |
|---|---|
| R1~R4 | ✅ 코드 검토 — `FrameMove` `>=`, `ClampedAcosf` 4곳, `CPlayer`·`CBoneSpear` `DirectAction` 의 비교식이 `>=`·`<= 360.f` 만 남음(경계값 표대로 어느 각도든 한 범위에 든다) |
| R5 | ✅ 클린 4개 구성 경고 0·오류 0. Client 스모크(입력 분리 상태) x64 Debug 50/50, Win32 Release 50/50, Win32 Debug 5/5, x64 Release 5/5 — 경고창·접근 위반 0. 툴 4개 구성, 일반 실행 4개 구성 × Client·툴 |

- **Release 드문 접근 위반**(smoke-input-isolation 검증 1/50): 디버거를 붙인 Win32 Release 120회에서 재현되지 않아 스택을 잡지 못했다.
  이 수정 후 반복 실행에서 0 이므로 **사용자 판단(2026-11-12)으로 해결된 것으로 본다. 다시 나오면 재현 로그와 함께 새로 다룬다.**
