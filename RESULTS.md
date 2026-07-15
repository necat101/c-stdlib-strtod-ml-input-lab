# RESULTS

zig: 0.14.0
target: x86_64-linux-musl
python: 3.12.3
platform: Linux-6.17.0-1009-aws-x86_64-with-glibc2.39

cases: 20
methods: 5
rows: 100

Classification totals:
- pass: 26
- expected_error: 5
- local_observation: 0
- locale_skip: 0
- toolchain_skip: 0
- context_only: 5
- not_applicable: 64
- fail: 0

no_conversion consumed: False
whitespace_and_sign value: -12.5
decimal_fraction: 0.125
decimal_exponent: 0.0625
trailing_junk suffix: ms
overflow isinf: 1
overflow isinf: 1
underflow is_zero: 1
negative_zero signbit: 1
infinity isinf: 1
nan isnan: 1
hex_float equals_3: 1
c_locale_radix dot_offset: 1, comma_offset: 0
roundtrip items: 7
vector_token: [0.25, -1.5, 2.0, 0.0]

elapsed: 0.372s

Narrow conclusion: local libc strtod correctness observed; no ML validation.
