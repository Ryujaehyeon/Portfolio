# Plan: x64-migration

Author: Ryu Jahyeon
Status: draft

<!-- 구현은 개발자(본인)가 직접 한다. 선행: debug-dll-removal 머지 완료. 경로는 2D-Portfolio/ 기준. -->

## 영향받는 파일

| 파일 | 할 일 |
|---|---|
| `MyProgram.sln` | 솔루션 플랫폼 `Debug|x64`·`Release|x64` 추가, 두 프로젝트 매핑 |
| `Client/Client.vcxproj` | x64 `ProjectConfiguration` 2개 + 구성별 PropertyGroup/ItemDefinitionGroup 복제, Post-Build 의 FMOD DLL 을 플랫폼별로 |
| `MFC_000/MFC_000.vcxproj` | x64 구성 추가(Post-Build 없음) |
| `Directory.Build.props` | 수정 없음 — 경로가 이미 `$(Platform)` 을 포함 |
| `Client/GameSound.hpp` | 수정 없음 — `_WIN64` 분기가 이미 있다(`:5-8`) |
| `MFC_000/stdafx.h` | 수정 없음 — `amd64` 매니페스트 분기가 이미 있다(`:52`) |
| `CLAUDE.md` | 빌드 섹션에 x64 명령·산출물 경로, 플랫폼별 DLL 설명 |

## 작업 순서

0. 선행 확인: `git grep -n "Debug.lib\|DebugMsg"` 결과가 없다(= `debug-dll-removal` 완료).
   브랜치: `git switch master && git pull && git switch -c feature/x64-migration`
1. **x64 구성 추가 — VS 구성 관리자 사용(권장)**
   - VS2026 에서 솔루션 열기 → 빌드 > 구성 관리자 → 「활성 솔루션 플랫폼」 → `<새로 만들기...>`
   - 새 플랫폼 `x64`, 「다음에서 설정 복사」 `Win32`, 「새 프로젝트 플랫폼 만들기」 체크 → 확인.
   - VS 가 sln 과 두 vcxproj 에 x64 구성을 Win32 설정 그대로 복제한다.
   → 확인: `git diff --stat` 에 sln·두 vcxproj 만 바뀜. vcxproj 에 `Debug|x64`·`Release|x64` 조건 블록이 Win32 와 같은 수만큼 생김
     (`grep -c "|x64'" Client/Client.vcxproj` ≈ `grep -c "|Win32'"`).
   - 복제 후 x64 블록의 `AdditionalDependencies` 에 `d3d9.lib;dxguid.lib;dinput8.lib` 가 있는지 눈으로 확인.
   - 손으로 하려면: 각 `Condition="...|Win32'"` 블록·줄을 복사해 `x64` 로 바꾼다. 주의 — 여는 태그만 있는 줄을
     따로 복제하면 XML 이 깨진다(MSB4025). 블록 단위로 복사할 것.
