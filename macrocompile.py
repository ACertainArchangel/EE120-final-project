#!/usr/bin/env python3
DEFAULT_MACROS = {
    "NUM_LIGHTS": 4,
    "LED_PIN": 13,
    "SENSOR_PIN": 2,
}



## This is a slop script to patch our hacky cross platform macro system.



import json
import re
import sys
from pathlib import Path

DEFAULT_ROOTS = ["ardu code", "ESP_code_EE120_final"]
SUBDIRS = ["src", "lib"]
EXTENSIONS = {".h", ".hpp", ".c", ".cpp", ".cc", ".ino"}

def build_pattern(name: str) -> re.Pattern:
    # Capture: (1) "#define NAME " prefix incl. whitespace
    #         (2) the value (everything up to a comment or EOL)
    #         (3) the trailing comment, if any
    return re.compile(
        r"^(\s*#\s*define\s+" + re.escape(name) + r"\s+)"
        r"(.*?)"
        r"(\s*(?://|/\*).*)?$",
        re.MULTILINE,
    )


def patch_text(text: str, macros: dict) -> tuple[str, list[tuple[str, str, str]]]:
    changes: list[tuple[str, str, str]] = []

    def make_sub(name: str, new_value: str):
        def _sub(m: re.Match) -> str:
            prefix, old_value, comment = m.group(1), m.group(2).rstrip(), m.group(3) or ""
            if old_value == new_value:
                return m.group(0)
            changes.append((name, old_value, new_value))
            return f"{prefix}{new_value}{comment}"
        return _sub

    for name, value in macros.items():
        text = build_pattern(name).sub(make_sub(name, str(value)), text)
    return text, changes


def iter_files(roots: list[Path]):
    for root in roots:
        if not root.exists():
            continue
        for sub in SUBDIRS:
            base = root / sub
            if not base.exists():
                continue
            for path in base.rglob("*"):
                if path.is_file() and path.suffix in EXTENSIONS:
                    yield path


def main(argv: list[str]) -> int:
    here = Path(__file__).resolve().parent
    args = argv[1:]

    root_args = args[1:] if len(args) > 1 else DEFAULT_ROOTS
    roots = [Path(r) if Path(r).is_absolute() else here / r for r in root_args]


    macros = DEFAULT_MACROS

    if not isinstance(macros, dict):
        print("error: macro file must contain a JSON object", file=sys.stderr)
        return 1

    total_changes = 0
    files_changed = 0
    for path in iter_files(roots):
        original = path.read_text()
        patched, changes = patch_text(original, macros)
        if changes:
            path.write_text(patched)
            files_changed += 1
            total_changes += len(changes)
            rel = path.relative_to(here) if path.is_relative_to(here) else path
            print(f"{rel}")
            for name, old, new in changes:
                print(f"  {name}: {old!r} -> {new!r}")

    print(f"\n{total_changes} replacement(s) across {files_changed} file(s).")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
