# 수정 가이드 — 내가 직접 고칠 때

레거시 정리를 **개발자가 직접** 진행하기 위한 안내. 무엇을 고칠지는 각 인텐트 문서에 있고, 이 문서는 **어떤 순서로·어떻게 작업하고·무엇으로 확인하는지**를 모은다.

- 왜/무엇: `intent/<슬러그>/intent.md` → `spec.md`(요구사항 R1… = 완료 기준)
- 어디를/어떻게: `intent/<슬러그>/plan.md`(단계별 명령)
- 코드 지도: [ARCHITECTURE.md](ARCHITECTURE.md) · 문제 목록: [legacy-audit/review.md](../intent/legacy-audit/review.md) · 규칙: [CLAUDE.md](../CLAUDE.md)

## 1. 지금 할 일 (순서 고정)

진행 중인 인텐트 없음(2026-11-12 `const-tchar` 완료). 착수하면 여기에 표(인텐트·브랜치·상태·선행)로 적는다.

다음은 오딧 「제안 순서」: `warning-cleanup` → `shared-render-lib` → `input-unify` → `resource-path` → DirectX 전환.
이것들은 아직 intent/spec/plan 이 없다 — 착수할 때 3번 절대로 만든다. 완료한 인텐트는 이 표에서 지운다(기록은 `intent/<슬러그>/` 와 PR).

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
VS 에서는 그냥 빌드(F7)해도 같다. x64 는 `-p:Platform=x64` — 구성 4개(Win32·x64 × Debug·Release)를 모두 본다.

### 실행 확인

리소스가 상대 경로라 **작업 디렉터리가 `2D-Portfolio/Client/`** 여야 한다(툴은 `MFC_000/`).
- VS: F5 (작업 디렉터리 기본값이 프로젝트 폴더라 그대로 됨)
- 직접: `cd 2D-Portfolio/Client && ../Exe/Win32/Debug/Client/Client.exe`
- 자동: `Client.exe -smoke`·`MFC_000.exe -smoke ../Client/Map.dat` — 명령·판정은 CLAUDE.md 「테스트」. 장면·배경음 로드·타일·몬스터·스킬 생성까지 본다(화면·소리·조작은 아래 체크리스트로).

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

(진행 중인 인텐트가 없어 비어 있다.)

## 5. 함정 모음

| 상황 | 대처 |
|---|---|
| 새 `.cpp/.h` 를 만들었다 | **UTF-8 BOM** 이어야 한다. VS 는 `.editorconfig` 로 자동 처리. 다른 편집기면 BOM 확인 — 없으면 한글이 CP949 로 읽혀 깨진다 |
| 문자열 비교를 새로 쓴다 | `==` 금지(주소 비교). `lstrcmp(a, b) == 0` 또는 `wstring` 비교 — 오딧 2번 |
| `ERR_MSG` 에 포맷 인자를 넘긴다 | `ERR_MSG(g_hWnd, L"%s 로드 실패", pKey)` 처럼 쓴다. 지정자와 인자 타입 불일치는 컴파일러가 못 잡는다 — `TCHAR*` 는 `%s` |
| `TILE`·`OBJINFO` 를 고치고 싶다 | `.dat` 호환이 깨진다. 포인터·`size_t` 를 넣으면 x86/x64 간에도 깨진다 |
| `Client/fmod.log` 가 생긴다 | FMOD 로깅판(`fmodexL`)이 실행마다 남기는 로그. `*.log` 로 gitignore — 무시 |
| exe 를 더블클릭했더니 리소스 오류 | 정상(작업 디렉터리 문제). VS F5 나 `cd Client` 후 실행 |
| `git status` 에 vcxproj 가 통째로 바뀐 것처럼 보임 | VS 가 줄바꿈을 CRLF 로 저장 — `.gitattributes`(`text=auto`)가 커밋 시 정규화하므로 실제 diff 는 작다. `git diff --stat` 로 확인 |
| 경고 수가 늘었다 | 새로 생긴 경고는 고치고 커밋. 기존 경고(CLAUDE.md 「빌드」의 기준 수)는 `warning-cleanup` 몫 |
| 빌드 산출물이 `git status` 에 보인다 | 산출물은 `Exe/`·`Temp/` 로만 나와야 한다. 다른 곳에 생기면 vcxproj 에 OutDir 이 박혔는지 확인(경로는 `Directory.Build.props` 한 곳에서만) |
| 새 코드를 어떤 스타일로 쓸지 | 이름은 주변 기존 관례(`C` 접두사·`m_` 헝가리안), 품질은 [C++ 표준](../.harness/standards/cpp11-coding-standard.md) — 경고 0·`const`·`nullptr`·`override`·한 줄 블록도 중괄호. 범위는 CLAUDE.md 「C++ 코딩 표준」 |

## 6. Claude 에게 맡길 수 있는 것

- **리뷰**: PR 을 올리고 「PR #N 리뷰해줘」 — spec 요구사항·CLAUDE.md 관례 기준으로 본다.
- **문서**: 다음 인텐트의 intent·spec·plan 초안, ARCHITECTURE.md 갱신.
- **조사**: 「이 함수 어디서 쓰여?」, 「이 바이너리 뭐야?」 같은 근거 수집.
- **검증 보조**: 결정적 빌드 해시 비교, 의존 DLL 확인, 경고 수 집계.
