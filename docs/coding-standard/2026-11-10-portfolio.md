# 코딩 표준 검사: portfolio

- 일시: 2026-11-10 16:09
- 대상: `./` (저장소 전체), 파일 146개(`.cpp` 67 · `.h`/`.hpp` 79) — 검사 제외 경로 뺀 수
- 기준: `.harness/standards/cpp11-coding-standard.md` · 범위: CLAUDE.md (「고치는 코드」 행: I-56, I-26, I-27, III-4, III-1, III-10, II-4, I-23, I-57)
- 결과: 위반 1084건 (항목 9종) · 미검사: 없음 (I-26·I-27 은 부분 검사 — 「미검사·제외」 참고)

## 항목별 집계

| 항목 | 건수 | 판정 방법 |
|---|---|---|
| I-56 모든 컴파일러 경고는 반드시 고친다 | 279 | Debug\|Win32 전체 Rebuild 출력, 위치(파일·줄·코드) 중복 제거 — C4244 175 · C4002 84 · C4838 17 · C4715 2 · C4018 1 |
| I-57 변수 가리기(variable shadowing)는 허용되지 않는다 | 18 | C4456~C4459 를 켠 Rebuild 출력(`CL=/w14456 /w14457 /w14458 /w14459`), 위치 중복 제거 |
| I-23 switch case 문에는 항상 `default:`를 넣는다 | 7 | switch 본문 최상위에 `default:` 없음 |
| II-4 중괄호 안에 코드가 한 줄만 있더라도 반드시 중괄호를 사용한다 | 275 | `if`·`else`·`for`·`while` 뒤 문장이 `{` 로 시작하지 않음(do-while 꼬리 제외) |
| III-4 포인터에 `NULL` 대신 `nullptr`를 사용한다 | 179 | `NULL`, 그리고 포인터 이름(`p`·`m_p`·`g_p` 접두)에 쓴 리터럴 `0` — 줄 단위 |
| III-1 `override`와 `final` 키워드를 반드시 사용한다 | 124 | 저장소 안 기반 클래스의 가상 함수를 재정의하면서 `override` 없음. 기반이 MFC 등 외부인 `virtual` 선언 28건은 「판단 필요」 |
| III-10 단순 상수 변수에는 `const` 대신 `constexpr`을 사용한다 | 99 | 리터럴·상수만으로 초기화한 `const` 스칼라 변수 |
| I-26 원칙적으로 모든 곳에 `const`를 사용한다 | 82 | 값을 바꾸지 않는 스칼라(정수·실수·bool·enum) 지역 변수와 값 매개변수 |
| I-27 개체를 수정하지 않는 멤버 함수에는 모두 `const`를 붙인다 | 21 | `Get`·`Is`·`Has`·`Can` 으로 시작하고 멤버를 바꾸지 않는 비 `const` 멤버 함수 |

## 요약

상세에 쓴 건만 싣는다. 10건을 넘는 항목은 대표 3건만 상세로 쓰고 나머지는 「동일 패턴」에 위치만 적었다.

| # | 파일:줄 | 위반 항목 | 비고 |
|---|---|---|---|
| 1 | `2D-Portfolio/Client/Stage.cpp:1006` | I-56 모든 컴파일러 경고는 반드시 고친다 | C4002 |
| 2 | `2D-Portfolio/Client/Stage2.cpp:60` | I-56 모든 컴파일러 경고는 반드시 고친다 | C4715 |
| 3 | `2D-Portfolio/Client/Blizzard.cpp:184` | I-56 모든 컴파일러 경고는 반드시 고친다 | C4244 |
| 4 | `2D-Portfolio/Client/Monster.cpp:139` | I-57 변수 가리기(variable shadowing)는 허용되지 않는다 | C4456 |
| 5 | `2D-Portfolio/MFC_000/Page3.cpp:137` | I-57 변수 가리기(variable shadowing)는 허용되지 않는다 | C4457 |
| 6 | `2D-Portfolio/Headers/Debug.hpp:63` | I-57 변수 가리기(variable shadowing)는 허용되지 않는다 | C4459 |
| 7 | `2D-Portfolio/Client/Client.cpp:233` | I-23 switch case 문에는 항상 `default:`를 넣는다 | switch 233줄 ~ 245줄 |
| 8 | `2D-Portfolio/Client/Monster.cpp:267` | I-23 switch case 문에는 항상 `default:`를 넣는다 | switch 267줄 ~ 302줄 |
| 9 | `2D-Portfolio/Client/ObjMgr.cpp:274` | I-23 switch case 문에는 항상 `default:`를 넣는다 | switch 274줄 ~ 349줄 |
| 10 | `2D-Portfolio/Client/TextureMgr.cpp:28` | I-23 switch case 문에는 항상 `default:`를 넣는다 | switch 28줄 ~ 36줄 |
| 11 | `2D-Portfolio/MFC_000/MFC_000View.cpp:539` | I-23 switch case 문에는 항상 `default:`를 넣는다 | switch 539줄 ~ 553줄 |
| 12 | `2D-Portfolio/MFC_000/MainFrm.cpp:252` | I-23 switch case 문에는 항상 `default:`를 넣는다 | switch 252줄 ~ 269줄 |
| 13 | `2D-Portfolio/MFC_000/TextureMgr.cpp:24` | I-23 switch case 문에는 항상 `default:`를 넣는다 | switch 24줄 ~ 32줄 |
| 14 | `2D-Portfolio/Client/AStar.cpp:21` | II-4 중괄호 안에 코드가 한 줄만 있더라도 반드시 중괄호를 사용한다 |  |
| 15 | `2D-Portfolio/Client/TextureMgr.cpp:62` | II-4 중괄호 안에 코드가 한 줄만 있더라도 반드시 중괄호를 사용한다 |  |
| 16 | `2D-Portfolio/Client/Stage2.cpp:55` | II-4 중괄호 안에 코드가 한 줄만 있더라도 반드시 중괄호를 사용한다 |  |
| 17 | `2D-Portfolio/Client/Device.cpp:7` | III-4 포인터에 `NULL` 대신 `nullptr`를 사용한다 |  |
| 18 | `2D-Portfolio/Client/TextureMgr.cpp:27` | III-4 포인터에 `NULL` 대신 `nullptr`를 사용한다 |  |
| 19 | `2D-Portfolio/Client/Monster.cpp:132` | III-4 포인터에 `NULL` 대신 `nullptr`를 사용한다 |  |
| 20 | `2D-Portfolio/Client/Logo.h:10` | III-1 `override`와 `final` 키워드를 반드시 사용한다 |  |
| 21 | `2D-Portfolio/Client/StageDynamic.h:16` | III-1 `override`와 `final` 키워드를 반드시 사용한다 |  |
| 22 | `2D-Portfolio/MFC_000/MFC_000View.h:46` | III-1 `override`와 `final` 키워드를 반드시 사용한다 | 판단 필요 |
| 23 | `2D-Portfolio/Headers/Constant.h:8` | III-10 단순 상수 변수에는 `const` 대신 `constexpr`을 사용한다 |  |
| 24 | `2D-Portfolio/Client/Value.h:5` | III-10 단순 상수 변수에는 `const` 대신 `constexpr`을 사용한다 |  |
| 25 | `2D-Portfolio/MFC_000/MainFrm.cpp:22` | III-10 단순 상수 변수에는 `const` 대신 `constexpr`을 사용한다 |  |
| 26 | `2D-Portfolio/MFC_000/Page1.cpp:137` | I-26 원칙적으로 모든 곳에 `const`를 사용한다 |  |
| 27 | `2D-Portfolio/Client/ObjMgr.cpp:476` | I-26 원칙적으로 모든 곳에 `const`를 사용한다 |  |
| 28 | `2D-Portfolio/Client/Device.cpp:13` | I-26 원칙적으로 모든 곳에 `const`를 사용한다 |  |
| 29 | `2D-Portfolio/Client/ObjMgr.h:31` | I-27 개체를 수정하지 않는 멤버 함수에는 모두 `const`를 붙인다 |  |
| 30 | `2D-Portfolio/Client/Device.h:29` | I-27 개체를 수정하지 않는 멤버 함수에는 모두 `const`를 붙인다 |  |
| 31 | `2D-Portfolio/Client/AStar.h:16` | I-27 개체를 수정하지 않는 멤버 함수에는 모두 `const`를 붙인다 | 판단 필요 |

## 상세

### 1. `2D-Portfolio/Client/Stage.cpp:1006` — I-56 모든 컴파일러 경고는 반드시 고친다

**수정 전**
```cpp
			ERR_MSG(g_hWnd, L"%d Run L 로드 실패" , Monster[i]);
```

**수정 후**
```cpp
			TCHAR szMsg[128];
			swprintf_s(szMsg, L"%d Run L 로드 실패", Monster[i]);
			ERR_MSG(g_hWnd, szMsg);
```

> 주의: `C4002`: `ERR_MSG(hwnd, sz)` 는 인수 2개짜리 매크로(`Headers/Macro.h:22`)라 `Monster[i]` 가 버려지고 `%d` 가 그대로 출력된다. 같은 원인의 C4002 가 84건(모두 `Stage.cpp`) — 형식 문자열을 받는 함수로 바꿀지는 별도 결정이 필요하다.

### 2. `2D-Portfolio/Client/Stage2.cpp:60` — I-56 모든 컴파일러 경고는 반드시 고친다

**수정 전**
```cpp
	if(iScene == SCENEID_END)
		return SCENEID_END;
}
```

**수정 후**
```cpp
	if(iScene == SCENEID_END)
		return SCENEID_END;

	return iScene;
}
```

> 주의: `C4715`: 끝에 반환문이 없어 `SCENEID_END` 가 아닐 때 반환값이 정의되지 않는다(정의되지 않은 동작). 추가하는 `return iScene;` 는 `CStage3`·`CMenuD2` 의 같은 함수와 같은 흐름이다. 같은 경고: `Client/UIObj.cpp:536`(`CUIObj::VelueToPercentage`).

### 3. `2D-Portfolio/Client/Blizzard.cpp:184` — I-56 모든 컴파일러 경고는 반드시 고친다

**수정 전**
```cpp
	m_sPlayInfo.fMaxExp = m_sPlayInfo.iLevel*20;
```

**수정 후**
```cpp
	m_sPlayInfo.fMaxExp = float(m_sPlayInfo.iLevel*20);
```

> 주의: `C4244`(175건)·`C4838`(17건)·`C4018`(1건)은 대부분 이처럼 명시적 형변환으로 의도를 드러내면 사라진다. 형변환 문법은 주변 코드(`int(...)`)에 맞췄다.

### 4. `2D-Portfolio/Client/Monster.cpp:139` — I-57 변수 가리기(variable shadowing)는 허용되지 않는다

**수정 전**
```cpp
		const TEXINFO* pTexInfo 
			= GET_SINGLE(CTextureMgr)->GetTexture(L"Shadow", L"Shadow", 0);
```

**수정 후**
```cpp
		const TEXINFO* pShadowTexInfo 
			= GET_SINGLE(CTextureMgr)->GetTexture(L"Shadow", L"Shadow", 0);
```

> 주의: `C4456`: 128줄의 바깥 `pTexInfo` 를 가린다. 이름을 바꾸면 이 블록 안에서 `pTexInfo` 를 쓰는 줄(142·145 등)도 함께 바꿔야 한다.

### 5. `2D-Portfolio/MFC_000/Page3.cpp:137` — I-57 변수 가리기(variable shadowing)는 허용되지 않는다

