# Intent: 애니메이션 프레임 번호가 텍스처 장 수를 넘지 않게

Author: Ryu Jahyeon
Status: draft

## Problem
객체를 그릴 때 **프레임 번호가 그 상태의 텍스처 장 수를 넘는다.** Debug 는 `vector subscript out of range` 경고창으로 멈추고, Release 는 범위 밖 메모리를 읽는다(미정의 동작).
경로는 `2D-Portfolio/` 기준, 2026-11-12 `master`(`86de07d`) 조사.

- **확인된 사례**: member-init 검증 중 x64 Debug 스모크가 경고창으로 멈췄다. 디버거 스택: `CPlayer::Render`(`Player.cpp`) → `CTextureMgr::GetTexture` → `CMultiTexture::GetTexture`(`MultiTexture.cpp:18`, `iter->second[iCnt]`).
  같은 시기 Release 에서 드문 접근 위반(Win32 Release 50회 중 1회, 입력 분리 후)도 나왔다 — 같은 원인인지 디버거로 확인 중.
- 프레임 번호는 `int(m_tFrame.fStart)`, 상태 키는 `DirectAction(모션)` 이 각도별로 정한다(`Player`·`Monster`·`BoneSpear`·`FireWall`·`Blizzard`). 범위를 넘는 경로 세 가지:
  1. **각도 경계값에서 상태 키가 갱신되지 않는다** — 각도 범위가 모두 열린 구간(`> 22.5f && < 67.5f` …, 오른쪽은 `> 0 && < 22.5f || > 337.5f && < 360.f`)이라
     정확히 0°·22.5°·…·360° 이면 어느 분기에도 걸리지 않는다. 특히 **정확히 오른쪽(수평)** 을 보면 각도가 0 또는 360(`2π − 0`)이 되어 빠진다.
     그러면 상태 키가 이전 모션의 것(예: `Stand_*` 8장)으로 남고, 프레임은 새 모션(예: CAST 16)으로 돌아 8 을 넘는다.
  2. **각도가 NaN** — `acosf(D3DXVec3Dot(dir, look))` 의 입력이 부동소수점 오차로 1 을 살짝 넘으면 NaN 이 되어(4곳) 역시 어느 범위에도 걸리지 않는다.
  3. **프레임이 정확히 최댓값** — `CObj::FrameMove` 는 `fStart > fMax` 일 때만 0 으로 되돌려 `fStart == fMax` 면 인덱스가 장 수와 같다. 시전 효과(`CastEffect` 16장 = CAST 16프레임)도 같은 경우에 넘는다.
- 사용자가 플레이 중 정확히 수평 방향을 클릭해도 같은 경로에 닿는다 — 스모크만의 문제가 아니다.

## Proposed outcome
- 어떤 각도(경계값·NaN 포함)에서도 `DirectAction` 이 그 모션의 상태 키를 정한다.
- 프레임 번호가 늘 `0 ≤ 번호 < 최댓값` 이다.
- 4개 구성 스모크 반복 실행(입력 분리 상태)에서 경고창·접근 위반이 나오지 않는다.

## Affected users and systems
사용자: 게임을 하는 사람(드문 멈춤·크래시 제거)
시스템: `Client/Obj.cpp`(`FrameMove`), `Client/{Player,Monster,BoneSpear,FireWall,Blizzard,StageDynamic}.cpp`(각도·`DirectAction`)

## Constraints
반드시 지킬 것:
- 경계가 아닌 각도에서 고르는 상태 키는 지금과 같다(경계값이 어느 쪽에 붙는지만 정한다).
- `fStart <= 0` 을 「한 바퀴 시작」으로 보는 판정(ub-fixes)은 그대로 성립해야 한다.
- 경고 0, 4개 구성 스모크.

범위 밖:
- 텍스처 조회에 범위 검사를 넣어 증상을 감추는 것 — 원인(상태 키·프레임)을 고친다.
- 방향 판정 구조(8방향 표) 재설계.

## Open questions
없음 — 사용자 지시(2026-11-12, 「진행해」).
