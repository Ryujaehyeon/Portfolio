# Spec: render-dxtk

Author: Ryu Jahyeon
Status: draft

<!-- 입력: intent.md. 경로는 2D-Portfolio/ 기준. 2단계(render-backend)의 IRenderer 를 전제로 한다. -->

## 한 줄 요약

`CRendererD3D11Tk` 가 `IRenderer` 를 D3D11 장치·DXGI 스왑체인·DirectXTK `SpriteBatch` 로 구현한다.
텍스처는 WIC 로 디코딩해 컬러키를 직접 적용한다. Client 는 `-d3d11` 일 때 이 백엔드를 만든다.

## 요구사항

| ID | 요구사항 | 판정 방법 |
|---|---|---|
| R1 | **장치·스왑체인.** `D3D11CreateDevice`(기능 수준 11_0 → 10_1 → 10_0 순서로 시도), 스왑체인은 플립 모델(`DXGI_SWAP_EFFECT_FLIP_DISCARD`), 버퍼 2개, `B8G8R8A8_UNORM`, 크기 `WINSIZEX`×`WINSIZEY`, 창 모드. `Present(0, 0)`(지금처럼 수직 동기 없음) | 코드 검토 |
| R2 | **디버그 레이어.** Debug 구성에서는 `D3D11_CREATE_DEVICE_DEBUG` 로 만든다. 디버그 레이어가 설치돼 있지 않아 실패하면 플래그 없이 다시 만든다 | 코드 검토 |
| R3 | **이미지 디코더** `LoadImagePixels(경로, 컬러키) → 너비·높이·BGRA 픽셀 배열`. WIC 로 png·bmp·jpg 를 32비트 BGRA 로 디코딩하고, RGB 가 컬러키와 같은 픽셀을 완전 투명(0,0,0,0)으로 바꾼다. D3D 를 쓰지 않는 CPU 함수로 두어 4단계(A)에서도 그대로 쓴다 | 코드 검토 |
| R4 | **텍스처.** 디코딩한 픽셀로 원본 크기 그대로, 밉맵 1단계, `B8G8R8A8_UNORM`, `IMMUTABLE` 텍스처와 SRV 를 만든다. `TEXINFO::ImgInfo` 에 크기를 채운다 | 코드 검토 |
| R5 | **그리기.** `SpriteBatch` 를 `Deferred` 정렬(제출 순서 유지)과 비 premultiplied 알파 블렌드(`CommonStates::NonPremultiplied`)로 `BeginFrame` 에서 시작하고 `EndFrame` 에서 끝낸다 | 코드 검토 |
| R6 | **변환.** `DrawSprite` 의 월드 행렬을 크기·회전·이동으로 분해해 `SpriteBatch::Draw` 에 넘긴다. 중심점은 `origin`, 원본 사각형은 `sourceRectangle` 로 그대로 넘긴다. 행렬식이 음수(뒤집힘)면 `SpriteEffects` 로 처리한다 | 코드 검토 + 캡처 |
| R7 | **색.** `D3DCOLOR`(ARGB 8비트)를 0~1 실수 RGBA 로 바꿔 넘긴다 | 코드 검토 |
| R8 | **캡처.** `SaveBackBuffer` 는 `Present` 전에 백버퍼를 PNG 로 저장한다(DirectXTK `ScreenGrab`) | 실행 |
| R9 | **선택.** Client 는 명령줄에 `-d3d11` 이 있으면 `CRendererD3D11Tk`, 없으면 `CRendererD3D9` 를 만든다. 툴은 바꾸지 않는다 | 실행 |
| R10 | **툴 무영향.** 툴의 링크 입력에 `d3d11.lib`·`dxgi.lib`·DirectXTK 가 없고 툴 스모크가 PASS | 빌드 로그 |
| R11 | **화면 비교.** 같은 시점의 D3D9 캡처와 D3D11 캡처를 비교해 허용 범위 안이다. 허용 범위(채널 차이·비율)는 실제 결과를 보고 정해 plan 에 근거와 함께 기록한다. 첫 목표는 「채널 차이 8 이하인 픽셀이 99% 이상」 | 비교 스크립트 |
| R12 | `-d3d11` 스모크 4개 구성 PASS, 기본(D3D9) 스모크 PASS, 경고 0 | CLAUDE.md |
| R13 | 문서 — CLAUDE.md 실행 옵션, ARCHITECTURE.md 구조 그림, 로드맵 상태 | 문서 검토 |

## 설계

### 어디에 두나

| 클래스·함수 | 파일 | 비고 |
|---|---|---|
| `CRendererD3D11Tk` | `Engine/RendererD3D11Tk.h/.cpp` | B 백엔드 |
| `LoadImagePixels` | `Engine/ImageLoader.h/.cpp` | WIC 디코딩 + 컬러키. D3D 와 무관 — A 도 쓴다 |

Engine 은 정적 라이브러리다. 링커는 정적 라이브러리에서 실제로 참조된 오브젝트만 가져온다.
툴은 `CRendererD3D11Tk` 를 만들지 않으므로 이 오브젝트와 D3D11·DirectXTK 의존이 툴로 들어가지 않는다(R10 으로 확인).
D3D11 관련 라이브러리(`d3d11.lib`·`dxgi.lib`·DirectXTK)는 Client 만 링크한다.

