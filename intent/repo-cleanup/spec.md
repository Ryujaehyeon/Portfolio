# Spec: repo-cleanup

Author: Ryu Jahyeon
Status: draft

## 요구사항

| ID | 요구사항 | 판정 방법 |
|---|---|---|
| R1 | 아래 「삭제 목록」의 파일이 git 추적에서 빠진다 | `git ls-files` 에 없음 |
| R2 | 남은 바이너리는 정확히 6개: `Client/Debug.dll`·`Debug.lib`·`fmodexL.dll`·`fmodex64.dll`, `Client/fmod/lib/fmodexL_vc.lib`·`fmodex64_vc.lib` | `git ls-files \| grep -i -E '\.(exe\|dll\|lib\|pdb\|suo\|user)$'` |
| R3 | `.gitignore` 가 `*.suo` 를 막고, VS2010 시절 경로 규칙 4줄이 없다 | `git check-ignore -v 2D-Portfolio/MyProgram.suo` |
| R4 | 소스·`.vcxproj`·`Resource/` 는 바뀌지 않는다 | `git diff --stat` 에 해당 경로 없음 |
| R5 | `Exe/`·`Temp/` 를 지운 깨끗한 상태에서 Debug·Release 빌드 성공, 경고 수 변화 없음 | MSBuild 종료 코드 0, 경고 수 비교 |
| R6 | 실행 동작 동일 — 로고→메뉴→스테이지, **배경음 재생**, 오류 메시지박스 없음 | 수동 실행 |

**삭제 목록**: `Release/Client.exe`, `Release/MFC_000.exe`, `MyProgram.suo`, `.vs/MyProgram/v14/.suo`,
`Client/Client.vcxproj.user`, `MFC_000/MFC_000.vcxproj.user`, `Client/Debug/Debug.dll`, `Client/fmodex.dll`, `Client/fmodexL64.dll`,
`Client/Monster.dat`, `Client/Monster1.dat`(아래 근거, 최종 결정은 개발자).

## 설계

- **`git rm` 으로 추적 해제 + 삭제, 이력 재작성은 하지 않는다** — 용량 이득보다 위험(강제 푸시·해시 변경)이 크다(KISS).
  지운 파일은 이력에서 복원 가능.
- **남길 바이너리 판정 기준 = 현재 빌드·실행이 실제로 쓰는가**:
  - `fmodexL.dll`+`fmodexL_vc.lib`: Win32 링크·실행(`GameSound.hpp:8`).
  - `fmodex64.dll`+`fmodex64_vc.lib`: x64 링크·실행(`GameSound.hpp:6`, [x64-migration](../x64-migration/spec.md)).
  - `fmodex.dll`: 짝 import lib 가 repo 에 없어 링크 불가 → 삭제. `fmodexL64.dll`: 짝 lib 없음 → 삭제.
  - `Debug.*`: 아직 링크됨 → 이 인텐트에선 유지, [debug-dll-removal](../debug-dll-removal/spec.md) 에서 제거.
- **`Monster1.dat` 분석(2026-11-10)** — 삭제 권장 근거:
  - 116바이트 레코드 4개. 현재 `OBJINFO`(324바이트, 첫 커밋부터 동일)와 형식이 달라 **이력상 어떤 코드로도 읽을 수 없다**.
    툴의 「몬스터 불러오기」는 324바이트 단위로 읽어 쓰레기를 만든다.
  - 내용: `vPos` 4개 (130,68)·(65,102)·(195,102)·(130,136) — 맵 원점 근처 마름모 시험 배치.
    `vDir`·`vLook`·`vCenter` 는 디버그 힙 미초기화 값(`0xCD`), 몬스터 종류 필드 없음.
  - Client 는 몬스터 파일을 읽지 않는다(몬스터는 `CStage` 코드가 생성).
  - `Monster.dat` 는 0바이트.
- **`.gitignore` 의 확장자 규칙(`*.obj`·`*.pdb` 등)은 남긴다** — 해가 없고 다른 경로 빌드에도 유효.

## flag된 우려사항

| 무엇이 | 어느 기준에 | 해소 |
|---|---|---|
| FMOD Ex 는 단종 — 지운 FMOD 파일은 공식적으로 다시 받을 수 없음 | 복구 가능성 | 이력 복원 `git show <커밋>^:<경로>`. 삭제 커밋 해시를 PR 에 기록 |
| `Monster1.dat` 삭제 | 데이터 손실 | 위 분석상 의미 있는 데이터 아님. **개발자 최종 확인** |
| `Client/Debug/Debug.dll` 을 `debug-dll-removal` 도 지움 | 작업 충돌 | 먼저 머지되는 쪽이 지우고 나중 쪽은 그 줄을 뺀다 |
| VS 가 열려 있으면 `.suo` 가 잠김 | 작업 실패 | VS 를 닫고 수행 |
| 이력 용량은 줄지 않음 | 기대 효과 | 수용. 작업 트리·체크아웃 크기만 감소 |
