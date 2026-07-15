#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include <math.h>
#include <errno.h>
#include <stdio.h>
#include <locale.h>

static void print_esc(const char *s) {
    putchar('"');
    while (*s) {
        unsigned char c = (unsigned char)*s++;
        if (c == '"' || c == '\\') { putchar('\\'); putchar(c); }
        else if (c == '\n') { printf("\\n"); }
        else if (c == '\r') { printf("\\r"); }
        else if (c == '\t') { printf("\\t"); }
        else if (c < 32) { printf("\\u%04x", c); }
        else putchar(c);
    }
    putchar('"');
}

static void bytes_hex(const unsigned char *b, size_t n) {
    for (size_t i=0;i<n;i++) printf("%02x", b[i]);
}

int main(void) {
    char *orig_locale = setlocale(LC_NUMERIC, NULL);
    char orig_copy[128] = {0};
    int orig_copied = 0;
    if (orig_locale) { strncpy(orig_copy, orig_locale, sizeof(orig_copy)-1); orig_copied=1; }
    int set_c_ok = setlocale(LC_NUMERIC, "C") != NULL;
    struct lconv *lc = localeconv();
    const char *radix = lc ? lc->decimal_point : "?";

    printf("{\n");

    /* representation markers */
    printf("\"char_bit\":%d,\n", CHAR_BIT);
    printf("\"sizeof_char\":%zu,\n", sizeof(char));
    printf("\"sizeof_float\":%zu,\n", sizeof(float));
    printf("\"sizeof_double\":%zu,\n", sizeof(double));
    printf("\"sizeof_long_double\":%zu,\n", sizeof(long double));
    printf("\"sizeof_void_p\":%zu,\n", sizeof(void*));
    printf("\"sizeof_size_t\":%zu,\n", sizeof(size_t));
    printf("\"flt_radix\":%d,\n", FLT_RADIX);
    printf("\"flt_mant_dig\":%d,\n", FLT_MANT_DIG);
    printf("\"dbl_mant_dig\":%d,\n", DBL_MANT_DIG);
    printf("\"ldbl_mant_dig\":%d,\n", LDBL_MANT_DIG);
    printf("\"decimal_dig\":%d,\n", DECIMAL_DIG);
#ifdef DBL_DECIMAL_DIG
    printf("\"dbl_decimal_dig\":%d,\n", DBL_DECIMAL_DIG);
#else
    printf("\"dbl_decimal_dig\":null,\n");
#endif
#ifdef __STDC_VERSION__
    printf("\"stdc_version\":%ld,\n", (long)__STDC_VERSION__);
#else
    printf("\"stdc_version\":null,\n");
#endif
    printf("\"dbl_min\":%.17e,\n", DBL_MIN);
    printf("\"dbl_max\":%.17e,\n", DBL_MAX);
    printf("\"locale_radix\":");
    print_esc(radix ? radix : "?");
    printf(",\n");
    printf("\"set_c_ok\":%s,\n", set_c_ok ? "true" : "false");

    /* no_conversion */
    {
        const char *s = "not-a-number";
        errno = 0;
        char *end = NULL;
        double v = strtod(s, &end);
        size_t off = end ? (size_t)(end - s) : 0;
        printf("\"no_conversion\":{\"input\":");
        print_esc(s);
        printf(",\"len\":%zu,\"value\":%.17g,\"errno\":%d,\"endptr_offset\":%zu,\"consumed\":%s},\n",
            strlen(s), v, errno, off, off>0 ? "true":"false");
    }
    /* whitespace_and_sign */
    {
        const char *s = " \t-12.5";
        errno = 0;
        char *end = NULL;
        double v = strtod(s, &end);
        size_t off = end ? (size_t)(end - s) : 0;
        printf("\"whitespace_and_sign\":{\"input\":");
        print_esc(s);
        printf(",\"value\":%.17g,\"endptr_offset\":%zu,\"errno\":%d,\"equals_minus_12_5\":%s},\n",
            v, off, errno, v == -12.5 ? "true":"false");
    }
    /* decimal_fraction */
    {
        const char *s = "0.125";
        errno = 0; char *end=NULL; double v=strtod(s,&end);
        size_t off = end ? (size_t)(end - s):0;
        printf("\"decimal_fraction\":{\"input\":");
        print_esc(s);
        printf(",\"value\":%.17g,\"endptr_offset\":%zu,\"errno\":%d,\"exact\":%s},\n",
            v, off, errno, v == 0.125 ? "true":"false");
    }
    /* decimal_exponent */
    {
        const char *s = "6.25e-2";
        errno = 0; char *end=NULL; double v=strtod(s,&end);
        size_t off = end ? (size_t)(end-s):0;
        printf("\"decimal_exponent\":{\"input\":");
        print_esc(s);
        printf(",\"value\":%.17g,\"endptr_offset\":%zu,\"errno\":%d,\"exact\":%s},\n",
            v, off, errno, v == 0.0625 ? "true":"false");
    }
    /* trailing_junk */
    {
        const char *s = "3.5ms";
        errno=0; char *end=NULL; double v=strtod(s,&end);
        size_t off = end ? (size_t)(end-s):0;
        const char *suffix = s + off;
        printf("\"trailing_junk\":{\"input\":");
        print_esc(s);
        printf(",\"value\":%.17g,\"endptr_offset\":%zu,\"suffix\":", v, off);
        print_esc(suffix);
        printf(",\"errno\":%d},\n", errno);
    }
    /* overflow */
    {
        const char *s = "1e9999";
        errno=0; char *end=NULL; double v=strtod(s,&end);
        size_t off = end ? (size_t)(end-s):0;
        printf("\"overflow\":{\"input\":");
        print_esc(s);
        printf(",\"value\":%.17g,\"endptr_offset\":%zu,\"errno\":%d,\"isinf\":%s,\"signbit\":%d},\n",
            v, off, errno, isinf(v)?"true":"false", signbit(v));
    }
    /* underflow */
    {
        const char *s = "1e-9999";
        errno=0; char *end=NULL; double v=strtod(s,&end);
        size_t off = end ? (size_t)(end-s):0;
        printf("\"underflow\":{\"input\":");
        print_esc(s);
        printf(",\"value\":%.17g,\"endptr_offset\":%zu,\"errno\":%d,\"is_zero\":%s,\"isfinite\":%s},\n",
            v, off, errno, v==0.0?"true":"false", isfinite(v)?"true":"false");
    }
    /* negative_zero */
    {
        const char *s = "-0.0";
        errno=0; char *end=NULL; double v=strtod(s,&end);
        size_t off = end ? (size_t)(end-s):0;
        unsigned char bytes[sizeof(double)];
        memcpy(bytes, &v, sizeof(double));
        printf("\"negative_zero\":{\"input\":");
        print_esc(s);
        printf(",\"value\":%.17g,\"endptr_offset\":%zu,\"signbit\":%d,\"bytes_hex\":\"", v, off, signbit(v));
        bytes_hex(bytes, sizeof(double));
        printf("\",\"errno\":%d},\n", errno);
    }
    /* infinity */
    {
        const char *s = "INF";
        errno=0; char *end=NULL; double v=strtod(s,&end);
        size_t off = end ? (size_t)(end-s):0;
        printf("\"infinity_token\":{\"input\":");
        print_esc(s);
        printf(",\"isinf\":%s,\"signbit\":%d,\"endptr_offset\":%zu,\"errno\":%d},\n",
            isinf(v)?"true":"false", signbit(v), off, errno);
    }
    /* nan */
    {
        const char *s = "NAN";
        errno=0; char *end=NULL; double v=strtod(s,&end);
        size_t off = end ? (size_t)(end-s):0;
        printf("\"nan_token\":{\"input\":");
        print_esc(s);
        printf(",\"isnan\":%s,\"endptr_offset\":%zu,\"errno\":%d},\n",
            isnan(v)?"true":"false", off, errno);
    }
    /* hex */
    {
        const char *s = "0x1.8p+1";
        errno=0; char *end=NULL; double v=strtod(s,&end);
        size_t off = end ? (size_t)(end-s):0;
        printf("\"hex_float\":{\"input\":");
        print_esc(s);
        printf(",\"value\":%.17g,\"endptr_offset\":%zu,\"errno\":%d,\"equals_3\":%s},\n",
            v, off, errno, v == 3.0 ? "true":"false");
    }
    /* c_locale_radix */
    {
        const char *s1 = "1.5";
        const char *s2 = "1,5";
        errno=0; char *e1=NULL; double v1=strtod(s1,&e1);
        size_t o1 = e1 ? (size_t)(e1-s1):0;
        int err1 = errno;
        errno=0; char *e2=NULL; double v2=strtod(s2,&e2);
        size_t o2 = e2 ? (size_t)(e2-s2):0;
        int err2 = errno;
        printf("\"c_locale_radix\":{\"dot_input\":");
        print_esc(s1);
        printf(",\"dot_value\":%.17g,\"dot_offset\":%zu,\"dot_errno\":%d,", v1, o1, err1);
        printf("\"comma_input\":");
        print_esc(s2);
        printf(",\"comma_value\":%.17g,\"comma_offset\":%zu,\"comma_errno\":%d},\n", v2, o2, err2);
    }
    /* snprintf_roundtrip */
    {
        double vals[] = {0.1, -0.0, 1.5, DBL_MIN, 9007199254740991.0};
        int prec =
#ifdef DBL_DECIMAL_DIG
            DBL_DECIMAL_DIG
#else
            DECIMAL_DIG
#endif
        ;
        printf("\"snprintf_roundtrip\":{\"precision\":%d,\"items\":[", prec);
        for (int i=0;i<5;i++) {
            double src = vals[i];
            char buf[64];
            int n = snprintf(buf, sizeof(buf), "%.*g", prec, src);
            errno=0; char *end=NULL; double parsed = strtod(buf, &end);
            size_t off = end ? (size_t)(end - buf) : 0;
            unsigned char sb[sizeof(double)], pb[sizeof(double)];
            memcpy(sb, &src, sizeof(double));
            memcpy(pb, &parsed, sizeof(double));
            int equal_bytes = memcmp(sb, pb, sizeof(double)) == 0;
            if (i) printf(",");
            printf("{\"src\":%.17g,\"signbit_src\":%d,\"text\":", src, signbit(src));
            print_esc(buf);
            printf(",\"snprintf_ret\":%d,\"parsed\":%.17g,\"signbit_parsed\":%d,\"offset\":%zu,\"bytes_equal\":%s}",
                n, parsed, signbit(parsed), off, equal_bytes?"true":"false");
        }
        printf("]},\n");
    }
    /* vector_token */
    {
        const char *s = "0.25,-1.5,2.0,0.0";
        double out[4] = {0};
        size_t starts[4] = {0};
        size_t ends[4] = {0};
        size_t seps[3] = {0};
        int count = 0;
        const char *p = s;
        while (count < 4) {
            starts[count] = (size_t)(p - s);
            errno=0; char *end=NULL; double v = strtod(p, &end);
            if (end == p) break;
            out[count] = v;
            ends[count] = (size_t)(end - s);
            count++;
            p = end;
            if (*p == ',') { if (count-1 < 3) seps[count-1] = (size_t)(p - s); p++; }
            else break;
        }
        int consumed_all = (*p == '\0');
        printf("\"vector_token\":{\"input\":");
        print_esc(s);
        printf(",\"values\":[%.17g,%.17g,%.17g,%.17g],\"starts\":[%zu,%zu,%zu,%zu],\"ends\":[%zu,%zu,%zu,%zu],\"seps\":[%zu,%zu,%zu],\"count\":%d,\"consumed_all\":%s},\n",
            out[0],out[1],out[2],out[3],
            starts[0],starts[1],starts[2],starts[3],
            ends[0],ends[1],ends[2],ends[3],
            seps[0],seps[1],seps[2],
            count, consumed_all?"true":"false");
    }
    /* alternate locale probe */
    {
        const char *cands[] = {"de_DE.UTF-8","de_DE.utf8","fr_FR.UTF-8","fr_FR.utf8", NULL};
        const char *found = NULL;
        const char *found_radix = NULL;
        for (int i=0; cands[i]; i++) {
            if (setlocale(LC_NUMERIC, cands[i])) {
                struct lconv *l = localeconv();
                found = cands[i];
                found_radix = l ? l->decimal_point : "?";
                break;
            }
        }
        setlocale(LC_NUMERIC, "C");
        printf("\"alt_locale\":{\"found\":");
        if (found) { print_esc(found); } else printf("null");
        printf(",\"radix\":");
        if (found_radix) { print_esc(found_radix); } else printf("null");
        printf("},\n");
    }

    /* restore */
    int restore_ok = 0;
    if (orig_copied) { restore_ok = setlocale(LC_NUMERIC, orig_copy) != NULL; }
    printf("\"restore_ok\":%s\n", restore_ok ? "true" : "false");
    printf("}\n");
    return 0;
}
