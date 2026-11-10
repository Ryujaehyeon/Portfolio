# 수정 가이드 — 내가 직접 고칠 때

레거시 정리를 **개발자가 직접** 진행하기 위한 안내. 무엇을 고칠지는 각 인텐트 문서에 있고, 이 문서는 **어떤 순서로·어떻게 작업하고·무엇으로 확인하는지**를 모은다.

- 왜/무엇: `intent/<슬러그>/intent.md` → `spec.md`(요구사항 R1… = 완료 기준)
- 어디를/어떻게: `intent/<슬러그>/plan.md`(단계별 명령)
- 코드 지도: [ARCHITECTURE.md](ARCHITECTURE.md) · 문제 목록: [legacy-audit/review.md](../intent/legacy-audit/review.md) · 규칙: [CLAUDE.md](../CLAUDE.md)

## 1. 지금 할 일 (순서 고정)

| # | 인텐트 | 브랜치 | 상태 | 선행 |
|---|---|---|---|---|
| 1 | [repo-cleanup](../intent/repo-cleanup/plan.md) | `feature/repo-cleanup` (만들어 둠) | 착수 대기 | — |
| 2 | [debug-dll-removal](../intent/debug-dll-removal/plan.md) | `feature/debug-dll-removal` (master 에서 새로) | 대기 | 1 머지 권장 |
| 3 | [x64-migration](../intent/x64-migration/plan.md) | `feature/x64-migration` (**WIP 커밋 `6c6b963` 있음**) | 진행 중 보관 | **2 머지 필수** |

그다음은 오딧 「제안 순서」: `dead-code-removal` → `err-msg-format` → `ub-fixes` → `objmgr-lifetime` →
`string-key-compare`+`const-tchar` → `warning-cleanup` → `shared-render-lib` → `input-unify` → `resource-path` → DirectX 전환.
이것들은 아직 intent/spec/plan 이 없다 — 착수할 때 3번 절대로 만든다.

## 2. 인텐트 하나를 처리하는 공통 사이클

```
① 읽기 ─ intent.md → spec.md(요구사항 표) → plan.md
② 브랜치 ─ git switch master && git pull && git switch -c feature/<슬러그>
③ 변경 전 기록 ─ plan 의 「변경 전」 단계가 있으면 먼저(예: 현재 동작·경고 수)
④ 수정 ─ plan 「작업 순서」대로, 단계마다 → 확인
⑤ 검증 ─ 빌드 + 실행 + spec 요구사항 R1… 를 하나씩 체크
⑥ 문서 ─ 규칙이 바뀌었으면 CLAUDE.md, 구조가 바뀌었으면 ARCHITECTURE.md
⑦ 커밋 ─ type(<슬러그>): 제목   (본문 끝에 무엇을/왜)
⑧ PR ─ feature/<슬러그> → master, 본문에 spec R 항목별 결과 → 머지 = 완료
```

### 빌드 (WSL 또는 Git Bash, repo 루트에서)

```bash
M="/mnt/c/Program Files/Microsoft Visual Studio/18/Insiders/MSBuild/Current/Bin/MSBuild.exe"   # WSL 경로
rm -rf 2D-Portfolio/Exe 2D-Portfolio/Temp                         # 깨끗한 빌드가 필요할 때
"$M" 2D-Portfolio/MyProgram.sln -t:restore -p:RestorePackagesConfig=true -nologo -v:m
"$M" 2D-Portfolio/MyProgram.sln -p:Configuration=Debug   -p:Platform=Win32 -m -nologo -v:m > build-debug.log
"$M" 2D-Portfolio/MyProgram.sln -p:Configuration=Release -p:Platform=Win32 -m -nologo -v:m > build-release.log
grep -c "warning C" build-debug.log                               # 경고 수 — 변경 전후 비교
```
VS 에서는 그냥 빌드(F7)해도 같다. x64 가 생긴 뒤에는 `-p:Platform=x64` 도.

### 실행 확인

리소스가 상대 경로라 **작업 디렉터리가 `2D-Portfolio/Client/`** 여야 한다(툴은 `MFC_000/`).
- VS: F5 (작업 디렉터리 기본값이 프로젝트 폴더라 그대로 됨)
- 직접: `cd 2D-Portfolio/Client && ../Exe/Win32/Debug/Client/Client.exe`

매번 볼 것(회귀 체크리스트):
- [ ] 로고 → 메뉴 → 「시작」 → 스테이지
- [ ] 메뉴·스테이지 배경음
- [ ] 스테이지 맵 타일 표시(`Map.dat`), 플레이어 이동(마우스), 몬스터 등장
- [ ] 스킬 1개 이상 사용, HP/MP 바
- [ ] 「system err」 메시지박스 없음 · Debug 는 콘솔 창 뜸 · Release 는 콘솔 없음
- [ ] 메뉴 「종료」로 정상 종료

## 3. 새 인텐트 시작하기 (오딧 백로그 항목)

1. `intent/<슬러그>/` 를 만들고 `.harness/templates/{intent,spec,plan}.md` 를 복사해 채운다.
   근거는 `legacy-audit/review.md` 의 해당 번호에서 가져온다.
2. 또는 Claude 에게 「`<슬러그>` intent·spec·plan 작성해줘」 — 지금까지와 같은 형식으로 조사해 초안을 만든다.
3. `intent/<슬러그>` 브랜치로 문서 PR → 머지(= 계획 승인) → 2번 절 사이클로 구현.

## 4. 인텐트별 요령