**수정 전**
```cpp
			float nPos = m_ctrSlider.GetPos() * 0.01f;
			((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_fScrollval = nPos;
```

**수정 후**
```cpp
			float fScale = m_ctrSlider.GetPos() * 0.01f;
			((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_fScrollval = fScale;
```

> 주의: `C4457`: `OnHScroll` 매개변수 `nPos` 를 가린다. 이 블록에서 지역 `nPos` 를 쓰는 다른 줄도 함께 바꾼다.

### 6. `2D-Portfolio/Headers/Debug.hpp:63` — I-57 변수 가리기(variable shadowing)는 허용되지 않는다

**수정 전**
```cpp
		static __debugclass* hInst = 0;
		if (hInst == 0)
			hInst = new __debugclass;
		return &hInst;
```

**수정 후**
```cpp
		static __debugclass* s_pInst = 0;
		if (s_pInst == 0)
			s_pInst = new __debugclass;
		return &s_pInst;
```

> 주의: `C4459`: `Client/Client.cpp:12` 의 전역 `HINSTANCE hInst` 를 가린다(Client 의 모든 번역 단위에서 반복 — 경고 66건이 이 두 줄에서 나온다). 같은 파일 70줄의 지역 `hInst` 도 이름을 바꿔야 한다. 이 줄의 `0`(III-4)·중괄호(II-4)는 별도 항목이라 그대로 두었다.

### 7. `2D-Portfolio/Client/Client.cpp:233` — I-23 switch case 문에는 항상 `default:`를 넣는다

**수정 전**
```cpp
		}
		break;
	}
```

**수정 후**
```cpp
		}
		break;
	default:
		break;
	}
```

### 8. `2D-Portfolio/Client/Monster.cpp:267` — I-23 switch case 문에는 항상 `default:`를 넣는다

**수정 전**
```cpp
					m_vMovePoint = D3DXVECTOR3(m_Info.vPos.x , m_Info.vPos.y + moveLength, 0);
					break;
				}
```

**수정 후**
```cpp
					m_vMovePoint = D3DXVECTOR3(m_Info.vPos.x , m_Info.vPos.y + moveLength, 0);
					break;
				default:
					break;
				}
```

### 9. `2D-Portfolio/Client/ObjMgr.cpp:274` — I-23 switch case 문에는 항상 `default:`를 넣는다

**수정 전**
```cpp
					break;
				}
			}
```

**수정 후**
```cpp
					break;
				}
			default:
				break;
			}
```

### 10. `2D-Portfolio/Client/TextureMgr.cpp:28` — I-23 switch case 문에는 항상 `default:`를 넣는다

**수정 전**
```cpp
			pTexture = new CMultiTexture;
			break;
		}
```

**수정 후**
```cpp
			pTexture = new CMultiTexture;
			break;
		default:
			break;
		}
```

> 주의: `default` 에서 `pTexture` 가 `NULL` 로 남으면 바로 아래에서 역참조한다. `return E_FAIL;` 로 끝낼지 정한다. `MFC_000/TextureMgr.cpp` 사본도 같은 구조다.

### 11. `2D-Portfolio/MFC_000/MFC_000View.cpp:539` — I-23 switch case 문에는 항상 `default:`를 넣는다

**수정 전**
```cpp
				}
			}
		}
```

**수정 후**
```cpp
				}
			}
		default:
			break;
		}
```

### 12. `2D-Portfolio/MFC_000/MainFrm.cpp:252` — I-23 switch case 문에는 항상 `default:`를 넣는다

**수정 전**
```cpp
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}
```

**수정 후**
```cpp
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		default:
			break;
		}
```

### 13. `2D-Portfolio/MFC_000/TextureMgr.cpp:24` — I-23 switch case 문에는 항상 `default:`를 넣는다

**수정 전**
```cpp
		pTexture = new CMultiTexture;
		break;
	}
```

**수정 후**
```cpp
		pTexture = new CMultiTexture;
		break;
	default:
		break;
	}
```

> 주의: `Client/TextureMgr.cpp` 와 같은 구조(사본) — 함께 고친다.

### 14. `2D-Portfolio/Client/AStar.cpp:21` — II-4 중괄호 안에 코드가 한 줄만 있더라도 반드시 중괄호를 사용한다

**수정 전**
```cpp
	if (pVecTile == NULL)
		return;
```

**수정 후**
```cpp
	if (pVecTile == NULL)
	{
		return;
	}
```

### 15. `2D-Portfolio/Client/TextureMgr.cpp:62` — II-4 중괄호 안에 코드가 한 줄만 있더라도 반드시 중괄호를 사용한다

**수정 전**
```cpp
	if(iter == m_MapTexture.end())
		return NULL;
```

**수정 후**
```cpp
	if(iter == m_MapTexture.end())
	{
		return NULL;
	}
```

### 16. `2D-Portfolio/Client/Stage2.cpp:55` — II-4 중괄호 안에 코드가 한 줄만 있더라도 반드시 중괄호를 사용한다

**수정 전**
```cpp
	if(iScene > SCENEID_NONPASS)
		GET_SINGLE(CSceneMgr)->InitScene(iScene);
```

**수정 후**
```cpp
	if(iScene > SCENEID_NONPASS)
	{
		GET_SINGLE(CSceneMgr)->InitScene(iScene);
	}
```

### 17. `2D-Portfolio/Client/Device.cpp:7` — III-4 포인터에 `NULL` 대신 `nullptr`를 사용한다

**수정 전**
```cpp
	,m_pDevice(0)
```

**수정 후**
```cpp
	,m_pDevice(nullptr)
```

### 18. `2D-Portfolio/Client/TextureMgr.cpp:27` — III-4 포인터에 `NULL` 대신 `nullptr`를 사용한다

**수정 전**
```cpp
		CTexture* pTexture = NULL;
```

**수정 후**
```cpp
		CTexture* pTexture = nullptr;
```

### 19. `2D-Portfolio/Client/Monster.cpp:132` — III-4 포인터에 `NULL` 대신 `nullptr`를 사용한다

**수정 전**
```cpp
	if(pTexInfo == NULL)
```

**수정 후**
```cpp
	if(pTexInfo == nullptr)
```

### 20. `2D-Portfolio/Client/Logo.h:10` — III-1 `override`와 `final` 키워드를 반드시 사용한다

**수정 전**
```cpp
	virtual void Render();
```

**수정 후**
```cpp
	virtual void Render() override;
```

### 21. `2D-Portfolio/Client/StageDynamic.h:16` — III-1 `override`와 `final` 키워드를 반드시 사용한다

**수정 전**
```cpp
	virtual void Release()PURE;
```

**수정 후**
```cpp
	virtual void Release() override PURE;
```

> 주의: `PURE` 는 `= 0` 매크로다. `override` 는 순수 지정자 앞에 둔다.

### 22. `2D-Portfolio/MFC_000/MFC_000View.h:46` — III-1 `override`와 `final` 키워드를 반드시 사용한다

**수정 전**
```cpp
	virtual void Dump(CDumpContext& dc) const;
```

**수정 후**
```cpp
	virtual void Dump(CDumpContext& dc) const override;
```

> 주의: 판단 필요: 기반 클래스가 MFC(`CView`)라 저장소 안에서 가상 함수 선언을 확인할 수 없다. MFC 마법사가 만든 재정의 선언이다.

### 23. `2D-Portfolio/Headers/Constant.h:8` — III-10 단순 상수 변수에는 `const` 대신 `constexpr`을 사용한다

**수정 전**
```cpp
const int TILESIZEY = 68;		 /*  맵타일의 세로축크기  */
```

**수정 후**
```cpp
constexpr int TILESIZEY = 68;		 /*  맵타일의 세로축크기  */
```

### 24. `2D-Portfolio/Client/Value.h:5` — III-10 단순 상수 변수에는 `const` 대신 `constexpr`을 사용한다

**수정 전**
```cpp
const DWORD KEY_RIGHT	=	0x00000008; // 0000 0000 0000 0000 0000 0000 0000 1000
```

**수정 후**
```cpp
constexpr DWORD KEY_RIGHT	=	0x00000008; // 0000 0000 0000 0000 0000 0000 0000 1000
```

### 25. `2D-Portfolio/MFC_000/MainFrm.cpp:22` — III-10 단순 상수 변수에는 `const` 대신 `constexpr`을 사용한다

**수정 전**
```cpp
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
```

**수정 후**
```cpp
constexpr UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
```

> 주의: 초기값의 `AFX_IDW_CONTROLBAR_FIRST` 가 상수 식이어야 한다(MFC 매크로 상수라 해당).

### 26. `2D-Portfolio/MFC_000/Page1.cpp:137` — I-26 원칙적으로 모든 곳에 `const`를 사용한다

**수정 전**
```cpp
	int iIndex = m_Combo.GetCurSel();
```

**수정 후**
```cpp
	const int iIndex = m_Combo.GetCurSel();
```

### 27. `2D-Portfolio/Client/ObjMgr.cpp:476` — I-26 원칙적으로 모든 곳에 `const`를 사용한다

**수정 전**
```cpp
		float fPushRange = fDistance - fRealDistance;
```

**수정 후**
```cpp
		const float fPushRange = fDistance - fRealDistance;
```

### 28. `2D-Portfolio/Client/Device.cpp:13` — I-26 원칙적으로 모든 곳에 `const`를 사용한다

**수정 전**
```cpp
HRESULT CDevice::InitDevice(WINMODE Mode)
```

**수정 후**
```cpp
HRESULT CDevice::InitDevice(const WINMODE Mode)
```

> 주의: 값 매개변수의 최상위 `const` 는 시그니처에 들어가지 않아 헤더 선언은 그대로 둬도 된다.

### 29. `2D-Portfolio/Client/ObjMgr.h:31` — I-27 개체를 수정하지 않는 멤버 함수에는 모두 `const`를 붙인다

**수정 전**
```cpp
	int GetMonsterCnt(){return m_MonsterCount;}
```

**수정 후**
```cpp
	int GetMonsterCnt() const {return m_MonsterCount;}
```

### 30. `2D-Portfolio/Client/Device.h:29` — I-27 개체를 수정하지 않는 멤버 함수에는 모두 `const`를 붙인다

**수정 전**
```cpp
	LPD3DXSPRITE GetSprite()
	{ return m_pSprite; }
```

**수정 후**
```cpp
	LPD3DXSPRITE GetSprite() const
	{ return m_pSprite; }
```

> 주의: `MFC_000/Device.h:17` 사본도 같은 함수가 있다 — 함께 고칠지 확인한다.

### 31. `2D-Portfolio/Client/AStar.h:16` — I-27 개체를 수정하지 않는 멤버 함수에는 모두 `const`를 붙인다

**수정 전**
```cpp
	list<int>* GetBestList()
	{
		return &m_BestList;
```

**수정 후**
```cpp
	const list<int>* GetBestList() const
	{
		return &m_BestList;
```

> 주의: `const` 멤버 함수에서는 `&m_BestList` 가 `const list<int>*` 라 반환형도 바뀐다. 호출부가 반환된 리스트를 수정하면 컴파일 오류가 나므로 호출부 확인이 먼저다.

## 동일 패턴 (대표 3건 외)

