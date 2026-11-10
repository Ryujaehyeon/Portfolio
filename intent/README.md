# intent/

이 제품 repo의 모든 변경에 대한 **SSOT(Single Source of Truth)**. 요구·설계·계획을
외부 트래커(Jira/Confluence 등)가 아니라 이 폴더의 마크다운 + PR 이력에 남긴다.
도구는 GitHub(repo·PR)·Slack(@claude 태그·알림)·Claude 셋뿐이다.

## 폴더 규칙

```
intent/
├── README.md            # 이 파일
└── <기능-슬러그>/        # 변경 1개 = 폴더 1개 (예: ship-tracking)
    ├── intent.md         # 왜 만드나          (초안: 에이전트 / 승인: PO)
    ├── spec.md           # 무엇을·요구·설계   (초안: 에이전트 / 승인: PO)
    └── plan.md           # 어디를·순서·검증   (초안: 에이전트 / 승인: 개발자)
```

## 규칙 3줄

1. 폴더당 `intent.md`·`spec.md`·`plan.md` 하나씩. spec 이 둘 필요하면 슬러그를 쪼갠다.
2. PR 하나 = 산출물 하나, 머지가 승인. 브랜치 `intent/<슬러그>` → `spec/<슬러그>` → `feature/<슬러그>`,
   전부 트렁크(MAIN, develop 을 쓰면 develop)로 머지. PR 이 없는 저장소(SVN 등)는 PR 머지 대신 산출물의
   `Status: approved (승인자, 날짜)` 가 승인이다(`.harness/sdlc/README.md` 「브랜치·승인 규약」).
3. 슬러그는 `<공통어간>-<구분어>`, 소문자+하이픈, 읽어서 아는 단어 (❌ `a-1`, ✅ `can-codec-cannelloni`).
   폴더·브랜치·커밋 scope·PR 제목에 같은 문자열을 쓴다.

## SSOT 표

| 파일 | 답하는 질문 | 초안 | 승인 |
|---|---|---|---|
| `CLAUDE.md` / `AGENTS.md` | 팀이 지키는 규칙·관례 | — | 팀 |
| `intent.md` | 왜 이 변경이 필요한가 | 에이전트 | PO |
| `spec.md` | 무엇을 어떤 요구·설계로 만드나 | 에이전트 | PO |
| `plan.md` | 어디를 어떤 순서로 고치고 무엇으로 검증하나 | 에이전트 | 개발자 |

단계별 운영 지침은 `.harness/sdlc/` 참조.
