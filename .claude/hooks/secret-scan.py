#!/usr/bin/env python3
"""Fail when files contain likely credentials or forbidden files."""
from __future__ import annotations

import re
import sys
from pathlib import Path

if len(sys.argv) != 2:
    print("SECRET SCAN: ERROR (usage: secret-scan.py <file|directory>)")
    raise SystemExit(2)

ROOT = Path(sys.argv[1]).resolve()
FORBIDDEN_NAMES = {".env", "auth.json", "credentials.json", "credentials.db", "cookies.json"}
ALLOWED_ENV_NAMES = {".env.example"}

PATTERNS = {
    "OpenAI-style secret": re.compile(r"\bsk-(?:proj-|ant-)?[A-Za-z0-9_-]{16,}\b"),
    "GitHub token": re.compile(r"\bgh[opusr]_[A-Za-z0-9]{20,}\b"),
    "private key": re.compile(r"-----BEGIN (?:RSA |EC |OPENSSH )?PRIVATE KEY-----"),
    "assigned secret": re.compile(
        r"(?i)(?:api[_-]?key|access[_-]?token|refresh[_-]?token|password|client[_-]?secret)"
        r"[\"']?[ \t]*[:=][ \t]*[\"']?(?!<|\[REDACTED\])[A-Za-z0-9_./+\-=]{12,}"
    ),
}

if not ROOT.exists():
    print(f"SECRET SCAN: ERROR (no such path: {ROOT})")
    raise SystemExit(2)

findings: list[str] = []
base = ROOT.parent if ROOT.is_file() else ROOT
for path in [ROOT] if ROOT.is_file() else sorted(ROOT.rglob("*")):
    if not path.is_file():
        continue
    rel = path.relative_to(base)
    if path.name in FORBIDDEN_NAMES or (
        path.name.startswith(".env.") and path.name not in ALLOWED_ENV_NAMES
    ):
        findings.append(f"forbidden file: {rel}")
        continue
    data = path.read_bytes()
    if data.startswith((b"\xff\xfe", b"\xfe\xff")):
        text = data.decode("utf-16", errors="replace")
    elif b"\0" in data[:8192]:
        continue
    else:
        text = data.decode("utf-8", errors="replace")
    for label, pattern in PATTERNS.items():
        for match in pattern.finditer(text):
            line = text.count("\n", 0, match.start()) + 1
            findings.append(f"{label}: {rel}:{line}")

if findings:
    print("SECRET SCAN: FAIL")
    for finding in findings:
        print(f"- {finding}")
    raise SystemExit(1)

print(f"SECRET SCAN: PASS ({ROOT})")
