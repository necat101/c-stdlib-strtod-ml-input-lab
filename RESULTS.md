# RESULTS

zig: 0.14.0
target: x86_64-unknown-linux-musl
python: 3.12.3
platform: Linux-6.17.0-1009-aws-x86_64-with-glibc2.39

cases: 20
methods: 5
rows: 100

Classification totals:
- pass: 31
- expected_error: 5
- local_observation: 0
- locale_skip: 0
- toolchain_skip: 0
- context_only: 5
- not_applicable: 59
- fail: 0

no_conversion consumed: False
whitespace_and_sign value: -12.5
decimal_fraction: 0.125
decimal_exponent: 0.0625
trailing_junk suffix: ms
overflow isinf: True
underflow is_zero: True
negative_zero signbit: 1
infinity isinf: True
nan isnan: True
hex_float equals_3: True
c_locale_radix dot_offset: 3, comma_offset: 1
roundtrip items: 5
vector_token: [0.25, -1.5, 2, 0]

elapsed: 0.208s

Narrow conclusion: local libc strtod correctness observed; no ML validation.