2. **Post-Build 의 FMOD DLL 을 플랫폼별로** (`Client.vcxproj`)
   현재 Post-Build 는 32비트 `fmodexL.dll` 을 무조건 복사 → x64 실행 시 로드 실패.
   ```xml
   <!-- Globals 아래 아무 PropertyGroup 에 -->
   <PropertyGroup>
     <FmodDll Condition="'$(Platform)'=='Win32'">fmodexL.dll</FmodDll>
     <FmodDll Condition="'$(Platform)'=='x64'">fmodex64.dll</FmodDll>
   </PropertyGroup>
   <!-- 기존 PostBuildEvent Command 를 -->
   <Command>copy /Y "$(ProjectDir)$(FmodDll)" "$(OutDir)" &gt; nul</Command>
   <Message>런타임 DLL 복사 ($(FmodDll))</Message>
   ```
   (`debug-dll-removal` 이 끝났으면 `Debug.dll` 복사 줄은 이미 없다.)
   → 확인: x64 빌드 출력 `Exe\x64\Debug\Client\` 에 `fmodex64.dll` 만, Win32 출력엔 `fmodexL.dll` 만.
3. **기본 플랫폼** — intent 「Open questions」 2 결정에 따라 구성 관리자에서 x64 를 활성으로 두고 저장
   (활성 플랫폼은 `.vs/` 사용자 상태에 저장되므로 커밋 대상이 아님 — 참고만).
4. **CLAUDE.md** 빌드 섹션 갱신: `-p:Platform=x64` 명령, `Exe\x64\...` 경로, 플랫폼별 FMOD DLL.
5. **커밋**: `chore(x64-migration): x64 플랫폼 구성 추가` → PR, 본문에 아래 검증 결과.

## 테스트

```
rm -rf 2D-Portfolio/Exe 2D-Portfolio/Temp
MSBuild.exe 2D-Portfolio/MyProgram.sln -t:restore -p:RestorePackagesConfig=true -nologo -v:m
# 4개 구성 모두
MSBuild.exe 2D-Portfolio/MyProgram.sln -p:Configuration=Debug   -p:Platform=x64   -m -nologo -v:m
MSBuild.exe 2D-Portfolio/MyProgram.sln -p:Configuration=Release -p:Platform=x64   -m -nologo -v:m
MSBuild.exe 2D-Portfolio/MyProgram.sln -p:Configuration=Debug   -p:Platform=Win32 -m -nologo -v:m
MSBuild.exe 2D-Portfolio/MyProgram.sln -p:Configuration=Release -p:Platform=Win32 -m -nologo -v:m
```
기대: 4개 모두 종료 코드 0. x64 는 Win32 대비 경고가 C4267 만큼(약 13곳) 늘어나는 것이 정상.
`file Exe/x64/*/Client/Client.exe` → `PE32+`.

실행(작업 디렉터리 `2D-Portfolio/Client/`, 툴은 `MFC_000/`):
- x64 Client: 로고 → 메뉴 → 스테이지. **배경음 재생**(fmodex64), **맵 타일 표시**(`Map.dat` 로드), 몬스터 등장·스킬 사용, 오류 메시지박스 없음.
- x64 MFC_000: 창이 뜨고 타일 불러오기로 `Client/Map.dat` 를 열 수 있음(32비트 툴이 저장한 데이터 호환 확인).
- Win32 도 같은 항목을 한 번 — 회귀 없음.

## 리스크

- **FMOD 로깅판 차이**: Win32 는 `fmodexL`(로깅), x64 는 `fmodex64`(비로깅). API 는 같아 동작 차이는 없고 로그만 안 남는다.
- **포인터를 정수에 담는 코드**: 시험 빌드에서 C4311/C4312/C4302(포인터 잘림)는 0건이었다. 그래도 실행 중 이상하면
  `SetWindowLong`·`(int)`/`(DWORD)` 캐스트부터 의심.
- **`.dat` 호환**: 현재 구조체엔 포인터가 없어 호환된다. 이후 `TILE`/`OBJINFO` 에 포인터·`size_t`·`long double` 을 넣으면
  플랫폼 간 파일 호환이 깨진다 → CLAUDE.md 「건드리면 안 되는 부분」에 이미 구조체 변경 경고가 있다.
- 구성 관리자 복제는 Win32 의 잘못된 설정도 그대로 복제한다(예: 이미 고친 Release 링크 누락 같은 것) → 1단계 확인 항목대로 눈으로 본다.

## 검증 진행 (2026-11-10, 보류 중)

| spec | 결과 |
|---|---|
| R1 4개 구성 빌드 | ✅ `Exe`·`Temp` 삭제 후 restore + 4개 구성(`-m:1 -nodeReuse:false`) 종료 코드 0·오류 0 |
| R2 PE32+ | ✅ x64 Client·MFC_000 Debug·Release |
| R3 플랫폼별 DLL | ✅ Win32 출력 `fmodexL.dll` 만, x64 출력 `fmodex64.dll` 만 |
| R4 x64 Client 실행 | ⏸ 8초 스모크만 통과(Debug·Release 창 응답, `system err` 없음). 스테이지·배경음·타일·몬스터·스킬은 `smoke-log` 완료 후 자동 검증 |
| R5 x64 MFC_000 `Map.dat` | ⏸ 툴 창 뜸(Debug·Release). 타일 불러오기는 `smoke-log` 후 |
| R6 Win32 회귀 | ⏸ 8초 스모크만 통과 |
| R7 새 경고 | ✅ x64 는 Win32(198) 대비 C4267 13곳뿐(Debug 276·Release 208 줄), C4311·C4312·C4302 0, C4018 은 x64 에서 사라짐 |
| R8 CLAUDE.md | ✅ |

보류 이유: 수동 확인 대신 자동 검증용 로그·실행 모드(`smoke-log` 인텐트)를 먼저 만들기로 사용자 결정(2026-11-10).
