# Plan: render-dxtk

Author: Ryu Jahyeon
Status: approved (PR #60)

<!-- 입력: spec.md. 경로는 2D-Portfolio/ 기준. -->

## 영향받는 파일

| 파일 | 할 일 | 요구 |
|---|---|---|
| `Engine/ImageLoader.h/.cpp`(새) | `LoadImagePixels` — WIC + 컬러키 | R3 |
| `Engine/RendererD3D11Tk.h/.cpp`(새) | B 백엔드 | R1·R2·R4~R8 |
| `Engine/packages.config`, `Engine/Engine.vcxproj(.filters)` | DirectXTK 패키지(헤더), 새 파일 | — |
| `Client/packages.config`, `Client/Client.vcxproj` | DirectXTK 패키지(링크), `d3d11.lib`·`dxgi.lib` | R9 |
| `Client/Client.cpp`·`Client/MainGame.cpp` | `-d3d11` 해석, 백엔드 선택 | R9 |
| `CLAUDE.md`·`docs/ARCHITECTURE.md`·`docs/RENDER-ROADMAP.md` | 문서 | R13 |

## 작업 순서

1. **패키지와 경고 확인.** DirectXTK NuGet(`directxtk_desktop_win10`, 최신 안정판)을 Engine·Client 에 넣는다.
   빈 `.cpp` 에 `SpriteBatch.h`·`CommonStates.h`·`ScreenGrab.h` 만 포함해 4개 구성을 빌드한다.
   → 경고가 나오면 **멈추고 사용자 결정**(spec 「경고 정책」).
2. **이미지 디코더.** `LoadImagePixels` 를 만들고, 기존 D3D9 경로로 적재한 텍스처와 크기가 같은지 몇 개 확인한다.
   반투명 픽셀이 있는 이미지가 있는지 확인한다(컬러키 규칙 검증).
   → 커밋 `feat(render-dxtk): WIC 이미지 디코더와 컬러키 적용`
3. **장치·스왑체인·지우기.** 화면이 지정 색으로 지워지는지 본다.
4. **텍스처·SpriteBatch·변환·색·캡처.** `-smoke -d3d11` PASS.
   → 커밋 `feat(render-dxtk): D3D11 + DirectXTK 백엔드`
5. **화면 비교.** 같은 시점에서 D3D9 와 D3D11 캡처를 찍어 비교한다. diff 이미지를 보고 허용 범위를 정해 이 문서의 「검증 결과」에 근거와 함께 적는다.
   위치가 어긋나 보이면(경계가 통째로 밀림) 변환 분해를 다시 본다.
6. **선택·툴 확인.** Client `-d3d11` 선택, 툴 링크 입력 확인(R10).
   → 4개 구성 빌드·스모크(두 백엔드)·툴 스모크.
   → 문서 → PR → 머지.

## 테스트

```
cd 2D-Portfolio/Client
../Exe/x64/Debug/Client/Client.exe -smoke -shot shots/d3d9
../Exe/x64/Debug/Client/Client.exe -smoke -shot shots/d3d11 -d3d11
powershell.exe -NoProfile -File ../../scripts/compare-shots.ps1 -Expected shots/d3d9 -Actual shots/d3d11 -Tolerance 8 -MaxRatio 0.01
```

- 디버그 레이어 메시지는 VS 출력 창이나 DebugView 로 본다. 스모크 중 D3D11 오류·경고 메시지가 0 인 것을 목표로 한다.

## 구현 중 변경

- **DirectXTK 헤더를 전방 선언하지 않는다.** 최신판(2026.5.8.1)은 클래스를 `DirectX::DX11` 인라인 네임스페이스에 둔다.
  `namespace DirectX { class SpriteBatch; }` 로 전방 선언하면 다른 클래스로 잡혀 「모호한 기호」 오류가 난다. 그래서 `RendererD3D11Tk.h` 가 `SpriteBatch.h`·`CommonStates.h` 를 직접 포함한다.
- **행렬식이 음수인 스프라이트**는 분해하지 않고, 그 스프라이트만 `SpriteBatch::Begin` 의 변환 행렬로 따로 그린다 — 어떤 아핀 행렬이든 정확하다.
  지금 Client 에는 음수 크기가 없다(음수가 될 수 있는 `nPos` 는 툴에만 있고 툴은 D3D9).
- **선택한 백엔드를 스모크 로그에 남긴다**(`INFO renderer D3D9` / `INFO renderer D3D11 (DirectXTK)`). 캡처가 같을 때 「정말 D3D11 로 그렸나」를 바로 확인하려고.
- 2~4번 작업을 한 커밋으로 묶었다. d3d11·dxgi·windowscodecs 는 Engine 소스의 `#pragma comment(lib)` 로 링크한다 — 그 오브젝트를 쓰는 Client 에만 들어간다.

## 검증 결과 (2026-11-16)

| spec | 결과 |
|---|---|
| 경고 정책 | ✅ DirectXTK 헤더만 포함해 Engine 4개 구성 경고 0 — 사용자 결정 불필요 |
| R1·R2·R5·R7·R8 | ✅ 플립 모델 스왑체인(버퍼 2, `B8G8R8A8`), 기능 수준 11_0→10_0, Debug 는 디버그 레이어(실패 시 없이 재시도), `Deferred` + `NonPremultiplied`, `ScreenGrab` 으로 Present 전 저장 |
| R3·R4 | ✅ `LoadImagePixels`(WIC → 32bpp BGRA, 알파 255·RGB 일치 → 투명), 원본 크기·밉맵 1·`IMMUTABLE` |
| R6 | ✅ 행렬 분해(위치 `_41,_42`, 크기 = 행 길이, 회전 `atan2(_12,_11)`) |
| R9 | ✅ Client `-d3d11` → `CRendererD3D11Tk`, 없으면 D3D9. 스모크 로그 `renderer` 줄로 확인 |
| R10 | ✅ 툴 exe 의존 DLL: `d3d9`·`d3dx9_43` 만(`d3d11` 없음). Client: `d3d11`·`d3d9`·`d3dx9_43` |
| R11 | ✅ **D3D9 와 D3D11 캡처가 허용 오차 0 으로 같다** — Win32 Debug·x64 Release 각 6장, 다른 픽셀 0. PNG 바이트는 다르다(인코더가 달라 D3D11 경로임을 확인). 허용 범위를 따로 정할 필요가 없었다 |
| R12 | ✅ 4개 구성 경고 0. 스모크 4개 구성 × 두 백엔드 각 3/3, 툴 스모크 4개 구성, `-d3d11` 일반 실행 살아 있음 |
| R13 | ✅ CLAUDE.md·ARCHITECTURE.md·로드맵 |

- 정수 좌표·크기 1 로 그리는 스프라이트는 텍스처 좌표가 텍셀 중심에 정확히 놓여 선형 필터링 결과가 두 API 에서 같다. 그래서 완전히 일치한 것으로 본다.
  크기 0.3 등으로 축소해 그리는 경우(툴의 미니맵 등)는 캡처 시점에 없어 차이가 날 수 있다 — A 의 비교 기준을 정할 때 다시 본다.
- ⏳ 디버그 레이어 메시지는 디버거 출력 창에서만 보여 자동으로 확인하지 못했다. A 가이드 0장에서 보는 방법을 다룬다.
