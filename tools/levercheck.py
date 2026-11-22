"""Lever audit helper: the lever entries whose cited examples no longer agree with the functions' status, and
whether an audit is due (one every AUDIT_EVERY lane merges, user's ruling 2026-11-21).

    python tools/levercheck.py            # due status, then the suspect entries
    python tools/levercheck.py due        # only: lane merges since the last lever audit
    python tools/levercheck.py --json F   # also write every entry's cited functions and their status to F

An entry is one top-level bullet of docs/levers/*.md. It cites a function by address (0x...) or by Class::Method.
Two kinds of entry are suspect (a report; it edits nothing):
  - every cited function is open, and the entry has no "(open)" mark: it may claim a lever closed what is open;
  - the entry marks something "(open)" and a cited function now matches: the mark may be out of date.
Statuses come from build/report.json (run `decomp.py build` first). The last audit is the latest commit whose
subject starts with "docs: lever audit"; the count is the "merge: lane/" commits after it.
"""
import argparse
import collections
import json
import os
import re
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
LEVERS = os.path.join(ROOT, 'docs', 'levers')
AUDIT_EVERY = 9
AUDIT_SUBJECT = 'docs: lever audit'

ADDR_RE = re.compile(r'0x([0-9A-Fa-f]{6,8})\b')
NAME_RE = re.compile(r'\b([A-Z][A-Za-z0-9_]*::~?[A-Za-z_][A-Za-z0-9_]*)\b')
OPEN_RE = re.compile(r'\(open\b')


def split_entries(text):
    """Top-level bullets of a lever file: [(line, heading, text)]. A bullet runs to the next line that is not
    indented (continuation lines and sub-bullets are indented)."""
    entries, cur, start, head = [], None, 0, ''
    for i, line in enumerate(text.split('\n')):
        if line.startswith('#'):
            if cur is not None:
                entries.append((start, head, '\n'.join(cur)))
                cur = None
            head = line.strip('# ').strip()
        elif line.startswith('- '):
            if cur is not None:
                entries.append((start, head, '\n'.join(cur)))
            cur, start = [line], i + 1
        elif cur is not None and (line.startswith(' ') or line.startswith('\t')):
            cur.append(line)
        elif cur is not None:
            entries.append((start, head, '\n'.join(cur)))
            cur = None
    if cur is not None:
        entries.append((start, head, '\n'.join(cur)))
    return entries


def is_matched(f):
    return f['status'] == 'MATCH'


def status_text(f):
    s = f['status']
    return 'DIFF %.1f' % f['score'] if s.startswith('DIFF') else s


def load_functions(report):
    by_addr, by_name = {}, collections.defaultdict(list)
    for f in report['functions']:
        by_addr[int(f['addr'], 16)] = f
        by_name[f['name']].append(f)
        parts = f['name'].split('::')
        if len(parts) > 2:
            by_name['::'.join(parts[-2:])].append(f)
    return by_addr, by_name


def cited(text, by_addr, by_name):
    out = {}
    for m in ADDR_RE.finditer(text):
        f = by_addr.get(int(m.group(1), 16))
        if f:
            out[f['addr']] = f
    for m in NAME_RE.finditer(text):
        fs = by_name.get(m.group(1), [])
        if len(fs) <= 3:  # an overloaded name with many copies says nothing about one of them
            for f in fs:
                out[f['addr']] = f
    return list(out.values())


def classify(text, cites):
    # a PATCHED function (binary-patched in retail, written as the original) is neither matched nor open
    cites = [f for f in cites if f['status'] != 'PATCHED']
    if not cites:
        return None
    has_open = bool(OPEN_RE.search(text))
    matched = [f for f in cites if is_matched(f)]
    if not matched and not has_open:
        return 'all-open-unmarked'
    if has_open and matched and len(matched) == len(cites):
        return 'open-mark-all-matched'
    return None


def lane_merges_since_audit():
    def git(*args):
        return subprocess.run(['git', '-C', ROOT] + list(args), capture_output=True, text=True).stdout
    last = git('log', '--format=%H', '-1', '--grep=^' + AUDIT_SUBJECT).strip()
    rng = (last + '..HEAD') if last else 'HEAD'
    subjects = git('log', '--first-parent', '--format=%s', rng).splitlines()
    return last[:9] or None, sum(1 for s in subjects if s.startswith('merge: lane/'))


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('what', nargs='?', default='all', choices=['all', 'due'])
    ap.add_argument('--json')
    a = ap.parse_args(argv)

    last, n = lane_merges_since_audit()
    due = n >= AUDIT_EVERY
    print('lever audit: %d lane merge(s) since the last audit (%s); one is due every %d: %s'
          % (n, last or 'none', AUDIT_EVERY, 'DUE' if due else 'not due'))
    if a.what == 'due':
        return 0

    report = json.load(open(os.path.join(ROOT, 'build', 'report.json'), encoding='utf8'))
    by_addr, by_name = load_functions(report)
    rows, suspects = [], collections.defaultdict(list)
    for fn in sorted(os.listdir(LEVERS)):
        if not fn.endswith('.md') or fn == 'README.md':
            continue
        text = open(os.path.join(LEVERS, fn), encoding='utf8').read()
        for line, head, body in split_entries(text):
            cites = cited(body, by_addr, by_name)
            kind = classify(body, cites)
            row = {'file': fn, 'line': line, 'head': head, 'first': body.split('\n')[0][:120],
                   'cites': ['%s %s=%s' % (f['addr'], f['name'], status_text(f)) for f in cites], 'suspect': kind}
            rows.append(row)
            if kind:
                suspects[kind].append(row)
    print('%d entries cite functions; suspect: %d with every example open and no (open) mark, %d marked (open) '
          'whose examples all match' % (sum(1 for r in rows if r['cites']), len(suspects['all-open-unmarked']),
                                         len(suspects['open-mark-all-matched'])))
    for kind in ('all-open-unmarked', 'open-mark-all-matched'):
        for r in suspects[kind]:
            print('  [%s] docs/levers/%s:%d  %s' % (kind, r['file'], r['line'], r['first'][:80]))
            for c in r['cites'][:4]:
                print('        ' + c)
    if a.json:
        json.dump(rows, open(a.json, 'w', encoding='utf8'), indent=1)
    return 0


if __name__ == '__main__':
    sys.exit(main())