| 위반 항목 | 위치 |
|---|---|
| I-56 `C4002` (83) | `2D-Portfolio/Client/Stage.cpp:62`, `2D-Portfolio/Client/Stage.cpp:74`, `2D-Portfolio/Client/Stage.cpp:409`, `2D-Portfolio/Client/Stage.cpp:418`, `2D-Portfolio/Client/Stage.cpp:427`, `2D-Portfolio/Client/Stage.cpp:436`, `2D-Portfolio/Client/Stage.cpp:445`, `2D-Portfolio/Client/Stage.cpp:454`, `2D-Portfolio/Client/Stage.cpp:463`, `2D-Portfolio/Client/Stage.cpp:472`, `2D-Portfolio/Client/Stage.cpp:485`, `2D-Portfolio/Client/Stage.cpp:494`, `2D-Portfolio/Client/Stage.cpp:503`, `2D-Portfolio/Client/Stage.cpp:512`, `2D-Portfolio/Client/Stage.cpp:521`, `2D-Portfolio/Client/Stage.cpp:530`, `2D-Portfolio/Client/Stage.cpp:539`, `2D-Portfolio/Client/Stage.cpp:548`, `2D-Portfolio/Client/Stage.cpp:561`, `2D-Portfolio/Client/Stage.cpp:570`, `2D-Portfolio/Client/Stage.cpp:579`, `2D-Portfolio/Client/Stage.cpp:588`, `2D-Portfolio/Client/Stage.cpp:597`, `2D-Portfolio/Client/Stage.cpp:606`, `2D-Portfolio/Client/Stage.cpp:615`, `2D-Portfolio/Client/Stage.cpp:624`, `2D-Portfolio/Client/Stage.cpp:637`, `2D-Portfolio/Client/Stage.cpp:646`, `2D-Portfolio/Client/Stage.cpp:655`, `2D-Portfolio/Client/Stage.cpp:664`, `2D-Portfolio/Client/Stage.cpp:673`, `2D-Portfolio/Client/Stage.cpp:682`, `2D-Portfolio/Client/Stage.cpp:691`, `2D-Portfolio/Client/Stage.cpp:700`, `2D-Portfolio/Client/Stage.cpp:714`, `2D-Portfolio/Client/Stage.cpp:723`, `2D-Portfolio/Client/Stage.cpp:732`, `2D-Portfolio/Client/Stage.cpp:741`, `2D-Portfolio/Client/Stage.cpp:750`, `2D-Portfolio/Client/Stage.cpp:759`, `2D-Portfolio/Client/Stage.cpp:768`, `2D-Portfolio/Client/Stage.cpp:777`, `2D-Portfolio/Client/Stage.cpp:797`, `2D-Portfolio/Client/Stage.cpp:806`, `2D-Portfolio/Client/Stage.cpp:815`, `2D-Portfolio/Client/Stage.cpp:824`, `2D-Portfolio/Client/Stage.cpp:833`, `2D-Portfolio/Client/Stage.cpp:842`, `2D-Portfolio/Client/Stage.cpp:851`, `2D-Portfolio/Client/Stage.cpp:860`, `2D-Portfolio/Client/Stage.cpp:952`, `2D-Portfolio/Client/Stage.cpp:961`, `2D-Portfolio/Client/Stage.cpp:970`, `2D-Portfolio/Client/Stage.cpp:979`, `2D-Portfolio/Client/Stage.cpp:988`, `2D-Portfolio/Client/Stage.cpp:997`, `2D-Portfolio/Client/Stage.cpp:1015`, `2D-Portfolio/Client/Stage.cpp:1105`, `2D-Portfolio/Client/Stage.cpp:1114`, `2D-Portfolio/Client/Stage.cpp:1123`, `2D-Portfolio/Client/Stage.cpp:1132`, `2D-Portfolio/Client/Stage.cpp:1141`, `2D-Portfolio/Client/Stage.cpp:1150`, `2D-Portfolio/Client/Stage.cpp:1159`, `2D-Portfolio/Client/Stage.cpp:1168`, `2D-Portfolio/Client/Stage.cpp:1258`, `2D-Portfolio/Client/Stage.cpp:1267`, `2D-Portfolio/Client/Stage.cpp:1276`, `2D-Portfolio/Client/Stage.cpp:1285`, `2D-Portfolio/Client/Stage.cpp:1294`, `2D-Portfolio/Client/Stage.cpp:1303`, `2D-Portfolio/Client/Stage.cpp:1312`, `2D-Portfolio/Client/Stage.cpp:1321`, `2D-Portfolio/Client/Stage.cpp:1546`, `2D-Portfolio/Client/Stage.cpp:1555`, `2D-Portfolio/Client/Stage.cpp:1564`, `2D-Portfolio/Client/Stage.cpp:1573`, `2D-Portfolio/Client/Stage.cpp:1582`, `2D-Portfolio/Client/Stage.cpp:1591`, `2D-Portfolio/Client/Stage.cpp:1600`, `2D-Portfolio/Client/Stage.cpp:1609`, `2D-Portfolio/Client/Stage.cpp:1623`, `2D-Portfolio/Client/Stage.cpp:1636` |
| I-56 `C4018` (1) | `2D-Portfolio/MFC_000/MFC_000View.cpp:502` |
| I-56 `C4244` (174) | `2D-Portfolio/Client/Blizzard.cpp:42`, `2D-Portfolio/Client/BoneSpear.cpp:43`, `2D-Portfolio/Client/BoneSpear.cpp:150`, `2D-Portfolio/Client/Client.cpp:36`, `2D-Portfolio/Client/FireWall.cpp:42`, `2D-Portfolio/Client/Monster.cpp:23`, `2D-Portfolio/Client/Monster.cpp:31`, `2D-Portfolio/Client/Monster.cpp:146`, `2D-Portfolio/Client/Monster.cpp:161`, `2D-Portfolio/Client/Monster.cpp:172`, `2D-Portfolio/Client/Monster.cpp:189`, `2D-Portfolio/Client/Monster.cpp:228`, `2D-Portfolio/Client/Monster.cpp:530`, `2D-Portfolio/Client/Monster.cpp:531`, `2D-Portfolio/Client/Obj.cpp:150`, `2D-Portfolio/Client/Obj.cpp:151`, `2D-Portfolio/Client/ObjMgr.cpp:568`, `2D-Portfolio/Client/Player.cpp:71`, `2D-Portfolio/Client/Player.cpp:172`, `2D-Portfolio/Client/Player.cpp:417`, `2D-Portfolio/Client/Player.cpp:418`, `2D-Portfolio/Client/Player.cpp:778`, `2D-Portfolio/Client/Stage.cpp:405`, `2D-Portfolio/Client/Stage.cpp:414`, `2D-Portfolio/Client/Stage.cpp:423`, `2D-Portfolio/Client/Stage.cpp:432`, `2D-Portfolio/Client/Stage.cpp:441`, `2D-Portfolio/Client/Stage.cpp:450`, `2D-Portfolio/Client/Stage.cpp:459`, `2D-Portfolio/Client/Stage.cpp:468`, `2D-Portfolio/Client/Stage.cpp:481`, `2D-Portfolio/Client/Stage.cpp:490`, `2D-Portfolio/Client/Stage.cpp:499`, `2D-Portfolio/Client/Stage.cpp:508`, `2D-Portfolio/Client/Stage.cpp:517`, `2D-Portfolio/Client/Stage.cpp:526`, `2D-Portfolio/Client/Stage.cpp:535`, `2D-Portfolio/Client/Stage.cpp:544`, `2D-Portfolio/Client/Stage.cpp:557`, `2D-Portfolio/Client/Stage.cpp:566`, `2D-Portfolio/Client/Stage.cpp:575`, `2D-Portfolio/Client/Stage.cpp:584`, `2D-Portfolio/Client/Stage.cpp:593`, `2D-Portfolio/Client/Stage.cpp:602`, `2D-Portfolio/Client/Stage.cpp:611`, `2D-Portfolio/Client/Stage.cpp:620`, `2D-Portfolio/Client/Stage.cpp:633`, `2D-Portfolio/Client/Stage.cpp:642`, `2D-Portfolio/Client/Stage.cpp:651`, `2D-Portfolio/Client/Stage.cpp:660`, `2D-Portfolio/Client/Stage.cpp:669`, `2D-Portfolio/Client/Stage.cpp:678`, `2D-Portfolio/Client/Stage.cpp:687`, `2D-Portfolio/Client/Stage.cpp:696`, `2D-Portfolio/Client/Stage.cpp:710`, `2D-Portfolio/Client/Stage.cpp:719`, `2D-Portfolio/Client/Stage.cpp:728`, `2D-Portfolio/Client/Stage.cpp:737`, `2D-Portfolio/Client/Stage.cpp:746`, `2D-Portfolio/Client/Stage.cpp:755`, `2D-Portfolio/Client/Stage.cpp:764`, `2D-Portfolio/Client/Stage.cpp:773`, `2D-Portfolio/Client/Stage.cpp:793`, `2D-Portfolio/Client/Stage.cpp:802`, `2D-Portfolio/Client/Stage.cpp:811`, `2D-Portfolio/Client/Stage.cpp:820`, `2D-Portfolio/Client/Stage.cpp:829`, `2D-Portfolio/Client/Stage.cpp:838`, `2D-Portfolio/Client/Stage.cpp:847`, `2D-Portfolio/Client/Stage.cpp:856`, `2D-Portfolio/Client/Stage.cpp:871`, `2D-Portfolio/Client/Stage.cpp:880`, `2D-Portfolio/Client/Stage.cpp:889`, `2D-Portfolio/Client/Stage.cpp:898`, `2D-Portfolio/Client/Stage.cpp:907`, `2D-Portfolio/Client/Stage.cpp:916`, `2D-Portfolio/Client/Stage.cpp:925`, `2D-Portfolio/Client/Stage.cpp:934`, `2D-Portfolio/Client/Stage.cpp:948`, `2D-Portfolio/Client/Stage.cpp:957`, `2D-Portfolio/Client/Stage.cpp:966`, `2D-Portfolio/Client/Stage.cpp:975`, `2D-Portfolio/Client/Stage.cpp:984`, `2D-Portfolio/Client/Stage.cpp:993`, `2D-Portfolio/Client/Stage.cpp:1002`, `2D-Portfolio/Client/Stage.cpp:1011`, `2D-Portfolio/Client/Stage.cpp:1024`, `2D-Portfolio/Client/Stage.cpp:1033`, `2D-Portfolio/Client/Stage.cpp:1042`, `2D-Portfolio/Client/Stage.cpp:1051`, `2D-Portfolio/Client/Stage.cpp:1060`, `2D-Portfolio/Client/Stage.cpp:1069`, `2D-Portfolio/Client/Stage.cpp:1078`, `2D-Portfolio/Client/Stage.cpp:1087`, `2D-Portfolio/Client/Stage.cpp:1101`, `2D-Portfolio/Client/Stage.cpp:1110`, `2D-Portfolio/Client/Stage.cpp:1119`, `2D-Portfolio/Client/Stage.cpp:1128`, `2D-Portfolio/Client/Stage.cpp:1137`, `2D-Portfolio/Client/Stage.cpp:1146`, `2D-Portfolio/Client/Stage.cpp:1155`, `2D-Portfolio/Client/Stage.cpp:1164`, `2D-Portfolio/Client/Stage.cpp:1177`, `2D-Portfolio/Client/Stage.cpp:1186`, `2D-Portfolio/Client/Stage.cpp:1195`, `2D-Portfolio/Client/Stage.cpp:1204`, `2D-Portfolio/Client/Stage.cpp:1213`, `2D-Portfolio/Client/Stage.cpp:1222`, `2D-Portfolio/Client/Stage.cpp:1231`, `2D-Portfolio/Client/Stage.cpp:1240`, `2D-Portfolio/Client/Stage.cpp:1254`, `2D-Portfolio/Client/Stage.cpp:1263`, `2D-Portfolio/Client/Stage.cpp:1272`, `2D-Portfolio/Client/Stage.cpp:1281`, `2D-Portfolio/Client/Stage.cpp:1290`, `2D-Portfolio/Client/Stage.cpp:1299`, `2D-Portfolio/Client/Stage.cpp:1308`, `2D-Portfolio/Client/Stage.cpp:1317`, `2D-Portfolio/Client/Stage.cpp:1331`, `2D-Portfolio/Client/Stage.cpp:1340`, `2D-Portfolio/Client/Stage.cpp:1349`, `2D-Portfolio/Client/Stage.cpp:1358`, `2D-Portfolio/Client/Stage.cpp:1367`, `2D-Portfolio/Client/Stage.cpp:1376`, `2D-Portfolio/Client/Stage.cpp:1385`, `2D-Portfolio/Client/Stage.cpp:1394`, `2D-Portfolio/Client/Stage.cpp:1410`, `2D-Portfolio/Client/Stage.cpp:1419`, `2D-Portfolio/Client/Stage.cpp:1428`, `2D-Portfolio/Client/Stage.cpp:1437`, `2D-Portfolio/Client/Stage.cpp:1446`, `2D-Portfolio/Client/Stage.cpp:1455`, `2D-Portfolio/Client/Stage.cpp:1464`, `2D-Portfolio/Client/Stage.cpp:1473`, `2D-Portfolio/Client/Stage.cpp:1542`, `2D-Portfolio/Client/Stage.cpp:1551`, `2D-Portfolio/Client/Stage.cpp:1560`, `2D-Portfolio/Client/Stage.cpp:1569`, `2D-Portfolio/Client/Stage.cpp:1578`, `2D-Portfolio/Client/Stage.cpp:1587`, `2D-Portfolio/Client/Stage.cpp:1596`, `2D-Portfolio/Client/Stage.cpp:1605`, `2D-Portfolio/Client/Stage.cpp:1619`, `2D-Portfolio/Client/Stage.cpp:1632`, `2D-Portfolio/Client/UIObj.cpp:344`, `2D-Portfolio/Client/UIObj.cpp:345`, `2D-Portfolio/Client/UIObj.cpp:346`, `2D-Portfolio/Client/UIObj.cpp:347`, `2D-Portfolio/Client/UIObj.cpp:358`, `2D-Portfolio/Client/UIObj.cpp:359`, `2D-Portfolio/Client/UIObj.cpp:360`, `2D-Portfolio/Client/UIObj.cpp:361`, `2D-Portfolio/Client/UIObj.cpp:364`, `2D-Portfolio/Client/UIObj.cpp:374`, `2D-Portfolio/Client/UIObj.cpp:375`, `2D-Portfolio/Client/UIObj.cpp:376`, `2D-Portfolio/Client/UIObj.cpp:377`, `2D-Portfolio/Client/UIObj.cpp:380`, `2D-Portfolio/Client/UIObj.cpp:524`, `2D-Portfolio/Client/UIObj.cpp:525`, `2D-Portfolio/Client/UIObj.cpp:526`, `2D-Portfolio/Client/UIObj.cpp:527`, `2D-Portfolio/MFC_000/MFC_000View.cpp:177`, `2D-Portfolio/MFC_000/MFC_000View.cpp:234`, `2D-Portfolio/MFC_000/MFC_000View.cpp:276`, `2D-Portfolio/MFC_000/MFC_000View.cpp:277`, `2D-Portfolio/MFC_000/MFC_000View.cpp:281`, `2D-Portfolio/MFC_000/MFC_000View.cpp:282`, `2D-Portfolio/MFC_000/MFC_000View.cpp:380`, `2D-Portfolio/MFC_000/MFC_000View.cpp:382`, `2D-Portfolio/MFC_000/MFC_000View.cpp:531`, `2D-Portfolio/MFC_000/MFC_000View.cpp:533`, `2D-Portfolio/MFC_000/MiniView.cpp:121`, `2D-Portfolio/MFC_000/Page2.cpp:197` |
| I-56 `C4715` (1) | `2D-Portfolio/Client/UIObj.cpp:536` |
| I-56 `C4838` (17) | `2D-Portfolio/Client/FullModeMgr.cpp:61`, `2D-Portfolio/Client/UIObj.cpp:344`, `2D-Portfolio/Client/UIObj.cpp:345`, `2D-Portfolio/Client/UIObj.cpp:346`, `2D-Portfolio/Client/UIObj.cpp:347`, `2D-Portfolio/Client/UIObj.cpp:358`, `2D-Portfolio/Client/UIObj.cpp:359`, `2D-Portfolio/Client/UIObj.cpp:360`, `2D-Portfolio/Client/UIObj.cpp:361`, `2D-Portfolio/Client/UIObj.cpp:374`, `2D-Portfolio/Client/UIObj.cpp:375`, `2D-Portfolio/Client/UIObj.cpp:376`, `2D-Portfolio/Client/UIObj.cpp:377`, `2D-Portfolio/Client/UIObj.cpp:524`, `2D-Portfolio/Client/UIObj.cpp:525`, `2D-Portfolio/Client/UIObj.cpp:526`, `2D-Portfolio/Client/UIObj.cpp:527` |
| I-57 (15) | `2D-Portfolio/Client/Monster.cpp:154`, `2D-Portfolio/Client/Monster.cpp:182`, `2D-Portfolio/Client/Player.cpp:178`, `2D-Portfolio/Client/Player.cpp:190`, `2D-Portfolio/Client/Player.cpp:208`, `2D-Portfolio/Client/UIObj.cpp:309`, `2D-Portfolio/Client/UIObj.cpp:610`, `2D-Portfolio/Client/UIObj.cpp:646`, `2D-Portfolio/Client/UIObj.cpp:682`, `2D-Portfolio/Client/UIObj.cpp:720`, `2D-Portfolio/Client/UIObj.cpp:749`, `2D-Portfolio/Client/UIObj.cpp:778`, `2D-Portfolio/Headers/Debug.hpp:70`, `2D-Portfolio/MFC_000/MFC_000View.cpp:82`, `2D-Portfolio/MFC_000/MFC_000View.cpp:113` |
| II-4 (271) | `2D-Portfolio/Client/AStar.cpp:18`, `2D-Portfolio/Client/AStar.cpp:24`, `2D-Portfolio/Client/AStar.cpp:45`, `2D-Portfolio/Client/AStar.cpp:244`, `2D-Portfolio/Client/AStar.cpp:258`, `2D-Portfolio/Client/AStar.cpp:266`, `2D-Portfolio/Client/Ball.cpp:40`, `2D-Portfolio/Client/Ball.cpp:66`, `2D-Portfolio/Client/Blizzard.cpp:72`, `2D-Portfolio/Client/Blizzard.cpp:77`, `2D-Portfolio/Client/Blizzard.cpp:80`, `2D-Portfolio/Client/Blizzard.cpp:83`, `2D-Portfolio/Client/Blizzard.cpp:108`, `2D-Portfolio/Client/Blizzard.cpp:159`, `2D-Portfolio/Client/BoneSpear.cpp:74`, `2D-Portfolio/Client/BoneSpear.cpp:79`, `2D-Portfolio/Client/BoneSpear.cpp:82`, `2D-Portfolio/Client/BoneSpear.cpp:85`, `2D-Portfolio/Client/BoneSpear.cpp:106`, `2D-Portfolio/Client/BoneSpear.cpp:126`, `2D-Portfolio/Client/BoneSpear.cpp:146`, `2D-Portfolio/Client/Client.cpp:67`, `2D-Portfolio/Client/Client.cpp:81`, `2D-Portfolio/Client/Client.cpp:210`, `2D-Portfolio/Client/Client.cpp:212`, `2D-Portfolio/Client/Client.cpp:214`, `2D-Portfolio/Client/CollisionMgr.cpp:35`, `2D-Portfolio/Client/CollisionMgr.cpp:50`, `2D-Portfolio/Client/CollisionMgr.cpp:56`, `2D-Portfolio/Client/CollisionMgr.cpp:59`, `2D-Portfolio/Client/CollisionMgr.cpp:91`, `2D-Portfolio/Client/CollisionMgr.cpp:95`, `2D-Portfolio/Client/Device.cpp:68`, `2D-Portfolio/Client/FireWall.cpp:71`, `2D-Portfolio/Client/FireWall.cpp:76`, `2D-Portfolio/Client/FireWall.cpp:78`, `2D-Portfolio/Client/FireWall.cpp:81`, `2D-Portfolio/Client/FireWall.cpp:106`, `2D-Portfolio/Client/FullModeMgr.h:30`, `2D-Portfolio/Client/GameSound.hpp:55`, `2D-Portfolio/Client/GameSound.hpp:58`, `2D-Portfolio/Client/GameSound.hpp:59`, `2D-Portfolio/Client/GameSound.hpp:60`, `2D-Portfolio/Client/GameSound.hpp:63`, `2D-Portfolio/Client/GameSound.hpp:64`, `2D-Portfolio/Client/GameSound.hpp:65`, `2D-Portfolio/Client/GameSound.hpp:69`, `2D-Portfolio/Client/GameSound.hpp:71`, `2D-Portfolio/Client/GameSound.hpp:79`, `2D-Portfolio/Client/GameSound.hpp:80`, `2D-Portfolio/Client/GameSound.hpp:81`, `2D-Portfolio/Client/GameSound.hpp:97`, `2D-Portfolio/Client/GameSound.hpp:113`, `2D-Portfolio/Client/GameSound.hpp:122`, `2D-Portfolio/Client/GameSound.hpp:132`, `2D-Portfolio/Client/KeyMgr.cpp:17`, `2D-Portfolio/Client/KeyMgr.cpp:19`, `2D-Portfolio/Client/KeyMgr.cpp:21`, `2D-Portfolio/Client/KeyMgr.cpp:23`, `2D-Portfolio/Client/KeyMgr.cpp:25`, `2D-Portfolio/Client/KeyMgr.cpp:27`, `2D-Portfolio/Client/KeyMgr.cpp:29`, `2D-Portfolio/Client/KeyMgr.cpp:31`, `2D-Portfolio/Client/KeyMgr.cpp:33`, `2D-Portfolio/Client/KeyMgr.cpp:35`, `2D-Portfolio/Client/KeyMgr.cpp:37`, `2D-Portfolio/Client/KeyMgr.cpp:39`, `2D-Portfolio/Client/KeyMgr.cpp:41`, `2D-Portfolio/Client/KeyMgr.cpp:43`, `2D-Portfolio/Client/KeyMgr.cpp:45`, `2D-Portfolio/Client/KeyMgr.cpp:47`, `2D-Portfolio/Client/KeyMgr.cpp:49`, `2D-Portfolio/Client/KeyMgr.cpp:51`, `2D-Portfolio/Client/KeyMgr.cpp:53`, `2D-Portfolio/Client/KeyMgr.cpp:55`, `2D-Portfolio/Client/Logo.cpp:68`, `2D-Portfolio/Client/Logo.cpp:71`, `2D-Portfolio/Client/LogoBackGround.cpp:45`, `2D-Portfolio/Client/MainGame.cpp:51`, `2D-Portfolio/Client/MenuBackGround.cpp:45`, `2D-Portfolio/Client/MenuButton.cpp:54`, `2D-Portfolio/Client/MenuButton.cpp:85`, `2D-Portfolio/Client/Monster.cpp:37`, `2D-Portfolio/Client/Monster.cpp:39`, `2D-Portfolio/Client/Monster.cpp:87`, `2D-Portfolio/Client/Monster.cpp:103`, `2D-Portfolio/Client/Monster.cpp:107`, `2D-Portfolio/Client/Monster.cpp:142`, `2D-Portfolio/Client/Monster.cpp:157`, `2D-Portfolio/Client/Monster.cpp:185`, `2D-Portfolio/Client/Monster.cpp:239`, `2D-Portfolio/Client/Monster.cpp:325`, `2D-Portfolio/Client/Monster.cpp:373`, `2D-Portfolio/Client/Monster.cpp:382`, `2D-Portfolio/Client/Monster.cpp:416`, `2D-Portfolio/Client/Monster.cpp:419`, `2D-Portfolio/Client/Monster.cpp:454`, `2D-Portfolio/Client/Monster.cpp:457`, `2D-Portfolio/Client/Monster.cpp:474`, `2D-Portfolio/Client/Monster.cpp:481`, `2D-Portfolio/Client/Monster.cpp:483`, `2D-Portfolio/Client/Monster.cpp:503`, `2D-Portfolio/Client/Monster.cpp:505`, `2D-Portfolio/Client/Monster.cpp:507`, `2D-Portfolio/Client/Monster.cpp:509`, `2D-Portfolio/Client/Monster.cpp:514`, `2D-Portfolio/Client/Monster.cpp:516`, `2D-Portfolio/Client/Monster.cpp:518`, `2D-Portfolio/Client/Monster.cpp:520`, `2D-Portfolio/Client/Monster.cpp:522`, `2D-Portfolio/Client/Monster.cpp:820`, `2D-Portfolio/Client/MultiTexture.cpp:15`, `2D-Portfolio/Client/Obj.cpp:68`, `2D-Portfolio/Client/Obj.cpp:72`, `2D-Portfolio/Client/Obj.h:84`, `2D-Portfolio/Client/Obj.h:98`, `2D-Portfolio/Client/ObjMgr.cpp:31`, `2D-Portfolio/Client/ObjMgr.cpp:35`, `2D-Portfolio/Client/ObjMgr.cpp:54`, `2D-Portfolio/Client/ObjMgr.cpp:85`, `2D-Portfolio/Client/ObjMgr.cpp:98`, `2D-Portfolio/Client/ObjMgr.cpp:118`, `2D-Portfolio/Client/ObjMgr.cpp:197`, `2D-Portfolio/Client/ObjMgr.cpp:217`, `2D-Portfolio/Client/ObjMgr.cpp:233`, `2D-Portfolio/Client/ObjMgr.cpp:281`, `2D-Portfolio/Client/ObjMgr.cpp:300`, `2D-Portfolio/Client/ObjMgr.cpp:306`, `2D-Portfolio/Client/ObjMgr.cpp:312`, `2D-Portfolio/Client/ObjMgr.cpp:316`, `2D-Portfolio/Client/ObjMgr.cpp:327`, `2D-Portfolio/Client/ObjMgr.cpp:350`, `2D-Portfolio/Client/ObjMgr.cpp:353`, `2D-Portfolio/Client/ObjMgr.cpp:378`, `2D-Portfolio/Client/ObjMgr.cpp:397`, `2D-Portfolio/Client/ObjMgr.cpp:402`, `2D-Portfolio/Client/ObjMgr.cpp:407`, `2D-Portfolio/Client/ObjMgr.cpp:412`, `2D-Portfolio/Client/ObjMgr.cpp:463`, `2D-Portfolio/Client/ObjMgr.cpp:498`, `2D-Portfolio/Client/ObjMgr.cpp:504`, `2D-Portfolio/Client/ObjMgr.cpp:507`, `2D-Portfolio/Client/ObjMgr.cpp:511`, `2D-Portfolio/Client/ObjMgr.cpp:537`, `2D-Portfolio/Client/ObjMgr.cpp:545`, `2D-Portfolio/Client/ObjMgr.cpp:549`, `2D-Portfolio/Client/ObjMgr.cpp:599`, `2D-Portfolio/Client/ObjMgr.cpp:627`, `2D-Portfolio/Client/ObjMgr.cpp:631`, `2D-Portfolio/Client/ObjMgr.cpp:643`, `2D-Portfolio/Client/ObjMgr.cpp:647`, `2D-Portfolio/Client/ObjMgr.cpp:650`, `2D-Portfolio/Client/ObjMgr.cpp:654`, `2D-Portfolio/Client/Player.cpp:122`, `2D-Portfolio/Client/Player.cpp:139`, `2D-Portfolio/Client/Player.cpp:141`, `2D-Portfolio/Client/Player.cpp:145`, `2D-Portfolio/Client/Player.cpp:168`, `2D-Portfolio/Client/Player.cpp:181`, `2D-Portfolio/Client/Player.cpp:193`, `2D-Portfolio/Client/Player.cpp:211`, `2D-Portfolio/Client/Player.cpp:332`, `2D-Portfolio/Client/Player.cpp:334`, `2D-Portfolio/Client/Player.cpp:348`, `2D-Portfolio/Client/Player.cpp:350`, `2D-Portfolio/Client/Player.cpp:380`, `2D-Portfolio/Client/Player.cpp:383`, `2D-Portfolio/Client/Player.cpp:395`, `2D-Portfolio/Client/Player.cpp:402`, `2D-Portfolio/Client/Player.cpp:404`, `2D-Portfolio/Client/Player.cpp:406`, `2D-Portfolio/Client/Player.cpp:408`, `2D-Portfolio/Client/Player.cpp:410`, `2D-Portfolio/Client/Player.cpp:699`, `2D-Portfolio/Client/Player.cpp:701`, `2D-Portfolio/Client/Player.cpp:703`, `2D-Portfolio/Client/Player.cpp:705`, `2D-Portfolio/Client/Player.cpp:711`, `2D-Portfolio/Client/Player.cpp:713`, `2D-Portfolio/Client/Player.cpp:715`, `2D-Portfolio/Client/Player.cpp:717`, `2D-Portfolio/Client/Player.cpp:723`, `2D-Portfolio/Client/Player.cpp:725`, `2D-Portfolio/Client/Player.cpp:727`, `2D-Portfolio/Client/Player.cpp:729`, `2D-Portfolio/Client/Player.cpp:747`, `2D-Portfolio/Client/Player.cpp:754`, `2D-Portfolio/Client/Player.cpp:756`, `2D-Portfolio/Client/Player.cpp:761`, `2D-Portfolio/Client/Player.cpp:764`, `2D-Portfolio/Client/Player.cpp:796`, `2D-Portfolio/Client/Player.cpp:798`, `2D-Portfolio/Client/Player.cpp:800`, `2D-Portfolio/Client/Player.cpp:802`, `2D-Portfolio/Client/Player.cpp:850`, `2D-Portfolio/Client/Player.cpp:852`, `2D-Portfolio/Client/Player.cpp:858`, `2D-Portfolio/Client/Player.cpp:860`, `2D-Portfolio/Client/Player.cpp:862`, `2D-Portfolio/Client/SceneMgr.cpp:61`, `2D-Portfolio/Client/SingleTexture.cpp:19`, `2D-Portfolio/Client/SortMgr.cpp:17`, `2D-Portfolio/Client/Stage.cpp:1768`, `2D-Portfolio/Client/Stage.cpp:1771`, `2D-Portfolio/Client/Stage2.cpp:58`, `2D-Portfolio/Client/Stage3.cpp:43`, `2D-Portfolio/Client/Stage3.cpp:46`, `2D-Portfolio/Client/StageBackground.cpp:76`, `2D-Portfolio/Client/StageDynamic.cpp:28`, `2D-Portfolio/Client/StageDynamic.cpp:36`, `2D-Portfolio/Client/StageDynamic.cpp:40`, `2D-Portfolio/Client/StageDynamic.cpp:52`, `2D-Portfolio/Client/StageDynamic.cpp:67`, `2D-Portfolio/Client/TextureMgr.cpp:22`, `2D-Portfolio/Client/TextureMgr.cpp:37`, `2D-Portfolio/Client/TextureMgr.cpp:39`, `2D-Portfolio/Client/UIObj.cpp:290`, `2D-Portfolio/Client/UIObj.cpp:292`, `2D-Portfolio/Client/UIObj.cpp:296`, `2D-Portfolio/Client/UIObj.cpp:314`, `2D-Portfolio/Client/UIObj.cpp:366`, `2D-Portfolio/Client/UIObj.cpp:382`, `2D-Portfolio/Client/UIObj.cpp:501`, `2D-Portfolio/Client/UIObj.cpp:503`, `2D-Portfolio/Client/UIObj.cpp:509`, `2D-Portfolio/Client/UIObj.cpp:512`, `2D-Portfolio/Client/UIObj.cpp:515`, `2D-Portfolio/Headers/Debug.hpp:64`, `2D-Portfolio/Headers/Debug.hpp:79`, `2D-Portfolio/Headers/Debug.hpp:82`, `2D-Portfolio/Headers/Debug.hpp:110`, `2D-Portfolio/Headers/MathDef.hpp:57`, `2D-Portfolio/Headers/MathDef.hpp:86`, `2D-Portfolio/MFC_000/Device.cpp:68`, `2D-Portfolio/MFC_000/MFC_000.cpp:119`, `2D-Portfolio/MFC_000/MFC_000.cpp:135`, `2D-Portfolio/MFC_000/MFC_000Doc.cpp:42`, `2D-Portfolio/MFC_000/MFC_000View.cpp:67`, `2D-Portfolio/MFC_000/MFC_000View.cpp:88`, `2D-Portfolio/MFC_000/MFC_000View.cpp:94`, `2D-Portfolio/MFC_000/MFC_000View.cpp:119`, `2D-Portfolio/MFC_000/MFC_000View.cpp:125`, `2D-Portfolio/MFC_000/MFC_000View.cpp:155`, `2D-Portfolio/MFC_000/MFC_000View.cpp:158`, `2D-Portfolio/MFC_000/MFC_000View.cpp:164`, `2D-Portfolio/MFC_000/MFC_000View.cpp:192`, `2D-Portfolio/MFC_000/MFC_000View.cpp:195`, `2D-Portfolio/MFC_000/MFC_000View.cpp:201`, `2D-Portfolio/MFC_000/MFC_000View.cpp:237`, `2D-Portfolio/MFC_000/MFC_000View.cpp:241`, `2D-Portfolio/MFC_000/MFC_000View.cpp:248`, `2D-Portfolio/MFC_000/MFC_000View.cpp:253`, `2D-Portfolio/MFC_000/MFC_000View.cpp:258`, `2D-Portfolio/MFC_000/MFC_000View.cpp:410`, `2D-Portfolio/MFC_000/MFC_000View.cpp:414`, `2D-Portfolio/MFC_000/MainFrm.cpp:55`, `2D-Portfolio/MFC_000/MainFrm.cpp:158`, `2D-Portfolio/MFC_000/MiniView.cpp:58`, `2D-Portfolio/MFC_000/MiniView.cpp:63`, `2D-Portfolio/MFC_000/MiniView.cpp:92`, `2D-Portfolio/MFC_000/MiniView.cpp:95`, `2D-Portfolio/MFC_000/MiniView.cpp:100`, `2D-Portfolio/MFC_000/Page2.cpp:49`, `2D-Portfolio/MFC_000/Page2.cpp:77`, `2D-Portfolio/MFC_000/Page2.cpp:126`, `2D-Portfolio/MFC_000/Page2.cpp:142`, `2D-Portfolio/MFC_000/Page2.cpp:157`, `2D-Portfolio/MFC_000/TextureMgr.cpp:21`, `2D-Portfolio/MFC_000/TextureMgr.cpp:33`, `2D-Portfolio/MFC_000/TextureMgr.cpp:35`, `2D-Portfolio/MFC_000/TextureMgr.cpp:57` |
| III-4 (175) | `2D-Portfolio/Client/AStar.cpp:39`, `2D-Portfolio/Client/AStar.cpp:45`, `2D-Portfolio/Client/AStar.cpp:48`, `2D-Portfolio/Client/Ball.cpp:22`, `2D-Portfolio/Client/Ball.cpp:66`, `2D-Portfolio/Client/Ball.cpp:75`, `2D-Portfolio/Client/Blizzard.cpp:108`, `2D-Portfolio/Client/Blizzard.cpp:125`, `2D-Portfolio/Client/Blizzard.cpp:133`, `2D-Portfolio/Client/Blizzard.cpp:141`, `2D-Portfolio/Client/Blizzard.cpp:149`, `2D-Portfolio/Client/Blizzard.cpp:157`, `2D-Portfolio/Client/BoneSpear.cpp:146`, `2D-Portfolio/Client/BoneSpear.cpp:154`, `2D-Portfolio/Client/Client.cpp:65`, `2D-Portfolio/Client/Client.cpp:119`, `2D-Portfolio/Client/Client.cpp:148`, `2D-Portfolio/Client/Device.cpp:75`, `2D-Portfolio/Client/Device.cpp:78`, `2D-Portfolio/Client/Device.cpp:86`, `2D-Portfolio/Client/Device.cpp:98`, `2D-Portfolio/Client/Device.cpp:109`, `2D-Portfolio/Client/FireWall.cpp:106`, `2D-Portfolio/Client/FireWall.cpp:114`, `2D-Portfolio/Client/LogoBackGround.cpp:24`, `2D-Portfolio/Client/LogoBackGround.cpp:45`, `2D-Portfolio/Client/LogoBackGround.cpp:52`, `2D-Portfolio/Client/MenuBackGround.cpp:24`, `2D-Portfolio/Client/MenuBackGround.cpp:45`, `2D-Portfolio/Client/MenuBackGround.cpp:52`, `2D-Portfolio/Client/MenuButton.cpp:25`, `2D-Portfolio/Client/MenuButton.cpp:85`, `2D-Portfolio/Client/MenuButton.cpp:102`, `2D-Portfolio/Client/Monster.cpp:142`, `2D-Portfolio/Client/Monster.cpp:150`, `2D-Portfolio/Client/Monster.cpp:157`, `2D-Portfolio/Client/Monster.cpp:165`, `2D-Portfolio/Client/Monster.cpp:176`, `2D-Portfolio/Client/Monster.cpp:185`, `2D-Portfolio/Client/Monster.cpp:193`, `2D-Portfolio/Client/Monster.cpp:239`, `2D-Portfolio/Client/Monster.cpp:376`, `2D-Portfolio/Client/Monster.cpp:385`, `2D-Portfolio/Client/MultiTexture.cpp:16`, `2D-Portfolio/Client/MultiTexture.cpp:48`, `2D-Portfolio/Client/MultiTexture.h:13`, `2D-Portfolio/Client/MultiTexture.h:16`, `2D-Portfolio/Client/Obj.cpp:6`, `2D-Portfolio/Client/Obj.cpp:7`, `2D-Portfolio/Client/Obj.cpp:18`, `2D-Portfolio/Client/Obj.cpp:19`, `2D-Portfolio/Client/Obj.cpp:20`, `2D-Portfolio/Client/Obj.cpp:38`, `2D-Portfolio/Client/Obj.cpp:39`, `2D-Portfolio/Client/Obj.cpp:40`, `2D-Portfolio/Client/Obj.h:98`, `2D-Portfolio/Client/ObjMgr.cpp:31`, `2D-Portfolio/Client/ObjMgr.cpp:600`, `2D-Portfolio/Client/ObjProto.cpp:21`, `2D-Portfolio/Client/Player.cpp:14`, `2D-Portfolio/Client/Player.cpp:89`, `2D-Portfolio/Client/Player.cpp:168`, `2D-Portfolio/Client/Player.cpp:181`, `2D-Portfolio/Client/Player.cpp:186`, `2D-Portfolio/Client/Player.cpp:193`, `2D-Portfolio/Client/Player.cpp:198`, `2D-Portfolio/Client/Player.cpp:204`, `2D-Portfolio/Client/Player.cpp:211`, `2D-Portfolio/Client/Player.cpp:216`, `2D-Portfolio/Client/SceneMgr.cpp:6`, `2D-Portfolio/Client/SingleTexture.cpp:6`, `2D-Portfolio/Client/SingleTexture.cpp:19`, `2D-Portfolio/Client/SingleTexture.cpp:35`, `2D-Portfolio/Client/SingleTexture.h:12`, `2D-Portfolio/Client/SingleTexture.h:15`, `2D-Portfolio/Client/SortMgr.cpp:17`, `2D-Portfolio/Client/StageBackground.cpp:25`, `2D-Portfolio/Client/StageBackground.cpp:26`, `2D-Portfolio/Client/StageBackground.cpp:31`, `2D-Portfolio/Client/StageBackground.cpp:76`, `2D-Portfolio/Client/StageBackground.cpp:88`, `2D-Portfolio/Client/Texture.h:9`, `2D-Portfolio/Client/Texture.h:12`, `2D-Portfolio/Client/TextureMgr.cpp:37`, `2D-Portfolio/Client/TextureMgr.cpp:63`, `2D-Portfolio/Client/TextureMgr.h:14`, `2D-Portfolio/Client/TextureMgr.h:18`, `2D-Portfolio/Client/UIObj.cpp:176`, `2D-Portfolio/Client/UIObj.cpp:310`, `2D-Portfolio/Client/UIObj.cpp:314`, `2D-Portfolio/Client/UIObj.cpp:339`, `2D-Portfolio/Client/UIObj.cpp:353`, `2D-Portfolio/Client/UIObj.cpp:368`, `2D-Portfolio/Client/UIObj.cpp:384`, `2D-Portfolio/Client/UIObj.cpp:395`, `2D-Portfolio/Client/UIObj.cpp:407`, `2D-Portfolio/Client/UIObj.cpp:419`, `2D-Portfolio/Client/UIObj.cpp:430`, `2D-Portfolio/Client/UIObj.cpp:444`, `2D-Portfolio/Client/UIObj.cpp:458`, `2D-Portfolio/Client/UIObj.cpp:472`, `2D-Portfolio/Client/UIObj.cpp:484`, `2D-Portfolio/Client/UIObj.cpp:605`, `2D-Portfolio/Client/UIObj.cpp:617`, `2D-Portfolio/Client/UIObj.cpp:641`, `2D-Portfolio/Client/UIObj.cpp:653`, `2D-Portfolio/Client/UIObj.cpp:677`, `2D-Portfolio/Client/UIObj.cpp:689`, `2D-Portfolio/Client/UIObj.cpp:715`, `2D-Portfolio/Client/UIObj.cpp:727`, `2D-Portfolio/Client/UIObj.cpp:744`, `2D-Portfolio/Client/UIObj.cpp:756`, `2D-Portfolio/Client/UIObj.cpp:773`, `2D-Portfolio/Client/UIObj.cpp:785`, `2D-Portfolio/Headers/Macro.h:7`, `2D-Portfolio/MFC_000/Device.cpp:7`, `2D-Portfolio/MFC_000/Device.cpp:76`, `2D-Portfolio/MFC_000/Device.cpp:87`, `2D-Portfolio/MFC_000/MFC_000Doc.cpp:110`, `2D-Portfolio/MFC_000/MFC_000Doc.cpp:112`, `2D-Portfolio/MFC_000/MFC_000View.cpp:94`, `2D-Portfolio/MFC_000/MFC_000View.cpp:103`, `2D-Portfolio/MFC_000/MFC_000View.cpp:125`, `2D-Portfolio/MFC_000/MFC_000View.cpp:137`, `2D-Portfolio/MFC_000/MFC_000View.cpp:164`, `2D-Portfolio/MFC_000/MFC_000View.cpp:177`, `2D-Portfolio/MFC_000/MFC_000View.cpp:201`, `2D-Portfolio/MFC_000/MFC_000View.cpp:216`, `2D-Portfolio/MFC_000/MFC_000View.cpp:280`, `2D-Portfolio/MFC_000/MFC_000View.cpp:305`, `2D-Portfolio/MFC_000/MFC_000View.cpp:393`, `2D-Portfolio/MFC_000/MFC_000View.cpp:423`, `2D-Portfolio/MFC_000/MFC_000View.cpp:428`, `2D-Portfolio/MFC_000/MFC_000View.cpp:504`, `2D-Portfolio/MFC_000/MFC_000View.cpp:509`, `2D-Portfolio/MFC_000/MFC_000View.cpp:546`, `2D-Portfolio/MFC_000/MFC_000View.cpp:549`, `2D-Portfolio/MFC_000/MainFrm.cpp:91`, `2D-Portfolio/MFC_000/MainFrm.cpp:119`, `2D-Portfolio/MFC_000/MainFrm.cpp:275`, `2D-Portfolio/MFC_000/MainFrm.cpp:304`, `2D-Portfolio/MFC_000/MainFrm.h:28`, `2D-Portfolio/MFC_000/MiniView.cpp:63`, `2D-Portfolio/MFC_000/MiniView.cpp:75`, `2D-Portfolio/MFC_000/MiniView.cpp:100`, `2D-Portfolio/MFC_000/MiniView.cpp:119`, `2D-Portfolio/MFC_000/MiniView.cpp:121`, `2D-Portfolio/MFC_000/MultiTexture.cpp:37`, `2D-Portfolio/MFC_000/MyForm.cpp:70`, `2D-Portfolio/MFC_000/MySheet.h:17`, `2D-Portfolio/MFC_000/MySheet.h:18`, `2D-Portfolio/MFC_000/Page1.cpp:69`, `2D-Portfolio/MFC_000/Page2.cpp:56`, `2D-Portfolio/MFC_000/Page2.cpp:57`, `2D-Portfolio/MFC_000/Page2.cpp:93`, `2D-Portfolio/MFC_000/Page2.cpp:94`, `2D-Portfolio/MFC_000/Page2.cpp:100`, `2D-Portfolio/MFC_000/Page2.cpp:105`, `2D-Portfolio/MFC_000/Page2.cpp:133`, `2D-Portfolio/MFC_000/Page2.cpp:134`, `2D-Portfolio/MFC_000/Page2.cpp:142`, `2D-Portfolio/MFC_000/Page2.cpp:173`, `2D-Portfolio/MFC_000/Page2.cpp:174`, `2D-Portfolio/MFC_000/Page2.cpp:187`, `2D-Portfolio/MFC_000/Page2.cpp:192`, `2D-Portfolio/MFC_000/Page3.cpp:107`, `2D-Portfolio/MFC_000/Page3.cpp:142`, `2D-Portfolio/MFC_000/Page3.cpp:203`, `2D-Portfolio/MFC_000/Page3.cpp:206`, `2D-Portfolio/MFC_000/Page3.cpp:219`, `2D-Portfolio/MFC_000/SingleTexture.cpp:6`, `2D-Portfolio/MFC_000/SingleTexture.cpp:32`, `2D-Portfolio/MFC_000/TextureMgr.cpp:23`, `2D-Portfolio/MFC_000/TextureMgr.cpp:33`, `2D-Portfolio/MFC_000/TextureMgr.cpp:58` |
| III-1 (121) | `2D-Portfolio/Client/Ball.h:9`, `2D-Portfolio/Client/Ball.h:10`, `2D-Portfolio/Client/Ball.h:11`, `2D-Portfolio/Client/Ball.h:12`, `2D-Portfolio/Client/Blizzard.h:15`, `2D-Portfolio/Client/Blizzard.h:16`, `2D-Portfolio/Client/Blizzard.h:17`, `2D-Portfolio/Client/Blizzard.h:18`, `2D-Portfolio/Client/BoneSpear.h:15`, `2D-Portfolio/Client/BoneSpear.h:16`, `2D-Portfolio/Client/BoneSpear.h:17`, `2D-Portfolio/Client/BoneSpear.h:18`, `2D-Portfolio/Client/FireWall.h:15`, `2D-Portfolio/Client/FireWall.h:16`, `2D-Portfolio/Client/FireWall.h:17`, `2D-Portfolio/Client/FireWall.h:18`, `2D-Portfolio/Client/Logo.h:9`, `2D-Portfolio/Client/Logo.h:11`, `2D-Portfolio/Client/LogoBackGround.h:9`, `2D-Portfolio/Client/LogoBackGround.h:10`, `2D-Portfolio/Client/LogoBackGround.h:11`, `2D-Portfolio/Client/LogoBackGround.h:12`, `2D-Portfolio/Client/LogoDynamic.h:9`, `2D-Portfolio/Client/LogoDynamic.h:10`, `2D-Portfolio/Client/LogoDynamic.h:11`, `2D-Portfolio/Client/LogoDynamic.h:12`, `2D-Portfolio/Client/LogoStatic.h:9`, `2D-Portfolio/Client/LogoStatic.h:10`, `2D-Portfolio/Client/LogoStatic.h:11`, `2D-Portfolio/Client/LogoStatic.h:12`, `2D-Portfolio/Client/MenuBackGround.h:8`, `2D-Portfolio/Client/MenuBackGround.h:9`, `2D-Portfolio/Client/MenuBackGround.h:10`, `2D-Portfolio/Client/MenuBackGround.h:11`, `2D-Portfolio/Client/MenuButton.h:37`, `2D-Portfolio/Client/MenuButton.h:38`, `2D-Portfolio/Client/MenuButton.h:39`, `2D-Portfolio/Client/MenuButton.h:40`, `2D-Portfolio/Client/MenuD2.h:9`, `2D-Portfolio/Client/MenuD2.h:10`, `2D-Portfolio/Client/MenuDynamic.h:8`, `2D-Portfolio/Client/MenuDynamic.h:9`, `2D-Portfolio/Client/MenuDynamic.h:10`, `2D-Portfolio/Client/MenuDynamic.h:11`, `2D-Portfolio/Client/MenuStatic.h:8`, `2D-Portfolio/Client/MenuStatic.h:9`, `2D-Portfolio/Client/MenuStatic.h:10`, `2D-Portfolio/Client/MenuStatic.h:11`, `2D-Portfolio/Client/Monster.h:26`, `2D-Portfolio/Client/Monster.h:27`, `2D-Portfolio/Client/Monster.h:28`, `2D-Portfolio/Client/Monster.h:29`, `2D-Portfolio/Client/MultiTexture.h:17`, `2D-Portfolio/Client/ObjProto.h:14`, `2D-Portfolio/Client/Player.h:28`, `2D-Portfolio/Client/Player.h:29`, `2D-Portfolio/Client/Player.h:30`, `2D-Portfolio/Client/Player.h:31`, `2D-Portfolio/Client/SingleTexture.h:16`, `2D-Portfolio/Client/Skill.h:10`, `2D-Portfolio/Client/Skill.h:11`, `2D-Portfolio/Client/Skill.h:12`, `2D-Portfolio/Client/Skill.h:13`, `2D-Portfolio/Client/SkillTree.h:7`, `2D-Portfolio/Client/SkillTree.h:8`, `2D-Portfolio/Client/SkillTree.h:9`, `2D-Portfolio/Client/SkillTree.h:10`, `2D-Portfolio/Client/Stage.h:9`, `2D-Portfolio/Client/Stage.h:10`, `2D-Portfolio/Client/Stage.h:11`, `2D-Portfolio/Client/Stage2.h:8`, `2D-Portfolio/Client/Stage2.h:9`, `2D-Portfolio/Client/Stage2.h:10`, `2D-Portfolio/Client/Stage3.h:8`, `2D-Portfolio/Client/Stage3.h:9`, `2D-Portfolio/Client/Stage3.h:10`, `2D-Portfolio/Client/StageBackground.h:14`, `2D-Portfolio/Client/StageBackground.h:15`, `2D-Portfolio/Client/StageBackground.h:16`, `2D-Portfolio/Client/StageBackground.h:17`, `2D-Portfolio/Client/StageDynamic.h:14`, `2D-Portfolio/Client/StageDynamic.h:15`, `2D-Portfolio/Client/StageDynamic.h:17`, `2D-Portfolio/Client/StageStatic.h:10`, `2D-Portfolio/Client/StageStatic.h:11`, `2D-Portfolio/Client/StageStatic.h:12`, `2D-Portfolio/Client/StageStatic.h:13`, `2D-Portfolio/Client/UIObj.h:20`, `2D-Portfolio/Client/UIObj.h:21`, `2D-Portfolio/Client/UIObj.h:22`, `2D-Portfolio/Client/UIObj.h:23`, `2D-Portfolio/Client/UIObj.h:38`, `2D-Portfolio/MFC_000/MFC_000.h:26`, `2D-Portfolio/MFC_000/MFC_000.h:32`, `2D-Portfolio/MFC_000/MFC_000.h:33`, `2D-Portfolio/MFC_000/MFC_000.h:34`, `2D-Portfolio/MFC_000/MFC_000Doc.h:24`, `2D-Portfolio/MFC_000/MFC_000Doc.h:26`, `2D-Portfolio/MFC_000/MFC_000Doc.h:27`, `2D-Portfolio/MFC_000/MFC_000Doc.h:34`, `2D-Portfolio/MFC_000/MFC_000Doc.h:35`, `2D-Portfolio/MFC_000/MFC_000View.h:34`, `2D-Portfolio/MFC_000/MFC_000View.h:37`, `2D-Portfolio/MFC_000/MFC_000View.h:38`, `2D-Portfolio/MFC_000/MFC_000View.h:39`, `2D-Portfolio/MFC_000/MFC_000View.h:45`, `2D-Portfolio/MFC_000/MainFrm.h:28`, `2D-Portfolio/MFC_000/MainFrm.h:34`, `2D-Portfolio/MFC_000/MainFrm.h:35`, `2D-Portfolio/MFC_000/MainFrm.h:53`, `2D-Portfolio/MFC_000/MiniView.h:17`, `2D-Portfolio/MFC_000/MiniView.h:19`, `2D-Portfolio/MFC_000/MultiTexture.h:14`, `2D-Portfolio/MFC_000/MyForm.h:20`, `2D-Portfolio/MFC_000/MyForm.h:22`, `2D-Portfolio/MFC_000/MyForm.h:32`, `2D-Portfolio/MFC_000/Page1.h:26`, `2D-Portfolio/MFC_000/Page1.h:29`, `2D-Portfolio/MFC_000/Page3.h:28`, `2D-Portfolio/MFC_000/Page3.h:31`, `2D-Portfolio/MFC_000/SingleTexture.h:14` |
| III-10 (96) | `2D-Portfolio/Client/Value.h:2`, `2D-Portfolio/Client/Value.h:3`, `2D-Portfolio/Client/Value.h:4`, `2D-Portfolio/Client/Value.h:6`, `2D-Portfolio/Client/Value.h:7`, `2D-Portfolio/Client/Value.h:8`, `2D-Portfolio/Client/Value.h:9`, `2D-Portfolio/Client/Value.h:10`, `2D-Portfolio/Client/Value.h:11`, `2D-Portfolio/Client/Value.h:12`, `2D-Portfolio/Client/Value.h:13`, `2D-Portfolio/Client/Value.h:14`, `2D-Portfolio/Client/Value.h:15`, `2D-Portfolio/Client/Value.h:16`, `2D-Portfolio/Client/Value.h:17`, `2D-Portfolio/Client/Value.h:18`, `2D-Portfolio/Client/Value.h:19`, `2D-Portfolio/Client/Value.h:20`, `2D-Portfolio/Client/Value.h:21`, `2D-Portfolio/Client/Value.h:22`, `2D-Portfolio/Headers/Constant.h:3`, `2D-Portfolio/Headers/Constant.h:4`, `2D-Portfolio/Headers/Constant.h:5`, `2D-Portfolio/Headers/Constant.h:6`, `2D-Portfolio/Headers/Constant.h:7`, `2D-Portfolio/Headers/Constant.h:75`, `2D-Portfolio/Headers/Constant.h:76`, `2D-Portfolio/Headers/Constant.h:77`, `2D-Portfolio/Headers/Constant.h:78`, `2D-Portfolio/Headers/Constant.h:79`, `2D-Portfolio/Headers/Constant.h:80`, `2D-Portfolio/Headers/Constant.h:81`, `2D-Portfolio/Headers/Constant.h:82`, `2D-Portfolio/Headers/Constant.h:83`, `2D-Portfolio/Headers/Constant.h:84`, `2D-Portfolio/Headers/Constant.h:85`, `2D-Portfolio/Headers/Constant.h:86`, `2D-Portfolio/Headers/Constant.h:87`, `2D-Portfolio/Headers/Constant.h:88`, `2D-Portfolio/Headers/Constant.h:89`, `2D-Portfolio/Headers/Constant.h:90`, `2D-Portfolio/Headers/Constant.h:91`, `2D-Portfolio/Headers/Constant.h:92`, `2D-Portfolio/Headers/Constant.h:93`, `2D-Portfolio/Headers/Constant.h:94`, `2D-Portfolio/Headers/Constant.h:95`, `2D-Portfolio/Headers/Constant.h:96`, `2D-Portfolio/Headers/Constant.h:97`, `2D-Portfolio/Headers/Constant.h:98`, `2D-Portfolio/Headers/Constant.h:99`, `2D-Portfolio/Headers/Constant.h:100`, `2D-Portfolio/Headers/Constant.h:101`, `2D-Portfolio/Headers/Constant.h:102`, `2D-Portfolio/Headers/Constant.h:103`, `2D-Portfolio/Headers/Constant.h:104`, `2D-Portfolio/Headers/Constant.h:105`, `2D-Portfolio/Headers/Constant.h:106`, `2D-Portfolio/Headers/Constant.h:107`, `2D-Portfolio/Headers/Constant.h:108`, `2D-Portfolio/Headers/Constant.h:109`, `2D-Portfolio/Headers/Constant.h:110`, `2D-Portfolio/Headers/Constant.h:111`, `2D-Portfolio/Headers/Constant.h:112`, `2D-Portfolio/Headers/Constant.h:113`, `2D-Portfolio/Headers/Constant.h:114`, `2D-Portfolio/Headers/Constant.h:115`, `2D-Portfolio/Headers/Constant.h:116`, `2D-Portfolio/Headers/Constant.h:117`, `2D-Portfolio/Headers/Constant.h:118`, `2D-Portfolio/Headers/Constant.h:119`, `2D-Portfolio/Headers/Constant.h:120`, `2D-Portfolio/Headers/Constant.h:121`, `2D-Portfolio/Headers/Constant.h:122`, `2D-Portfolio/Headers/Constant.h:123`, `2D-Portfolio/Headers/Constant.h:129`, `2D-Portfolio/Headers/Constant.h:130`, `2D-Portfolio/Headers/Constant.h:134`, `2D-Portfolio/Headers/Constant.h:135`, `2D-Portfolio/Headers/Constant.h:136`, `2D-Portfolio/Headers/Constant.h:137`, `2D-Portfolio/Headers/Constant.h:138`, `2D-Portfolio/Headers/Constant.h:140`, `2D-Portfolio/Headers/Constant.h:141`, `2D-Portfolio/Headers/Constant.h:142`, `2D-Portfolio/Headers/Constant.h:143`, `2D-Portfolio/Headers/Constant.h:145`, `2D-Portfolio/Headers/Constant.h:146`, `2D-Portfolio/Headers/Constant.h:147`, `2D-Portfolio/Headers/Constant.h:148`, `2D-Portfolio/Headers/Constant.h:150`, `2D-Portfolio/Headers/Constant.h:151`, `2D-Portfolio/Headers/Constant.h:152`, `2D-Portfolio/Headers/Constant.h:153`, `2D-Portfolio/Headers/Constant.h:154`, `2D-Portfolio/MFC_000/MainFrm.cpp:21`, `2D-Portfolio/MFC_000/MainFrm.cpp:23` |
| I-26 (79) | `2D-Portfolio/Client/AStar.cpp:282`, `2D-Portfolio/Client/AStar.cpp:288`, `2D-Portfolio/Client/Blizzard.cpp:112`, `2D-Portfolio/Client/BoneSpear.cpp:99`, `2D-Portfolio/Client/CollisionMgr.cpp:20`, `2D-Portfolio/Client/CollisionMgr.cpp:25`, `2D-Portfolio/Client/CollisionMgr.cpp:29`, `2D-Portfolio/Client/CollisionMgr.cpp:32`, `2D-Portfolio/Client/Device.h:26`, `2D-Portfolio/Client/GameSound.hpp:57`, `2D-Portfolio/Client/GameSound.hpp:62`, `2D-Portfolio/Client/KeyMgr.cpp:59`, `2D-Portfolio/Client/MainGame.cpp:50`, `2D-Portfolio/Client/Monster.cpp:80`, `2D-Portfolio/Client/Monster.cpp:228`, `2D-Portfolio/Client/Monster.cpp:331`, `2D-Portfolio/Client/Monster.cpp:334`, `2D-Portfolio/Client/MultiTexture.cpp:11`, `2D-Portfolio/Client/MultiTexture.cpp:21`, `2D-Portfolio/Client/Obj.cpp:83`, `2D-Portfolio/Client/Obj.cpp:98`, `2D-Portfolio/Client/Obj.h:80`, `2D-Portfolio/Client/Obj.h:82`, `2D-Portfolio/Client/Obj.h:85`, `2D-Portfolio/Client/ObjMgr.cpp:424`, `2D-Portfolio/Client/ObjMgr.cpp:428`, `2D-Portfolio/Client/ObjMgr.cpp:450`, `2D-Portfolio/Client/ObjMgr.cpp:454`, `2D-Portfolio/Client/Player.cpp:115`, `2D-Portfolio/Client/SceneMgr.cpp:57`, `2D-Portfolio/Client/SingleTexture.cpp:12`, `2D-Portfolio/Client/SingleTexture.cpp:16`, `2D-Portfolio/Client/Stage.cpp:23`, `2D-Portfolio/Client/Stage.cpp:24`, `2D-Portfolio/Client/StageBackground.cpp:69`, `2D-Portfolio/Client/StageBackground.cpp:105`, `2D-Portfolio/Client/StageDynamic.cpp:57`, `2D-Portfolio/Client/StageDynamic.cpp:61`, `2D-Portfolio/Headers/MathDef.hpp:18`, `2D-Portfolio/Headers/MathDef.hpp:19`, `2D-Portfolio/Headers/MathDef.hpp:50`, `2D-Portfolio/Headers/MathDef.hpp:51`, `2D-Portfolio/Headers/MathDef.hpp:52`, `2D-Portfolio/Headers/MathDef.hpp:53`, `2D-Portfolio/Headers/MathDef.hpp:56`, `2D-Portfolio/Headers/Struct.h:58`, `2D-Portfolio/MFC_000/MFC_000View.cpp:83`, `2D-Portfolio/MFC_000/MFC_000View.cpp:114`, `2D-Portfolio/MFC_000/MFC_000View.cpp:142`, `2D-Portfolio/MFC_000/MFC_000View.cpp:150`, `2D-Portfolio/MFC_000/MFC_000View.cpp:187`, `2D-Portfolio/MFC_000/MFC_000View.cpp:276`, `2D-Portfolio/MFC_000/MFC_000View.cpp:277`, `2D-Portfolio/MFC_000/MFC_000View.cpp:294`, `2D-Portfolio/MFC_000/MFC_000View.cpp:383`, `2D-Portfolio/MFC_000/MFC_000View.cpp:457`, `2D-Portfolio/MFC_000/MFC_000View.cpp:534`, `2D-Portfolio/MFC_000/MainFrm.cpp:210`, `2D-Portfolio/MFC_000/MiniView.cpp:56`, `2D-Portfolio/MFC_000/MiniView.cpp:83`, `2D-Portfolio/MFC_000/MiniView.cpp:90`, `2D-Portfolio/MFC_000/MultiTexture.cpp:11`, `2D-Portfolio/MFC_000/MultiTexture.cpp:14`, `2D-Portfolio/MFC_000/Page1.cpp:83`, `2D-Portfolio/MFC_000/Page1.cpp:101`, `2D-Portfolio/MFC_000/Page1.cpp:121`, `2D-Portfolio/MFC_000/Page1.cpp:144`, `2D-Portfolio/MFC_000/Page1.cpp:160`, `2D-Portfolio/MFC_000/Page1.cpp:168`, `2D-Portfolio/MFC_000/Page2.cpp:60`, `2D-Portfolio/MFC_000/Page2.cpp:81`, `2D-Portfolio/MFC_000/Page2.cpp:139`, `2D-Portfolio/MFC_000/Page2.cpp:161`, `2D-Portfolio/MFC_000/Page2.cpp:182`, `2D-Portfolio/MFC_000/Page2.cpp:197`, `2D-Portfolio/MFC_000/Page3.cpp:164`, `2D-Portfolio/MFC_000/Page3.cpp:184`, `2D-Portfolio/MFC_000/SingleTexture.cpp:12`, `2D-Portfolio/MFC_000/SingleTexture.cpp:15` |
| I-27 (18) | `2D-Portfolio/Client/Device.h:26`, `2D-Portfolio/Client/Device.h:27`, `2D-Portfolio/Client/Device.h:31`, `2D-Portfolio/Client/MenuButton.h:11`, `2D-Portfolio/Client/MultiTexture.h:12`, `2D-Portfolio/Client/Obj.h:54`, `2D-Portfolio/Client/Obj.h:103`, `2D-Portfolio/Client/ObjMgr.h:15`, `2D-Portfolio/Client/ObjMgr.h:32`, `2D-Portfolio/Client/ObjProto.h:11`, `2D-Portfolio/Client/Player.h:25`, `2D-Portfolio/Client/SingleTexture.h:11`, `2D-Portfolio/Client/TextureMgr.h:13`, `2D-Portfolio/MFC_000/Device.h:17`, `2D-Portfolio/MFC_000/Device.h:19`, `2D-Portfolio/MFC_000/MultiTexture.h:11`, `2D-Portfolio/MFC_000/SingleTexture.h:11`, `2D-Portfolio/MFC_000/TextureMgr.h:13` |

