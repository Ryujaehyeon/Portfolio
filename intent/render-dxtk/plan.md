# Plan: render-dxtk

Author: Ryu Jahyeon
Status: draft — `render-backend` 완료 후 확정

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
