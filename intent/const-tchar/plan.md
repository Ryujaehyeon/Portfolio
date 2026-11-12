# Plan: const-tchar

Author: Ryu Jahyeon
Status: approved (사용자 위임 — 「진행해」, 2026-11-12)

<!-- 입력: 승인된 spec.md (PR #34). 경로는 2D-Portfolio/ 기준. -->

## 영향받는 파일

| 파일 | 할 일 |
|---|---|
| `Headers/Constant.h` | 이름 상수 38개 `const TCHAR* const`(R1) |
| `Headers/Debug.hpp` | `__writew`·`__writea` 포맷 인자 `const`(R2) |
| `Client/Obj.h` | 멤버 `m_pObjName`·`m_pObjKey`·`m_pStateKey`·`m_pMotion` `const TCHAR*`(R2) |
| `Client/*.h`·`*.cpp`(`CObj` 계열 생성자·`DirectAction`·`SkillActive`·`SetMotion`·`FontOutput`·`VelueToPercentage` 등) | 읽기 전용 인자 40곳 `const TCHAR*`(R2) |
| `Client/MenuButton.h`·`Player.h` | 멤버 `m_DrawID`·`m_SkillActiveName` `const`(R2) |
| `Client/ObjMgr.cpp`·`Stage.cpp`·`UIObj.cpp` | 지역 문자열 배열 5개 `const`(R2) |
| `Client/{MenuButton,Monster,Player,ObjMgr,UIObj}.cpp`, `MFC_000/{MFC_000View,MiniView}.cpp` | 임시 값 주소 16곳 → 지역 변수(R3) |
| `Client/Client.vcxproj`·`MFC_000/MFC_000.vcxproj` | 4개 구성 `ConformanceMode=true`(R5) |
| `CLAUDE.md` | 「빌드」·「코드 관례」(R8) |

## 작업 순서

1. **const 화·임시 값 주소** — `_CL_=/permissive-` Win32 Debug 증분 빌드의 오류를 따라 고친다(근원: 이름 상수·`CObj` 멤버 → 연쇄: 인자·멤버·지역 배열·`Debug.hpp` → C2102).
   → `/permissive-` 오류 0, 기존 설정 Win32 Debug 빌드·스모크 PASS. 커밋 `refactor(const-tchar): 읽기 전용 문자열 포인터를 const 로, 임시 값 주소 제거`
2. **ConformanceMode 켜기** — 두 vcxproj 의 4개 구성 `ClCompile` 에 `<ConformanceMode>true</ConformanceMode>`. 커밋 `chore(const-tchar): 두 프로젝트에 표준 준수 모드(/permissive-)를 켠다`
3. **전체 검증**(클린 4개 구성, Client 스모크 3회씩, 툴 스모크, 일반 실행) → **문서** → PR → 머지 → 브랜치 삭제.

## 테스트

- R5: 4개 구성 tlog 에 `/permissive-`(Client·MFC_000).
- R6: 경고 Win32 196, x64 Debug 268·Release 206 — 경고 번호별 집계 비교.
- R4: `git diff master -- 2D-Portfolio | grep const_cast` 0줄.

## 리스크

- `/permissive-` 가 새 경고를 낼 수 있음 → R6 집계로 확인, 생기면 고친다.
- 툴 동작은 스모크가 타일 읽기만 본다 → 일반 실행으로 창이 뜨는지만 확인(툴 변경은 그리기 중심점을 지역 변수로 받은 것뿐).