### 장치와 스왑체인을 이렇게 만드는 이유

- **플립 모델**은 Windows 10 이후 권장 방식이다. 옛 비트 블록 전송 방식(`DISCARD`)보다 효율적이고, 최신 기능(가변 주사율 등)의 전제다.
- **버퍼 2개**는 플립 모델의 최소값이다.
- **`B8G8R8A8_UNORM`** 은 지금의 `D3DFMT_A8R8G8B8` 과 같은 메모리 배치다. 캡처를 비교하기 쉽다.
- 기능 수준을 11_0 아래까지 열어 두는 이유는 오래된 그래픽 장치에서도 실행되게 하려는 것이다. 스프라이트 렌더링은 10_0 기능으로 충분하다.

### 텍스처: 왜 DirectXTK 로더를 쓰지 않나

DirectXTK 의 `WICTextureLoader` 에는 컬러키 기능이 없다. 그런데 이 게임의 리소스는 배경색(검정·흰색)을 투명으로 바꿔 쓰는 전제로 만들어져 있다.
그래서 WIC 로 픽셀을 직접 받아 컬러키를 적용한 뒤 텍스처를 만든다.

D3DX 의 컬러키 규칙: 알파를 포함한 ARGB 가 키와 같은 픽셀을 투명한 검정(0,0,0,0)으로 바꾼다. 지금 키는 모두 알파 255 이고,
원본 이미지는 대부분 알파가 없거나 불투명이다. 그래서 「RGB 가 같고 알파가 255 인 픽셀」을 바꾸는 것과 결과가 같다. plan 에서 반투명 픽셀이 있는 이미지를 한 번 확인한다.

### 변환: D3DX 스프라이트와 SpriteBatch 의 대응

`ID3DXSprite` 는 원본 사각형 안의 점 `p` 를 다음처럼 화면에 놓는다.

```
화면 위치 = (p − 중심점) × 월드 행렬
```

`SpriteBatch::Draw(텍스처, 위치, 원본 사각형, 색, 회전, origin, 크기)` 는 다음과 같다.

```
화면 위치 = 회전((p − origin) × 크기) + 위치
```

지금 게임의 월드 행렬은 모두 「크기 × Z 회전 × 이동」 형태다. 그래서 다음처럼 분해하면 두 식이 같아진다.

| SpriteBatch 인자 | 월드 행렬에서 |
|---|---|
| 위치 | 이동 성분 `(m41, m42)` |
| 크기 | x 축 길이 `|(m11, m12)|`, y 축 길이 `|(m21, m22)|` |
| 회전 | `atan2(m12, m11)` |
| origin | 중심점 `(x, y)` 그대로 |

중심점과 origin 은 둘 다 「원본 사각형의 왼쪽 위」를 기준으로 한 좌표라 그대로 넘기면 된다.

### 블렌드와 정렬

- `D3DXSPRITE_ALPHABLEND` 는 일반(비 premultiplied) 알파 블렌드다(`SRCALPHA`, `INVSRCALPHA`).
  `SpriteBatch` 의 기본값은 premultiplied 이므로 `NonPremultiplied` 상태를 명시해야 같은 결과가 나온다.
- 그리는 순서가 곧 겹침 순서다. `Deferred` 는 제출 순서를 유지하면서 같은 텍스처가 연속될 때만 묶는다.

### 경고 정책(DirectXTK 헤더)

DirectXTK 헤더는 `/W3` 에서 경고가 없는 것으로 알려져 있지만, VS2026(v145)의 `/permissive-` 와 함께 확인된 적은 없다.
plan 1번에서 가장 먼저 빈 파일에 헤더만 포함해 빌드해 본다. 경고가 나오면 진행을 멈추고, 아래 선택지로 사용자 결정을 받는다.

1. 외부 헤더 경고 수준 `/external:W0` 로 서드파티 헤더만 제외한다(프로젝트 코드의 경고는 그대로).
2. DirectXTK 를 소스로 받아 빌드 설정을 직접 맞춘다.

## flag된 우려사항

| 무엇이 | 어느 기준에 | 해소 |
|---|---|---|
| 필터링·반올림 차이로 픽셀이 조금 다르다 | 화면 동일성 | 허용 범위를 수치로 정하고 근거(diff 이미지)를 남긴다. 위치가 어긋나는 차이(경계가 통째로 밀림)는 허용하지 않는다 |
| 깊이 테스트(D3D9 Client 의 `ZENABLE`) | 동작 차이 | D3D11 백엔드는 깊이 버퍼를 만들지 않는다. 캡처가 같으면 깊이가 실제로 쓰이지 않았다는 증거다 |
| NuGet DirectXTK 와 v145 도구 집합 | 빌드 | 패키지는 v143 으로 빌드되어 있다. MSVC 14.x 는 이진 호환이라 링크되어야 한다. 실패하면 소스 빌드로 바꾼다(위 선택지 2) |
| 오래된 GPU·원격 데스크톱 | 실행 | 기능 수준 10_0 까지 연다. 그래도 실패하면 오류를 보고하고 D3D9 로 실행하라고 안내한다 |
