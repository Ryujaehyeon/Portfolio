# Plan: repo-cleanup

Author: Ryu Jahyeon
Status: draft

<!-- 구현은 개발자(본인)가 직접 한다. 아래 명령은 repo 루트(Portfolio/) 기준, Git Bash 또는 WSL. -->

## 영향받는 파일

경로는 `2D-Portfolio/` 기준. 판단 근거는 2026-11-10 조사.

| 파일 | 크기 | 처리 | 근거 |
|---|---|---|---|
| `Release/Client.exe`, `Release/MFC_000.exe` | 192K·154K | 삭제 | 2016-08-22 빌드 산출물. 이제 `Exe/` 로 나온다. 폴더째 삭제 |
| `MyProgram.suo` | — | 삭제 + ignore 추가 | VS 사용자 상태. **현재 `.gitignore` 에 안 걸린다** |
| `.vs/MyProgram/v14/.suo` | — | 삭제 | VS2015 사용자 상태. `2D-Portfolio/.vs/` 로 이미 ignore |
| `Client/Client.vcxproj.user`, `MFC_000/MFC_000.vcxproj.user` | 작음 | 삭제 | 사용자별 설정(내용은 `ShowAllFiles` 뿐). `*.user` 로 이미 ignore |
| `Client/Debug/Debug.dll` | — | 삭제 | `Client/Debug.dll` 과 sha256 동일한 중복. Post-Build 는 `Client/Debug.dll` 을 복사 |
| `Client/fmodex.dll` | — | 삭제 | 짝인 import lib(`fmodex_vc.lib`)가 repo 에 없어 링크 불가 — 쓸 수 없는 파일 |
| `Client/fmodex64.dll`, `Client/fmodexL64.dll`, `Client/fmod/lib/fmodex64_vc.lib` | — | 삭제(권장) | x64 전용. 플랫폼은 Win32 뿐. intent 「Open questions」 2 |
| `Client/Monster.dat`, `Client/Monster1.dat` | 0B·464B | **결정 필요** | Client 미참조. intent 「Open questions」 1 |

**남기는 것**: `Client/Debug.dll`·`Client/Debug.lib`(링크됨, `debug-dll-removal` 에서 처리),
`Client/fmodexL.dll`·`Client/fmod/lib/fmodexL_vc.lib`(Win32 링크·실행, `GameSound.hpp:8`), `Client/Map.dat`(게임이 읽음).

`.gitignore`(repo 루트):
- 추가: `*.suo`
- 삭제(VS2010 시절 경로 — 이제 산출물은 `Exe/`·`Temp/`): 10행 `2D-Portfolio/Debug/Client.exe`, 18행 `2D-Portfolio/Debug/MFC_000.exe`,
  19행 `2D-Portfolio/Client/Debug/Client_manifest.rc`, 20행 `2D-Portfolio/MFC_000/Debug/MFC_000_manifest.rc`.
  `*.obj`·`*.pdb` 같은 확장자 규칙은 남긴다(해가 없고, 다른 경로 빌드에도 유효).

## 작업 순서

0. 브랜치: `git switch master && git pull && git switch -c feature/repo-cleanup`
1. **삭제** — `git rm` 은 추적 해제와 디스크 삭제를 같이 한다.
   ```
   cd 2D-Portfolio
   git rm -r Release
   git rm MyProgram.suo .vs/MyProgram/v14/.suo
   git rm Client/Client.vcxproj.user MFC_000/MFC_000.vcxproj.user
   git rm Client/Debug/Debug.dll
   git rm Client/fmodex.dll Client/fmodex64.dll Client/fmodexL64.dll Client/fmod/lib/fmodex64_vc.lib
   # Monster*.dat: 결정에 따라 — 삭제면 git rm, 이동이면 git mv
   ```
   - `.vcxproj.user` 는 VS 를 열면 다시 생긴다(이제 ignore 되므로 문제없음). 디스크에 남기고 싶으면 `git rm --cached`.
   - VS 를 켠 상태면 `.suo` 가 잠겨 실패할 수 있다 → VS 를 닫고 한다.
   → 확인: `git status` 에 위 경로가 `deleted:` 로만 보이고 다른 변경이 없다.
2. **`.gitignore` 수정** — 위 「영향받는 파일」대로.
   → 확인: `git check-ignore -v 2D-Portfolio/MyProgram.suo` 가 `*.suo` 규칙을 출력.
3. **검증** — 아래 「테스트」.
4. **CLAUDE.md** — 「건드리면 안 되는 부분」의 `커밋돼 있는 *.dll·*.lib` 줄을 남은 파일 목록에 맞게 고친다.
5. **커밋** — 이 인텐트 하나 = 커밋 하나(또는 문서 커밋 + 정리 커밋).
   제목 예: `chore(repo-cleanup): 커밋된 빌드 산출물과 미사용 바이너리 제거`
   → PR(`feature/repo-cleanup` → `master`), 본문에 검증 결과.

## 테스트

소스는 그대로이므로 **같은 결과가 나오는지**만 본다.

```
# 1) 깨끗한 상태에서 다시 빌드 (지운 파일에 숨은 의존이 없는지)
rm -rf 2D-Portfolio/Exe 2D-Portfolio/Temp
MSBuild.exe 2D-Portfolio/MyProgram.sln -t:restore -p:RestorePackagesConfig=true -nologo -v:m
MSBuild.exe 2D-Portfolio/MyProgram.sln -p:Configuration=Debug   -p:Platform=Win32 -m -nologo -v:m
MSBuild.exe 2D-Portfolio/MyProgram.sln -p:Configuration=Release -p:Platform=Win32 -m -nologo -v:m
```
기대: 두 구성 모두 종료 코드 0, 경고 수 변화 없음(정리 전 빌드 로그와 비교), `런타임 DLL 복사 (Debug.dll, fmodexL.dll)` 출력.

```
# 2) 실행 — 작업 디렉터리를 Client/ 로 (리소스 상대 경로)
cd 2D-Portfolio/Client && ../Exe/Win32/Debug/Client/Client.exe
```
기대: 로고 → 메뉴 → 스테이지 진입, **배경음 재생**(fmodexL.dll 정상), 오류 메시지박스("system err") 없음.
Release 도 같은 방법으로. MFC_000 은 `cd 2D-Portfolio/MFC_000` 에서 실행해 창이 뜨는지.

```
# 3) 남은 바이너리 목록이 의도와 같은지
git ls-files | grep -i -E '\.(exe|dll|lib|pdb|suo|user)$'
```
기대: `Client/Debug.dll`, `Client/Debug.lib`, `Client/fmodexL.dll`, `Client/fmod/lib/fmodexL_vc.lib` 4개뿐.

## 리스크

- **x64 FMOD 파일 삭제**: x64 를 나중에 추가할 때 필요. FMOD Ex 는 단종이라 공식 재배포처가 없다 →
  `git show <이 커밋>^:2D-Portfolio/Client/fmodex64.dll > fmodex64.dll` 로 이력에서 복원. 삭제 커밋 해시를 PR 에 적어 둔다.
- **`Monster1.dat` 가 의미 있는 툴 데이터일 가능성**: 지우기 전에 MFC_000 에서 열어(`Page2` 몬스터 불러오기) 내용 확인 권장.
- **이력 용량은 줄지 않는다**: 이력 재작성은 범위 밖. 작업 트리·클론 이후 체크아웃 크기만 준다.
- 실패 시 되돌리기: 커밋 전 `git restore --staged --worktree <경로>`, 커밋 후 `git revert`.