## 미검사·제외

- **작업본 상태**: 검사 도중 작업본의 `Client/MenuD2.cpp`·`MenuD2.h`·`SceneMgr.cpp`·`SceneMgr.h`·`SceneObj.h` 가 바뀌었다. 스캔은 16:09 기준 파일로 다시 돌렸고, 바뀐 `.cpp` 2개는 다시 컴파일해 새 경고가 없음을 확인했다. 이후 수정분은 반영되지 않았다.
- **I-56**: `Debug|Win32` 전체 Rebuild(`.cpp` 67개 모두 컴파일 확인, 오류 0) 출력 기준. 경고 줄 282개 중 같은 줄의 열만 다른 것을 합쳐 279건. `Release|Win32` 는 빌드하지 않았다.
- **I-57**: `/W3` 에서는 가리기 경고가 꺼져 있어 `CL=/w14456 /w14457 /w14458 /w14459` 로 일시적으로 켜고 Rebuild 했다(프로젝트 파일은 바꾸지 않음). C4458(멤버 가리기) 0건. C4459 경고 66줄은 `Headers/Debug.hpp:63`·`:70` 두 위치가 Client 번역 단위마다 반복된 것이다.
- **I-26 (부분 검사)**: 정수·실수·`bool`·enum 스칼라의 지역 변수와 값 매개변수만 검사했다. 함수 호출 인수로 그대로 넘기는 변수는 비 `const` 참조로 바뀔 수 있어 제외했다. 포인터 자체의 `const`(`T* const`), 객체·참조 매개변수(`const T&`)는 호출 관계를 따라가야 해 **미검사**다.
- **I-27 (부분 검사)**: `Get`·`Is`·`Has`·`Can` 으로 시작하는 멤버 함수만 검사했다. 정의가 없는 순수 가상 선언(예: `CTexture::GetTexture`)은 미검사다. 가상 `GetTexture` 에 `const` 를 붙이려면 기반 `CTexture` 선언과 파생 클래스 전체를 함께 바꿔야 한다. 반복자를 쓰는 함수(`CTextureMgr::GetTexture` 등)는 `const_iterator` 로도 바꿔야 한다.
- **III-1**: 소멸자 재정의(`virtual ~CX()`)는 검사하지 않았다. `final` 은 어느 클래스·함수를 더 파생하지 않을지가 설계 판단이라 보고하지 않았다(판단 필요).
- **III-4**: `NULL` 은 모두 보고했다. 리터럴 `0` 은 포인터 이름(`p`·`m_p`·`g_p` 접두)에 쓴 것만 잡아, 그 밖의 이름(예: `Headers/Debug.hpp:63` 의 `hInst = 0`)은 빠졌을 수 있다.
- **III-10**: 리터럴·대문자 상수만으로 초기화한 `const` 만 보고했다. `MFC_000/MainFrm.cpp:23` 은 직접 확인해 추가했다.
- **CLAUDE.md 범위 밖으로 제외**: 명명(I-1~15, I-29, I-35~37, I-47), 설계·구조(I-16, I-17, I-30~32, I-34, I-42, I-43, I-45, I-46, I-48~52, I-55, I-59~61, III-2, III-5, III-7), 기반 코드 전제(I-24, I-25, I-38~41, I-44), 포맷팅 II-1~3·II-5·II-6, 목록에 없는 품질 항목(I-18~22, I-28, I-33, I-53, I-54, I-58, III-3, III-6, III-8), 프로젝트 설정 IV-1·IV-2(`Client.vcxproj`·`MFC_000.vcxproj`·`Directory.Build.props` 가 대상에 있지만 범위 목록에 없다).
- **검사 안 함**: P-1~4, I-62, III-9, III-11, III-12.
- **제외 경로**: `2D-Portfolio/Client/fmod/`, `2D-Portfolio/packages/`, `2D-Portfolio/Client/Resource.h`, `2D-Portfolio/MFC_000/resource.h`. 경고 출력에도 이 경로는 없었다.
