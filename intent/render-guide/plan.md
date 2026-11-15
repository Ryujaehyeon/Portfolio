# Plan: render-guide

Author: Ryu Jahyeon
Status: draft — `render-dxtk` 완료 후 확정

<!-- 입력: spec.md. 경로는 2D-Portfolio/ 기준. -->

## 영향받는 파일

| 파일 | 할 일 | 요구 |
|---|---|---|
| `Engine/RendererD3D11Custom.h/.cpp`(새) | 정답 구현 → 검증 → 뼈대로 되돌림 | R1·R7 |
| `Engine/Shaders/SpriteVS.hlsl`·`SpritePS.hlsl`(새) | 정답 셰이더 → 최소 내용으로 되돌림 | R2 |
| `Engine/Engine.vcxproj(.filters)` | 새 파일, `FxCompile` 설정 | R2 |
| `Client/Client.cpp`·`Client/MainGame.cpp` | `-d3d11custom` 선택 | R3 |
| `docs/render-guide/README.md`, `00-prepare.md` ~ `08-verify.md` | 가이드 | R5·R6·R9 |
| `docs/render-guide/solutions/01-*.md` ~ `08-*.md`, `solutions/complete/` | 정답 | R8 |
| `docs/render-guide/images/` | 장별 체크포인트 캡처(작은 PNG) | R6 |
| `CLAUDE.md`·`docs/RENDER-ROADMAP.md` | 사용법·위치 | R10 |

## 작업 순서

1. **뼈대와 빌드 설정.** 빈 백엔드, 셰이더 자리, `FxCompile`, `-d3d11custom` 선택.
   → 4개 구성 빌드, `-smoke -d3d11custom` PASS(R4).
   → 커밋 `feat(render-guide): 직접 구현할 D3D11 백엔드 뼈대와 셰이더 빌드 설정`
2. **장별 정답 구현.** 1장부터 8장까지 순서대로 구현한다. 장이 끝날 때마다 다음을 한다.
   - 체크포인트 화면을 캡처해 `images/` 에 넣는다.
   - 그 장에서 바뀐 부분을 `solutions/NN-*.md` 로 옮긴다.
   - 그 장의 본문(개념·할 일·힌트·흔한 실수)을 쓴다. 구현하면서 실제로 막힌 점을 「흔한 실수」에 반영한다.
3. **정답 검증.** 8장까지 마친 코드로 B 와 캡처 비교(R7) → 결과를 이 문서에 기록. 완성본을 `solutions/complete/` 에 복사.
4. **뼈대로 되돌림.** 백엔드·셰이더를 1번 상태로 되돌린다. 4개 구성 빌드·`-smoke -d3d11custom` PASS 를 다시 확인한다.
   → 커밋 `docs(render-guide): D3D11 스프라이트 렌더러 구현 가이드와 검증된 정답`
5. **문서 마무리.** README, 링크 확인(R9), CLAUDE.md·로드맵(R10) → PR → 머지.

## 테스트

```
# 뼈대 상태
../Exe/<플랫폼>/<구성>/Client/Client.exe -smoke -d3d11custom; echo $?   → 0

# 정답 검증(2~3번 동안)
../Exe/x64/Debug/Client/Client.exe -smoke -shot shots/b -d3d11
../Exe/x64/Debug/Client/Client.exe -smoke -shot shots/a -d3d11custom
powershell.exe -NoProfile -File ../../scripts/compare-shots.ps1 -Expected shots/b -Actual shots/a -Tolerance <3단계 값> -MaxRatio <3단계 값>
```
