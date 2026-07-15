# VERIFY.md — v2

Repository: https://github.com/necat101/c-stdlib-strtod-ml-input-lab
Implementation SHA: b64a14714452e07c562ce8549225aa769b12b286

This is the v2 implementation, addressing the audit feedback from 2026-07-15.

## Changes from v1

- parse_lab.c now exercises `strtof()`, `strtod()`, and `strtold()` via function pointers
- bounded_probability: full 9-input set parsed and classified
- quantization_scale_policy: full 8-input set parsed and classified
- fixed_inference_config: 3 valid + 3 invalid inputs with full range checking
- run_lab.py handlers verify observations against cdata (behavior-driven), not case ID allowlists
- missing handler now emits `fail` with reason instead of KeyError
- no-Zig path properly marks `toolchain_skip` and nulls c-dependent fields
- test_lab.py: 21 tests (was 11), with independent parse verification, policy input verification, artifact scanner with real rules, JSON/CSV field-level agreement
- RESULTS.md derives entirely from the rows collection
- sanitized paths in README, run_lab.py, test_lab.py
- test_artifact_scanner no longer false-positives on its own source

## Clone verification

```
git clone https://github.com/necat101/c-stdlib-strtod-ml-input-lab.git verify-clone-v2
cd verify-clone-v2
git checkout b64a14714452e07c562ce8549225aa769b12b286
```

Zig: 0.14.0 — /portable-zig/zig
Target: x86_64-unknown-linux-musl
Python: 3.12.3 — /python-lab/python3
Platform: Linux-6.17.0-1009-aws-x86_64-with-glibc2.39

C representation: CHAR_BIT=8, sizeof(float)=4, sizeof(double)=8, sizeof(long_double)=16, DBL_MANT_DIG=53, DECIMAL_DIG=21, DBL_DECIMAL_DIG=17

Locale: C, radix=".", restoration: ok

## Validation commands

```
$ZIG_BIN cc -std=c11 -O2 -Wall -Wextra -Wpedantic -Werror parse_lab.c -o parse_lab_check
exit: 0
python3 -m py_compile run_lab.py test_lab.py
exit: 0
python3 run_lab.py
exit: 0
python3 -m unittest -v
tests run: 21
failures: 0
exit: 0
```

## Results

- Cases: 20
- Methods: 5
- Rows: 100
- Classifications: pass 31, expected_error 5, context_only 5, not_applicable 59, fail 0, toolchain_skip 0, locale_skip 0, local_observation 0

Parse results (from rows):
- no_conversion endptr_offset: 0
- whitespace_and_sign: -12.5
- decimal_fraction: 0.125
- decimal_exponent: 0.0625
- trailing_junk suffix: ms
- overflow isinf: true
- underflow isfinite: true
- negative_zero signbit: 1
- infinity isinf: true
- nan isnan: true
- hex_float: 3.0
- c_locale_radix endptr_offset: 3
- roundtrip items: 5
- vector_token count: 4, values [0.25, -1.5, 2.0, 0.0]
- bounded_probability: accepts 0, 0.5, 1 — rejects -0.001, 1.001, nan, inf, 0.5x, ""
- quantization_scale: accepts 0.0078125, 1 — rejects 0, -0.5, 1.0001, nan, 1e9999, 0.01x
- fixed_inference_config: temperature 0.8, feature_clip 6.0, quant_scale 0.0078125 accepted; nan, 6x, 0 rejected

## Evidence comparison

Regenerated results_rows.json: identical to committed
Regenerated results_rows.csv: identical to committed
RESULTS.md: differs only in elapsed_time field (allowed)

Working tree after regeneration: clean (timing-only change in RESULTS.md)
Restoration: git checkout HEAD -- RESULTS.md
Final git status --porcelain: (empty)

Artifact scanner: PASS
Unittest: 21 tests, OK
Wall-clock verification duration: ~3s

Locale skips: 0
Toolchain skips: 0
Failures: 0

## Post-VERIFY unittest

After writing VERIFY.md:
```
python3 -m unittest -v
Ran 21 tests in 0.760s
OK
```

Documentation commit is direct descendant of b64a14714452e07c562ce8549225aa769b12b286.
