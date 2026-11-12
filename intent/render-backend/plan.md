# Plan: render-backend

Author: Ryu Jahyeon
Status: draft — `render-capture` 완료 후 확정

<!-- 입력: spec.md. 경로는 2D-Portfolio/ 기준. -->

## 영향받는 파일

| 파일 | 할 일 | 요구 |
|---|---|---|
| `Engine/Renderer.h`(새) | `IRenderer`·`IRenderTexture` | R1·R2 |
| `Engine/RendererMgr.h/.cpp`(새) | `CRenderer` 싱글턴 | R4 |
| `Engine/Device.h/.cpp` → `Engine/RendererD3D9.h/.cpp`(`git mv`) | `CRendererD3D9`, 텍스처 적재 이동, 캡처 저장 | R5 |
| `Headers/Struct.h` | `TEXINFO` — `IRenderTexture*` + 크기 구조체 | R2·R3 |
| `Engine/SingleTexture.cpp`·`MultiTexture.cpp`·`TextureMgr.*` | `CRenderer::CreateTexture` 사용, 해제 | R6 |
| `Client/MainGame.cpp` | 백엔드 생성·`Initialize`, `ZENABLE` 직접 호출 삭제, `BeginFrame`/`EndFrame` | R5·R8 |
| Client 그리기 호출부 11개 파일 | `DrawSprite` 로 변환 | R7 |
| `MFC_000/MFC_000View.cpp`·`MiniView.cpp` | 백엔드 생성, `DrawSprite` 로 변환 | R7·R8 |
| `Engine/Engine.vcxproj(.filters)` | 파일 추가·이름 변경 | — |
| `CLAUDE.md`·`docs/ARCHITECTURE.md` | 구조 설명 | R11 |

## 작업 순서

0. **기준 캡처.** 변경 전 빌드로 `-smoke -shot shots/before` 를 Win32 Debug·x64 Release 에서 찍어 둔다(R9 의 기준).
1. **Engine.** 인터페이스·싱글턴·D3D9 구현·`TEXINFO`·텍스처 클래스. 아직 호출부가 옛 API 를 쓰므로,
   이 단계에서는 `CRendererD3D9` 에 임시로 `GetSprite()` 를 남겨 두고 빌드를 통과시킨다.
   `TEXINFO` 가 파일에 쓰이지 않는지 확인한다(`git grep -n "sizeof(TEXINFO)\|WriteFile\|ReadFile"`).
   → 빌드·스모크·캡처 비교.
   → 커밋 `refactor(render-backend): 렌더러 인터페이스와 D3D9 구현을 Engine 에 둔다`
2. **Client 호출부.** 파일 단위로 `DrawSprite` 로 바꾼다. `UIObj.cpp`(25곳)는 따로 한 번 더 확인한다.
   → 캡처 비교 허용 오차 0.
   → 커밋 `refactor(render-backend): Client 그리기 호출을 DrawSprite 로 바꾼다`
3. **툴 호출부.** → 툴 스모크·일반 실행 화면 확인.
   → 커밋 `refactor(render-backend): 툴 그리기 호출을 DrawSprite 로 바꾼다`
4. **임시 접근자 삭제.** `GetSprite()`·`GetDevice()` 를 없애고, Engine 밖에 남은 사용처가 없음을 확인한다(R7).
   → 4개 구성 빌드·스모크·캡처 비교(R9·R10).
   → 문서(R11) → PR → 머지.

## 테스트

```
# 기준(변경 전)과 변경 후를 같은 시점에서 비교
../Exe/<플랫폼>/<구성>/Client/Client.exe -smoke -shot shots/after
powershell.exe -NoProfile -File ../../scripts/compare-shots.ps1 -Expected shots/before -Actual shots/after   → 0
git grep -n -E "GetSprite\(\)|GetDevice\(\)" -- 2D-Portfolio/Client 2D-Portfolio/MFC_000   → 결과 없음
```
