#!/usr/bin/env python3
DEFAULT_MACROS = {

    "LAMP_INDEX": 2,
    "SENSOR_ACTIVE_HIGH": 0,
    "SENSOR_MODE": "\"ANALOG\"", # NOTE FOR AFTER LUNCH !!! FLASH NUMBER 2!!!

    "IS_ROUTER": 0,
    "ADJACENT": "",

    "NUM_LIGHTS": 4,

    "SENSOR_PIN_ANALOG": "A0", # For the sensor
    "SENSOR_PIN_DIGITAL": 13,

    "RED": 255, 
    "GREEN": 255,
    "BLUE": 255,

    "RED_PIN": 10, # For outputting to the LED
    "GREEN_PIN": 11,
    "BLUE_PIN": 3,

    "ESP_RX_ADD_PIN": 4, # For the arduino to send a signal to the esp to add the adjacent lights to the network list (D2 = GPIO4)
    "ARDU_TX_ADD_PIN": 12,

    "ESP_RX_REMOVE_PIN": 16, # For the arduino to respond to the light timer signal with an ack to remove the light from the network list (D0 = GPIO16)
    "ARDU_TX_REMOVE_PIN": 5,

    "ESP_TX_PIN": 14, # For turning the light timer on from the ESP (D5 = GPIO14)
    "ARDU_RX_PIN": 6,

    "SENSOR_THRESHOLD": 512,

    "NET_SSID": "\"EE120_NET\"",
    "NET_PASS": "\"ee120pass\"",
    "NET_PORT": 4210,
    "MAX_CLIENTS": 8,

    "TIME": 10

}

match DEFAULT_MACROS["LAMP_INDEX"]:
    case 0:
        DEFAULT_MACROS["ADJACENT"] = "{1}"
        DEFAULT_MACROS["IS_ROUTER"] = 1
    case 1:
        DEFAULT_MACROS["ADJACENT"] = "{0, 2, 3}"
    case 2:
        DEFAULT_MACROS["ADJACENT"] = "{1, 3}"
    case 3:
        DEFAULT_MACROS["ADJACENT"] = "{1, 2}"

DEFAULT_MACROS["LIGHT_INDEX"] = DEFAULT_MACROS["LAMP_INDEX"]

## This is a slop script to patch our hacky cross platform macro system.



import json
import re
import sys
from pathlib import Path

DEFAULT_ROOTS = ["ardu code", "ESP_code_EE120_final"]
SUBDIRS = ["src", "lib"]
EXTENSIONS = {".h", ".hpp", ".c", ".cpp", ".cc", ".ino"}

DEFINE_PATTERN = re.compile(
    r"^(\s*#\s*define\s+)(\w+)(\s+)(.*?)(\s*(?://|/\*).*)?$",
    re.MULTILINE,
)


class UnknownMacroError(Exception):
    pass


def patch_text(text: str, macros: dict, source: str = "<text>") -> tuple[str, list[tuple[str, str, str]]]:
    changes: list[tuple[str, str, str]] = []

    def _sub(m: re.Match) -> str:
        prefix, name, ws, old_value, comment = (
            m.group(1),
            m.group(2),
            m.group(3),
            m.group(4).rstrip(),
            m.group(5) or "",
        )
        if name not in macros:
            raise UnknownMacroError(
                f"{source}: #define {name!r} is not listed in DEFAULT_MACROS "
                f"(add it with a value, or set it to None to leave it unmodified)"
            )
        new_value = macros[name]
        if new_value is None:
            return m.group(0)
        new_value_str = str(new_value)
        if old_value == new_value_str:
            return m.group(0)
        changes.append((name, old_value, new_value_str))
        return f"{prefix}{name}{ws}{new_value_str}{comment}"

    text = DEFINE_PATTERN.sub(_sub, text)
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
        rel = path.relative_to(here) if path.is_relative_to(here) else path
        patched, changes = patch_text(original, macros, str(rel))
        if changes:
            path.write_text(patched)
            files_changed += 1
            total_changes += len(changes)
            print(f"{rel}")
            for name, old, new in changes:
                print(f"  {name}: {old!r} -> {new!r}")

    print(f"\n{total_changes} replacement(s) across {files_changed} file(s).")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
