"""PreToolUse hook: a lane may not change the verifier or what it compares against.

Claude Code runs this before every Edit, Write, NotebookEdit, Bash and PowerShell call (.claude/settings.json), with
the call as JSON on stdin. Inside a slot worktree (C:/Recomp/stbc-wt/<slot>/) it refuses, with exit code 2 and the
reason on stderr, a change to:

    config/baseline.json   the score baseline (only the orchestrator updates it, at merge)
    tools/decomp.py        the compare, the byte gate and the rule checker's driver
    tools/rules.py         the rule checker
    tools/merge_lane.py    the merge check
    orig/                  the retail binary
    build/gate/            the gate's fresh compile

and `decomp.py report --update-baseline`. The main checkout is not covered: the orchestrator changes these there,
and a lane never writes to it (lane-worker.md). For Bash and PowerShell the check reads the command text: a
redirection into a protected path, or a command that writes files (cp, mv, rm, sed -i, tee, git checkout, Set-Content,
...) naming one. A script that writes the file by other means is not seen; merge_lane.py judges every lane with
main's own tools, which is the backstop.
"""

import json
import re
import sys

PROTECTED_FILES = ("config/baseline.json", "tools/decomp.py", "tools/rules.py", "tools/merge_lane.py")
PROTECTED_DIRS = ("orig/", "build/gate/")
SLOT = re.compile(r"(?:^|/)stbc-wt/[^/]+/(.*)$", re.I)

WRITE_VERBS = re.compile(
    r"^(?:cp|mv|rm|rmdir|tee|touch|truncate|dd|install|ln|chmod|unlink|"
    r"sed\s+(?:-\w*\s+)*-i|perl\s+(?:-\w*\s+)*-i|"
    r"git\s+(?:checkout|restore|rm|mv|reset|apply|am|stash)|"
    r"set-content|add-content|out-file|clear-content|remove-item|copy-item|move-item|rename-item|new-item|"
    r"sc|ac|del|erase|copy|move|ren|ri|rni|cpi|mi)\b", re.I)
REDIRECT = re.compile(r"\d?>>?\s*['\"]?([^\s'\"|;&<>]+)")
SEGMENT = re.compile(r"&&|\|\||[;|\n]")


def norm(path):
    return (path or "").replace("\\", "/")


def protected_rel(rel):
    rel = norm(rel).lstrip("./")
    low = rel.lower()
    if low in PROTECTED_FILES:
        return rel
    for d in PROTECTED_DIRS:
        if low.startswith(d) or low == d.rstrip("/"):
            return rel
    return None


def protected_path(path):
    """The protected path a file path names inside a slot worktree, else None."""
    m = SLOT.search(norm(path))
    return protected_rel(m.group(1)) if m else None


def in_slot(cwd, command):
    return bool(SLOT.search(norm(cwd).rstrip("/") + "/")) or "stbc-wt/" in norm(command).lower()


def token_hit(token):
    """The protected path a command-line token names, absolute or relative to the slot."""
    t = norm(token).strip("'\"")
    hit = protected_path(t)
    if hit:
        return hit
    low = t.lower()
    for p in PROTECTED_FILES:
        if low == p or low.endswith("/" + p):
            return p
    for d in PROTECTED_DIRS:
        if low.startswith(d) or ("/" + d) in low or low == d.rstrip("/"):
            return d
    return None


def command_hit(cwd, command):
    """Why a shell command run in a slot changes a protected path, else None."""
    if not in_slot(cwd, command):
        return None
    if re.search(r"decomp\.py\b.*\breport\b.*--update-baseline", command):
        return "decomp.py report --update-baseline (the orchestrator's, at merge)"
    for target in REDIRECT.findall(command):
        hit = token_hit(target)
        if hit:
            return "a redirection into %s" % hit
    for seg in SEGMENT.split(command):
        words = seg.strip()
        words = re.sub(r"^(?:\w+=\S*\s+)+", "", words)          # VAR=value prefixes
        if not WRITE_VERBS.match(words):
            continue
        for token in words.split()[1:]:
            hit = token_hit(token)
            if hit:
                return "`%s` on %s" % (words.split()[0], hit)
    return None


def verdict(call):
    """The reason to refuse a tool call, or None to let it run."""
    tool = call.get("tool_name", "")
    args = call.get("tool_input") or {}
    if tool in ("Edit", "Write", "MultiEdit", "NotebookEdit"):
        hit = protected_path(args.get("file_path") or args.get("notebook_path"))
        return hit and "%s writes %s" % (tool, hit)
    if tool in ("Bash", "PowerShell"):
        return command_hit(call.get("cwd", ""), args.get("command", ""))
    return None


def main():
    try:
        call = json.load(sys.stdin)
    except ValueError:
        return 0
    why = verdict(call)
    if not why:
        return 0
    sys.stderr.write("Refused in a lane worktree: %s. The verifier (decomp.py, rules.py, merge_lane.py), the score "
                     "baseline, orig/ and build/gate/ are the orchestrator's; report what needs changing instead "
                     "(tools/hooks/protect_verifier.py).\n" % why)
    return 2


if __name__ == "__main__":
    sys.exit(main())
