# HN Thread 24436158

Title: Parsing floats in C++: benchmarking strtod vs. from_chars
URL: https://news.ycombinator.com/item?id=24436158

Comments captured:
- jeffbee: absl::from_chars, >2x faster than strtod
- osolo: Stephan Lavavej CppCon 2019 talk on floating-point char conversion
- SAI_Peregrinus: strtod example incomplete, forgets errno / HUGE_VAL
- jwilk: underflow may return 0, C++ example also missing checks
- thangalin: float-to-string Ryū algorithm
- SomeoneFromCA: Locale not a big deal, most use C locale
- wolf550e: optimized function faster than compiler bundled
- Someone: fast parser may omit NaN/inf/denormal/round-trip
- titzer: hard cases = big exponents, long significands, large-integer arithmetic
- alfalfasprout: JSON serialization, fixed text interfaces; JSON grammar != IEEE754 values
- dundarious: text numerical protocols still matter (FIX)

Tool: hackernews get-item --id 24436158