### 4.1 repo-cleanup
- **VS 를 닫고** 시작(`.suo` 잠김).
- `Monster.dat`·`Monster1.dat` 는 **삭제 권장** — `Monster1.dat` 는 이력상 어떤 코드로도 못 읽는 구버전 형식의 시험 배치 4개(spec 「설계」 근거).
- 남길 바이너리 6개를 지우지 않도록 plan 의 `git rm` 줄을 그대로 쓴다. 특히 `fmodex64.dll`·`fmodex64_vc.lib` 는 x64 용으로 **남긴다**.
- 확인: `git ls-files | grep -i -E '\.(exe|dll|lib|pdb|suo|user)$'` → 6줄.
- 커밋 예: `chore(repo-cleanup): 커밋된 빌드 산출물과 미사용 바이너리 제거` — PR 에 삭제 커밋 해시 기록(FMOD 복원용).

### 4.2 debug-dll-removal
- **변경 전에** Debug 로 메뉴까지 실행해 `Scene :` 로그가 어디에도 안 나오는지 본다(plan 1단계). 결과를 PR 에 적는다.
- `MenuD2.cpp:82-84` 는 `if(iScene > SCENEID_NONPASS) DebugLog(L"Scene : %d", iScene);` 로. `wsprintf` 두 줄을 꼭 지운다.
- `Client/Debug/Debug.dll` 은 repo-cleanup 에서 이미 지웠으면 이 단계에서 뺀다.
- 확인: VS 개발자 명령 프롬프트에서 `dumpbin /dependents 2D-Portfolio\Exe\Win32\Release\Client\Client.exe` 에 `Debug.dll` 없음.
- 커밋 예: `refactor(debug-dll-removal): 소스 없는 Debug.dll 의존 제거, 디버그 로그를 Debug.hpp 로 통일`

### 4.3 x64-migration (WIP 이어 하기)
```bash
git switch feature/x64-migration
git rebase master          # debug-dll-removal 머지 후
```
- 충돌은 `Client.vcxproj` 의 Post-Build 부근에서 날 가능성이 높다 — **master 쪽(Debug.dll 복사 줄 없음)** 을 택하고 x64 블록은 유지.
- 남은 일은 plan 2단계(`$(FmodDll)`)와 CLAUDE.md. x64 구성 자체는 WIP 에 이미 있다.
- WIP 커밋은 그대로 둬도 되고, 하나로 합치려면 `git reset --soft master` 후 다시 커밋한다.
- 확인: 4개 구성 빌드, `Exe/x64/Debug/Client/` 에 `fmodex64.dll` 만 있고 `fmodexL.dll` 없음, x64 로 2번 절 체크리스트 + 툴에서 `Map.dat` 열기.

## 5. 함정 모음

| 상황 | 대처 |
|---|---|
| 새 `.cpp/.h` 를 만들었다 | **UTF-8 BOM** 이어야 한다. VS 는 `.editorconfig` 로 자동 처리. 다른 편집기면 BOM 확인 — 없으면 한글이 CP949 로 읽혀 깨진다 |
| 문자열 비교를 새로 쓴다 | `==` 금지(주소 비교). `lstrcmp(a, b) == 0` 또는 `wstring` 비교 — 오딧 2번 |
| `ERR_MSG` 에 포맷 인자를 넘긴다 | 지금 매크로는 3번째 인자를 버린다. `err-msg-format` 전까지는 `swprintf` 로 먼저 조립 |
| `TILE`·`OBJINFO` 를 고치고 싶다 | `.dat` 호환이 깨진다. 포인터·`size_t` 를 넣으면 x86/x64 간에도 깨진다 |
| exe 를 더블클릭했더니 리소스 오류 | 정상(작업 디렉터리 문제). VS F5 나 `cd Client` 후 실행 |
| `git status` 에 vcxproj 가 통째로 바뀐 것처럼 보임 | VS 가 줄바꿈을 CRLF 로 저장 — `.gitattributes`(`text=auto`)가 커밋 시 정규화하므로 실제 diff 는 작다. `git diff --stat` 로 확인 |
| 경고 수가 늘었다 | 새로 생긴 경고는 고치고 커밋. 기존 283건(Debug)은 `warning-cleanup` 몫 |
| 빌드 산출물이 `git status` 에 보인다 | 산출물은 `Exe/`·`Temp/` 로만 나와야 한다. 다른 곳에 생기면 vcxproj 에 OutDir 이 박혔는지 확인(경로는 `Directory.Build.props` 한 곳에서만) |
| 새 코드를 어떤 스타일로 쓸지 | 이름은 주변 기존 관례(`C` 접두사·`m_` 헝가리안), 품질은 [C++ 표준](../.harness/standards/cpp11-coding-standard.md) — 경고 0·`const`·`nullptr`·`override`·한 줄 블록도 중괄호. 범위는 CLAUDE.md 「C++ 코딩 표준」 |

## 6. Claude 에게 맡길 수 있는 것

- **리뷰**: PR 을 올리고 「PR #N 리뷰해줘」 — spec 요구사항·CLAUDE.md 관례 기준으로 본다.
- **문서**: 다음 인텐트의 intent·spec·plan 초안, ARCHITECTURE.md 갱신.
- **조사**: 「이 함수 어디서 쓰여?」, 「이 바이너리 뭐야?」 같은 근거 수집.
- **검증 보조**: 결정적 빌드 해시 비교, 의존 DLL 확인, 경고 수 집계.
