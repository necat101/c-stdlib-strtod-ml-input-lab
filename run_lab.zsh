#!/usr/bin/env zsh
set -euo pipefail
# Resolve zig (matches run_lab.py order + portable install)
if [[ -n "${ZIG_BIN:-}" && -x "$ZIG_BIN" ]]; then ZIG="$ZIG_BIN"
elif (( $+commands[zig] )); then ZIG="$(command -v zig)"
elif [[ -x "$HOME/.local/bin/zig" ]]; then ZIG="$HOME/.local/bin/zig"
elif [[ -x "$HOME/bin/zig" ]]; then ZIG="$HOME/bin/zig"
elif [[ -x "$HOME/.local/zig/zig" ]]; then ZIG="$HOME/.local/zig/zig"
else echo "zig not found (tried ZIG_BIN, PATH, ~/.local/bin/zig, ~/bin/zig, ~/.local/zig/zig)" >&2; exit 1; fi
# Resolve python
if [[ -n "${PYTHON_BIN:-}" && -x "$PYTHON_BIN" ]]; then PY="$PYTHON_BIN"
elif (( $+commands[python3] )); then PY="$(command -v python3)"
elif (( $+commands[python] )); then PY="$(command -v python)"
else echo "python not found" >&2; exit 1; fi
echo "zig: $($ZIG version)"
echo "python: $($PY --version)"
"$ZIG" cc -std=c11 -O2 -Wall -Wextra -Wpedantic -Werror parse_lab.c -o parse_lab_check
rm -f parse_lab_check parse_lab parse_lab.exe
"$PY" -m py_compile run_lab.py test_lab.py
"$PY" run_lab.py
"$PY" -m unittest -v
