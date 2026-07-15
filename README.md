# c-stdlib-strtod-ml-input-lab

Correctness lab for C standard library `strtod` / `strtof` / `strtold` floating-point text parsing, with machine-learning-adjacent input policy examples.

No benchmark. No model training. No dataset.

## Scope

- `strtod` / `strtof` / `strtold`
- `endptr`, `errno`, ERANGE
- whitespace, signs, decimal fractions, exponents, trailing text
- overflow, underflow, negative zero, infinity, NaN, hexadecimal floats
- C locale radix `.`
- `snprintf` round-trip (same implementation)
- Bounded probability policy [0,1]
- Vector token sequence parsing
- Quantization-scale policy (0,1]
- Fixed inference config parsing (temperature, feature_clip, quant_scale)

All parsing is single-threaded, C locale, fixed inputs.

## Results

See RESULTS.md (generated).

## Hacker News thread access

Thread 24436158 was read via:

```
hackernews get-item --id 24436158
```

using the bundled real Hacker News CLI (`/usr/lib/node_modules/openclaw/dist/extensions/hackernews/skills/hackernews/hackernews`).

Relevant public evidence captured in `hn_thread_evidence.md` and `hn_comments_sanitized.json` before preparing the sentiment summary.

## HN discussion summary

- **jeffbee** pointed to `absl::from_chars` and reported a materially faster result in a modified benchmark, comparing an alternate `from_chars` implementation rather than treating the article's two implementations as the only options.
- **osolo** linked Stephan Lavavej's CppCon 2019 talk explaining floating-point character conversion, pointing readers toward algorithm and implementation details.
- **sai_peregrinus** said the article's first `strtod` example was incomplete because it did not clear and inspect `errno` or account for `HUGE_VAL` range results; correct `strtod` use needs range-error handling beyond checking whether characters were consumed.
- **jwilk** added that underflow may return zero, and noted that the compared C++ example also omitted complete range checks – both sides need complete correctness checks.
- **thangalin** said the opposite float-to-string direction is important and connected that work to Ryū, noting float formatting is a separate and useful conversion direction.
- **someonefromca** argued that locale may be unimportant in many programs because they remain in the C locale; that position does not allow middleware or libraries to assume the C locale without evidence.
- **wolf550e** expected optimized alternatives to the implementation bundled with a compiler or runtime.
- **someone** warned that a faster parser may omit NaN, infinity, denormal, or round-trip edge cases.
- **titzer** said the hardest cases are often large exponents and long decimal significands rather than NaN and infinity, and explained that correct conversion may require large-integer arithmetic and difficult rounding work.
- **alfalfasprout** identified human-readable serialization and fixed text-producing interfaces as major float-parsing use cases, and distinguished the JSON number grammar from the set of IEEE binary floating-point values.
- **dundarious** argued that text numerical protocols still matter outside the narrowest hot loops (FIX messages for compliance/auditing, etc.).

The thread contains disagreement rather than one settled parser recommendation. The local lab prioritizes correctness and evidence rather than reproducing the benchmark.

Text-configured thresholds and scales are machine-learning-adjacent while successful parsing does not validate a model.

The thread does **not** establish that every `strtod` implementation has the same speed, that locale never matters, that checking only `endptr` is sufficient, that `errno` alone establishes whether parsing succeeded, that every underflow returns exactly zero, that every overflow representation is identical, that NaN payload text is portable, that the fastest parser is necessarily complete, that all decimal inputs are equally difficult, that a successful conversion consumed the entire token, that JSON numbers and binary64 values are interchangeable, or that correctly parsing a few configuration values validates a machine-learning model.

## Disclaimers

This repository does not validate a model, dataset, tokenizer, feature pipeline, normalization scheme, quantization format, calibration procedure, threshold choice, numerical-analysis library, JSON parser, CSV parser, libc, compiler, operating system, deployment platform, security boundary, or production inference system.

Text parsing does not validate ML semantics. Parsed probabilities are syntactically valid only, not statistically calibrated. Parsed thresholds are not statistically justified. Parsed temperatures do not improve generation. Parsed quantization scales do not match any production quantization format (llama.cpp, ggml, gguf, q8, q4, etc.). Four parsed values do not constitute a tensor.

No benchmark is performed. No throughput comparison. No parser ranking.

Locale is fixed to C for deterministic cases. Changing locale concurrently is unsafe. `setlocale` is not suitable for library code in arbitrary multithreaded processes.

NaN payload text is not portable. Infinity and NaN are not valid JSON numbers. Not every decimal fraction is exactly representable as binary64. One local `snprintf`/`strtod` round trip does not prove cross-platform interoperability.

See `no_global_parse_or_ml_validity_claim_marker` case for full list of non-claims.

## Build

Linux / macOS / zsh:
```sh
./run_lab.sh    # bash/sh
./run_lab.zsh   # zsh
```

Windows:
```bat
run_lab.bat
```

Or manually:
```sh
$ZIG_BIN cc -std=c11 -O2 -Wall -Wextra -Wpedantic -Werror parse_lab.c -o parse_lab
python3 run_lab.py
python3 -m unittest -v
```

Zig compiler resolved via `$ZIG_BIN`, `command -v zig`, `~/.local/bin/zig`, `~/bin/zig`.
