#!/usr/bin/env python3
"""Line coverage of the wpw_core sources from a coverage build.

    cmake --preset coverage && cmake --build --preset coverage && ctest --preset coverage
    scripts/coverage.py [build/coverage]

Runs gcov in JSON mode on every .gcda file and merges the line counts, so
template headers compiled into several translation units (OrmObject.h,
OrmRepository.h, OrmService.h) are counted once. Needs only gcov (no lcov).
Exit code 1 when the total is below 80 % (the target in CLAUDE.md).
"""
import json
import os
import subprocess
import sys

TARGET = 80.0
CORE_DIRS = ('src/Database/', 'src/Models/', 'src/Security/')
CORE_COMMON = {'PasswordGenerator', 'ImportCsv', 'ExportCsv', 'CsvModel', 'CsvCodec', 'CsvFormat', 'Logger', 'Tools'}


def is_core(rel):
    if rel.startswith(CORE_DIRS):
        return True
    if rel.startswith('src/Common/'):
        return os.path.splitext(os.path.basename(rel))[0] in CORE_COMMON
    return False


def main():
    root = os.path.realpath(os.path.join(os.path.dirname(__file__), '..'))
    build = os.path.realpath(sys.argv[1] if len(sys.argv) > 1 else os.path.join(root, 'build', 'coverage'))
    lines = {}
    for directory, _, files in os.walk(build):
        for name in files:
            if not name.endswith('.gcda'):
                continue
            out = subprocess.run(['gcov', '--json-format', '--stdout', '-o', directory, os.path.join(directory, name)],
                                 capture_output=True, text=True, cwd=directory).stdout
            for chunk in out.splitlines():
                if not chunk.strip():
                    continue
                data = json.loads(chunk)
                cwd = data.get('current_working_directory', directory)
                for entry in data.get('files', []):
                    path = os.path.realpath(os.path.join(cwd, entry['file']))
                    if not path.startswith(root + os.sep):
                        continue
                    rel = os.path.relpath(path, root)
                    if not is_core(rel):
                        continue
                    counts = lines.setdefault(rel, {})
                    for line in entry['lines']:
                        counts[line['line_number']] = counts.get(line['line_number'], 0) + line['count']
    if not lines:
        sys.exit('coverage.py: no coverage data in %s (build and run the coverage preset first)' % build)
    total = hit = 0
    rows = []
    for rel, counts in lines.items():
        covered = sum(1 for count in counts.values() if count > 0)
        total += len(counts)
        hit += covered
        rows.append((covered / len(counts) * 100, rel, covered, len(counts)))
    for percent, rel, covered, count in sorted(rows):
        print('%6.1f%%  %4d/%-4d  %s' % (percent, covered, count, rel))
    percent = hit / total * 100
    print('TOTAL %d/%d = %.1f%% (target %.0f%%)' % (hit, total, percent, TARGET))
    return 0 if percent >= TARGET else 1


if __name__ == '__main__':
    sys.exit(main())
