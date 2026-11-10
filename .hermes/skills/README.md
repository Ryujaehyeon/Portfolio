# 저장소 스킬

이 폴더의 스킬은 하네스 저장소가 정본입니다. git 으로 관리하는 저장소는 커밋·`git pull` 로 동기화하고,
하네스 파일을 VCS 밖에 두는 product 는 update 스크립트(SVN 작업본은 `update.sh --svn`)로 갱신합니다.

## 새 스킬 추가

```
.hermes/skills/<스킬이름>/SKILL.md
```

```markdown
---
name: 스킬이름
description: 언제 이 스킬을 써야 하는지 (자동 트리거 판단 기준)
---

<작업 절차·지침>
```

git 으로 관리하는 저장소라면 추가 후 커밋·푸시해야 다른 장치에 반영됩니다. update 스크립트는 하네스에 있는
스킬만 덮어쓰므로 product 가 직접 만든 스킬은 그대로 남습니다.
