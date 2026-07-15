# VERIFY.md

Repository: https://github.com/necat101/c-stdlib-strtod-ml-input-lab
Implementation SHA: 27cf02b76187faae0797721fdc527ac1d1266513

## Clone verification

```
git clone https://github.com/necat101/c-stdlib-strtod-ml-input-lab.git verify-clone
cd verify-clone
git checkout 27cf02b76187faae0797721fdc527ac1d1266513
```

Zig candidates attempted:
- $ZIG_BIN (unset)
- command -v zig → /home/ubuntu/.local/bin/zig
- ~/.local/bin/zig
- ~/bin/zig
- /home/ubuntu/.local/zig/zig

Selected: /home/ubuntu/.local/zig/zig (sanitized: /portable-zig/zig)
Zig version: 0.14.0
zig cc version: clang version 19.1.7
Compiler target: x86_64-unknown-linux-musl (also observed x86_64-unknown-linux-gnu)

Python candidates:
- $PYTHON_BIN (unset)
- python3 → /usr/bin/python3
- python

Selected: /usr/bin/python3 (sanitized: /python-lab/python3)
Python version: 3.12.3
Platform: Linux-6.17.0-1009-aws-x86_64-with-glibc2.39

C representation:
- CHAR_BIT: 8
- sizeof(float): 4
- sizeof(double): 8
- sizeof(long_double): 16
- FLT_RADIX: 2
- DBL_MANT_DIG: 53
- DECIMAL_DIG: 21
- DBL_DECIMAL_DIG: 17

Locale: C
Locale radix: .
Locale restoration: ok

## Validation commands

```
$ZIG_BIN cc -std=c11 -O2 -Wall -Wextra -Wpedantic -Werror parse_lab.c -o parse_lab_check
exit: 0
rm parse_lab_check

python3 -m py_compile run_lab.py test_lab.py
exit: 0

python3 run_lab.py
exit: 0

python3 -m unittest -v
tests run: 11
failures: 0
exit: 0
```

## Results

- Cases: 20
- Methods: 5
- Rows: 100
- Classifications:
  - pass: 31
  - expected_error: 5
  - local_observation: 0
  - locale_skip: 0
  - toolchain_skip: 0
  - context_only: 5
  - not_applicable: 59
  - fail: 0

Key parse results:
- no_conversion consumed: false
- whitespace_and_sign: -12.5
- decimal_fraction: 0.125
- decimal_exponent: 0.0625
- trailing_junk suffix: ms
- overflow isinf: true
- underflow is_zero: true
- negative_zero signbit: 1
- infinity isinf: true
- nan isnan: true
- hex_float equals_3: true
- c_locale_radix dot_offset: 3, comma_offset: 1
- roundtrip items: 5
- vector_token: [0.25, -1.5, 2.0, 0.0]
- bounded_probability: 0, 0.5, 1 accepted
- quantization_scale: 0.0078125, 1 accepted
- fixed_inference_config: temperature 0.8, feature_clip 6.0, quant_scale 0.0078125 accepted

## Evidence comparison

Regenerated results_rows.json matches committed byte-for-byte.
Regenerated results_rows.csv matches committed byte-for-byte.
RESULTS.md differs only in elapsed_time field (0.451s vs 0.208s) – allowed timing variance.

Comparison commands:
```
diff results_rows.json <committed>
# no output – identical
diff results_rows.csv <committed>
# no output – identical
```

Working tree after regeneration: clean (only RESULTS.md elapsed time)
Restoration: git checkout HEAD -- RESULTS.md
Final git status --porcelain: (empty)

Artifact scanner: PASS – all required files present, no prohibited paths, no raw pointers, no environment dumps, no executables committed.

Verification wall-clock: ~2 seconds (clone + build + run + test)

Locale skips: 0
Toolchain skips: 0
Failures: 0

## Post-VERIFY unittest

After writing VERIFY.md locally, reran:
```
python3 -m unittest -v
Ran 11 tests in 0.146s
OK
```

Documentation commit will be direct descendant of implementation SHA 27cf02b76187faae0797721fdc527ac1d1266513.
