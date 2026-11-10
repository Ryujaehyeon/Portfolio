# Intent: 저장소 정리 (커밋된 산출물·미사용 바이너리)

Author: Ryu Jahyeon
Status: draft

## Problem
2016년 빌드 산출물·VS 사용자 상태 파일·쓰지 않는 DLL/lib·데이터가 저장소에 커밋돼 있다(15개, 약 6MB).
VS2026 전환으로 산출물은 `Exe/` 로 나오고 gitignore 되므로 이 파일들은 아무 역할이 없고,
어떤 바이너리가 실제로 필요한지 저장소만 봐서는 알 수 없다.
근거: [legacy-audit/review.md](../legacy-audit/review.md) 7번.

## Proposed outcome
- 저장소에 남은 바이너리는 **빌드·실행에 실제로 필요한 것만**: `Client/Debug.lib`·`Client/Debug.dll`(8번에서 제거 예정),
  `Client/fmodexL.dll`, `Client/fmod/lib/fmodexL_vc.lib`, `Client/Map.dat`.
- `.gitignore` 가 VS 사용자 파일을 빠짐없이 막고, VS2010 시절의 낡은 경로 규칙이 사라진다.
- 동작은 바뀌지 않는다.

## Affected users and systems
사용자: 이 repo 개발자, GitHub 열람자(저장소 크기·잡음 감소)
시스템: git 추적 목록, `.gitignore`. 소스·프로젝트 파일은 건드리지 않는다.

## Constraints
반드시 지킬 것: 소스(`.cpp/.h`)·`.vcxproj`·`Resource/` 는 수정하지 않는다. 빌드·실행 결과가 같아야 한다.
범위 밖: `Debug.dll`/`Debug.lib` 제거(→ `debug-dll-removal`), `Resource/` 미사용 파일 정리(런타임 조사 필요),
git 이력에서 바이너리를 지우는 이력 재작성(용량 이득이 작고 위험이 큼).

## Open questions
1. `Client/Monster.dat`(0 바이트)·`Client/Monster1.dat`(464 바이트): Client 는 읽지 않는다(`StageBackground.cpp:25` 가 `Map.dat` 만 연다).
   MFC 툴(`MFC_000/Page2.cpp:122`)로 저장한 몬스터 배치 샘플일 수 있다 → **삭제 / `Data/` 등으로 이동 / 유지** 중 결정.
2. FMOD x64 파일(`fmodex64.dll`, `fmodex64_vc.lib`, `fmodexL64.dll`): 지금 x64 플랫폼이 없어 안 쓴다.
   FMOD Ex 는 단종돼 다시 받기 어렵지만 **git 이력에 남으므로** 필요하면 복원 가능 → 삭제 권장. x64 계획이 가까우면 유지.
