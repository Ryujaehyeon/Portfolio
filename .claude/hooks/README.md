# Hooks

이 폴더는 hook **스크립트만** 둔다. `~/.claude/settings.json` 이나 저장소 설정에 **배선하지 않는다** — 배선은 사용자가 직접 한다.

## pre-commit-secret-scan.sh

staged 파일 내용을 임시 디렉터리에 펼친 뒤 `.claude/hooks/secret-scan.py` 로 스캔하고, 비밀정보 후보가 발견되면 **exit 2** 로 종료해 커밋을 막는다. exit 2 는 git hook 과 Claude Code hook 양쪽에서 차단으로 동작한다.

- 정본 스캐너: `.claude/hooks/secret-scan.py` — 저장소 루트 기준으로 찾으므로 스크립트를 어디서 호출해도 된다
- 검사 대상: `git diff --cached --diff-filter=d` (삭제를 뺀 전부 — 이름을 바꾼 파일 포함)
- 검사 내용: staged 블롭(`git show :path`) — 워킹트리가 아님
- 검사 범위: 바이너리(NUL 바이트 포함)를 뺀 모든 파일 — BOM 이 있는 UTF-16 텍스트는 검사한다. 서브모듈은 그 저장소에서 검사한다. `.env`·`.env.*`(`.env.example` 제외) 같은 비밀정보 파일은 내용과 무관하게 차단
- git 작업본 전용: SVN 작업본에서는 hook 이 동작하지 않는다(스크립트가 git 으로 staged 파일을 찾는다). 대신 `python3 .claude/hooks/secret-scan.py <파일|디렉터리>` 로 직접 검사한다

### 수동 실행

```
bash .claude/hooks/pre-commit-secret-scan.sh        # staged 파일 검사
python3 .claude/hooks/secret-scan.py <파일|디렉터리>   # 임의 경로 검사
```

### git 네이티브 hook 으로 배선 (권장)

실제 커밋 시점에 돌기 때문에 git 작업본에서 사람·Claude·Hermes 의 커밋을 모두 막는다.

```
printf '#!/bin/sh\nexec bash "$(git rev-parse --show-toplevel)/.claude/hooks/pre-commit-secret-scan.sh"\n' > .git/hooks/pre-commit
chmod +x .git/hooks/pre-commit
```

`.git/hooks/` 는 저장소에 커밋되지 않으므로 각 클론에서 한 번씩 연결해야 한다.

### Claude Code hook 으로 배선 (보조)

`~/.claude/settings.json` 또는 저장소 `.claude/settings.json` 의 `hooks.PreToolUse` 에 아래 항목을 추가한다. 클론마다 연결할 필요는 없지만 Claude 의 커밋만 막는다.

```json
{
  "hooks": {
    "PreToolUse": [
      {
        "matcher": "Bash",
        "hooks": [
          {
            "type": "command",
            "if": "Bash(git commit *)",
            "command": "bash",
            "args": ["${CLAUDE_PROJECT_DIR}/.claude/hooks/pre-commit-secret-scan.sh"]
          }
        ]
      }
    ]
  }
}
```

- `if` 는 명령 안의 하위 명령까지 검사하므로 `git add -A && git commit ...` 에도 hook 이 실행된다. 인자 없는 `git commit` 도 해당한다.
- `args` 가 있으면 셸 없이 실행(exec form)되어 경로에 공백이 있어도 안전하다.
- Windows 에서 Claude Code 를 WSL 이 아니라 직접 실행한다면 `command` 에 Git Bash 의 절대 경로를 적는다
  (예: `"C:\\Program Files\\Git\\bin\\bash.exe"`). WSL 이 깔린 PC 에서는 PATH 의 `bash` 가 WSL 실행기
  (`…\WindowsApps\bash.exe`)로 풀려, Windows 경로의 스크립트를 열지 못하고 차단도 하지 못한다.
- 비밀정보가 발견되면 exit 2 로 끝나 Claude Code 가 해당 Bash 호출을 차단한다. exit 1 은 차단하지 않는다.
- 한계: hook 은 명령이 실행되기 **전에** 돈다. `git add` 와 `git commit` 을 한 명령에 묶으면 그 `git add` 로 새로 staging 한 파일은 검사하지 못한다 — 그래서 git 네이티브 hook 을 권장한다.
