# VERIFY.md — v2

Repository: https://github.com/necat101/c-stdlib-strtod-ml-input-lab
Implementation SHA: 0700a8465a3de5f5f77bf8ef788addd598a375dc

v2 with wrapper scripts (run_lab.sh / run_lab.bat / run_lab.zsh).

## Clone verification

```
git clone https://github.com/necat101/c-stdlib-strtod-ml-input-lab.git
cd c-stdlib-strtod-ml-input-lab
git checkout 0700a8465a3de5f5f77bf8ef788addd598a375dc
```

Zig: 0.14.0 — /portable-zig/zig
Target: x86_64-unknown-linux-musl
Python: 3.12.3 — /python-lab/python3
Platform: Linux-6.17.0-1009-aws-x86_64-with-glibc2.39

## Validation

```
$ZIG_BIN cc -std=c11 -O2 -Wall -Wextra -Wpedantic -Werror parse_lab.c -o parse_lab_check
exit: 0
python3 -m py_compile run_lab.py test_lab.py
exit: 0
python3 run_lab.py
exit: 0
python3 -m unittest -v
tests: 21, failures: 0
```

## Results

Cases: 20, Methods: 5, Rows: 100
Classifications: pass 31, expected_error 5, context_only 5, not_applicable 59, fail 0

- strtof/strtod/strtold exercised via function pointers
- bounded_probability: 9 inputs verified
- quantization_scale: 8 inputs verified  
- fixed_inference_config: 3 valid + 3 invalid verified
- vector_token: [0.25, -1.5, 2.0, 0.0] verified
- all handlers behavior-driven
- missing-handler → fail path works
- no-Zig toolchain_skip path works
- 21 tests, JSON/CSV field-level agreement, artifact scanner passes
- RESULTS.md derives from rows
- paths sanitized

Regenerated evidence matches committed (json/csv identical, RESULTS.md elapsed_time only difference).

Wall-clock: ~3s
Locale skips: 0, Toolchain skips: 0, Failures: 0
