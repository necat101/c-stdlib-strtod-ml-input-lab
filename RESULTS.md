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

no_conversion endptr_offset: 0
whitespace_and_sign value: -12.5
decimal_fraction value: 0.125
decimal_exponent value: 0.0625
trailing_junk suffix: ms
overflow isinf: True
underflow isfinite: True
negative_zero signbit: 1
infinity isinf: True
nan isnan: True
hex_float value: 3
c_locale_radix endptr_offset: 3
roundtrip items: 5
vector_token count: 4
bounded_probability policy_accepted: True
quantization_scale policy_accepted: True
fixed_inference_config policy: fixed_inference

elapsed: 0.380s

Narrow conclusion: local libc strtod/strtof/strtold correctness observed; no ML validation.
