#!/usr/bin/env bash
#
# staged 파일에서 비밀정보를 스캔하고, 발견 시 exit 2 로 종료해 커밋을 차단한다.
# exit 2 는 git pre-commit hook 과 Claude Code PreToolUse hook 양쪽에서 차단으로 동작한다
# (Claude Code 는 exit 1 을 차단하지 않는다).
# 스캐너(secret-scan.py)는 저장소 루트의 .claude/hooks/ 에서 찾는다 — 이 스크립트를
# .git/hooks/ 에서 호출하거나 symlink 로 걸어도 위치가 바뀌지 않는다.
#
# 배선 방법은 .claude/hooks/README.md 참고. 이 스크립트 자체는 아무 설정도 건드리지 않는다.

set -euo pipefail
# 예기치 않은 오류로 끝나도 exit 2 로 막는다 — 다른 종료 코드는 Claude Code 가 차단하지 않는다(fail-closed).
trap 'exit 2' ERR

repo_root=$(git rev-parse --show-toplevel)
scanner="$repo_root/.claude/hooks/secret-scan.py"

if [[ ! -f "$scanner" ]]; then
    printf 'secret-scan: %s 를 찾을 수 없습니다.\n' "$scanner" >&2
    exit 2
fi

# staged 상태에서 삭제를 뺀 모든 파일을 대상으로 한다 — 이름을 바꾼 파일(R)도 넣어야
# rename 과 함께 넣은 비밀정보나 .env.example → .env 같은 우회를 잡는다.
# mapfile -d 는 bash 4.4 이상이라 macOS 기본 bash(3.2)에서도 도는 read 루프를 쓴다.
staged=()
while IFS= read -r -d '' path; do
    staged+=("$path")
done < <(git diff --cached --name-only -z --diff-filter=d)

if [[ ${#staged[@]} -eq 0 ]]; then
    exit 0
fi

# staged 블롭 내용을 임시 디렉터리에 그대로 펼친 뒤 스캐너를 디렉터리 단위로 돌린다.
# 워킹트리가 아니라 "실제로 커밋될 내용"을 검사하기 위함이다.
tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT

for path in "${staged[@]}"; do
    # 서브모듈(gitlink)은 블롭이 없어 git show 가 실패한다. 그 내용은 서브모듈 저장소에서 검사한다.
    [[ $(git ls-files -s -- "$path") == 160000* ]] && continue
    mkdir -p "$tmp/$(dirname "$path")"
    git show ":$path" > "$tmp/$path"
done

# 차단 사유(파일:줄)를 stderr 로 낸다 — Claude Code PreToolUse 는 exit 2 일 때 stderr 만 모델에 전달한다.
if ! python3 "$scanner" "$tmp" >&2; then
    printf '\nsecret-scan: staged 파일에서 비밀정보 후보가 발견되어 커밋을 차단합니다.\n' >&2
    exit 2
fi
