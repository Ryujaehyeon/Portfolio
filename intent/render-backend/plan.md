# Plan: render-backend

Author: Ryu Jahyeon
Status: approved (PR #60)

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

## 구현 중 변경

- **`EndFrame` 을 둘로 나눴다.** 인터페이스는 `EndFrame()`(그리기 끝)과 `Present(창)` 을 따로 가진다.
  캡처는 「그리기 끝 → 백버퍼 저장 → Present」 순서여야 하는데, 이 순서를 백엔드마다 반복하지 않고 `CRenderer::EndFrame(창)` 한 곳에서 맞추기 위해서다.
- **깊이 테스트 인자(`bDepthTest`)를 만들지 않았다.** D3D9 는 자동 깊이 버퍼(`EnableAutoDepthStencil`)를 만들면 `D3DRS_ZENABLE` 기본값이 이미 TRUE 다.
  Client 의 직접 설정은 중복이었으므로 지웠고, 캡처가 픽셀 단위로 같아 영향이 없음을 확인했다.
- **임시 `GetSprite()` 단계(작업 순서 1~3)를 거치지 않았다.** 호출부 50곳을 스크립트로 한 번에 바꿨다 —
  `Draw` 마다 같은 함수 안에서 바로 앞의 `SetTransform` 행렬을 넘긴다. `CUIObj::FontOutput` 은 함수 안에 `SetTransform` 이 없어
  호출한 `CUIObj::Render` 가 설정한 `m_Info.matWorld` 를 쓴다(그 사이에 `matWorld` 는 바뀌지 않는다). 그래서 코드 커밋이 하나다.
- 두 인터페이스(`IRenderer`·`IRenderTexture`)를 `Engine/Renderer.h` 한 파일에 두었고, D3D9 텍스처 감싸개는 `CRendererD3D9` 의 안쪽 클래스(`CTextureD3D9`)로 두었다.

## 검증 결과 (2026-11-16)

| spec | 결과 |
|---|---|
| R1~R6·R8 | ✅ `IRenderer`·`IRenderTexture`(`Renderer.h`), `CRenderer`(`RendererMgr.*`), `CRendererD3D9`(`Device.*` 를 `git mv`), `TEXINFO` = `IRenderTexture*` + `TEXSIZE ImgInfo`. 텍스처 클래스는 `CRenderer::CreateTexture`. Client·툴이 각자 `new CRendererD3D9` |
| R3 | ✅ 크기를 읽는 Client·툴 코드는 수정 없음 |
| R7 | ✅ Engine 밖에 `GetSprite()`·`GetDevice()`·`CDevice` 없음 |
| R9 | ✅ 변경 전후 캡처 허용 오차 0 비교 — Win32 Debug 6/6, x64 Release 6/6 모두 다른 픽셀 0 |
| R10 | ✅ 4개 구성 경고 0·오류 0, Client·툴 스모크 4개 구성 PASS, 일반 실행 Client·툴 살아 있음(창 제목 정상). ⏳ 툴 화면(타일·미니맵)은 자동 캡처가 안 돼(`PrintWindow` 는 D3D9 창 내용을 담지 못함) 사용자 확인 대기 |
| R11 | ✅ CLAUDE.md·ARCHITECTURE.md |
