# A compilation of coding techniques from reading professional source code

---
## Parsing
> a good parsing example
```
/*
 * Curl_compareheader()
 *
 * Returns TRUE if 'headerline' contains the 'header' with given 'content'.
 * Pass headers WITH the colon.
 */
bool Curl_compareheader(const char  *headerline, /* line to check */
                        const char  *header,     /* header keyword _with_ colon */
                        const size_t hlen,       /* len of the keyword in bytes */
                        const char  *content,    /* content string to find */
                        const size_t clen)       /* len of the content in bytes */
{
    /* RFC2616, section 4.2 says: "Each header field consists of a name followed
     * by a colon (":") and the field value. Field names are case-insensitive.
     * The field value MAY be preceded by any amount of LWS, though a single SP
     * is preferred." */

    size_t      len;
    const char *start;
    const char *end;
    DEBUGASSERT(hlen);
    DEBUGASSERT(clen);
    DEBUGASSERT(header);
    DEBUGASSERT(content);

    if (!strncasecompare(headerline, header, hlen))
        return FALSE; /* doesn't start with header */

    /* pass the header */
    start = &headerline[hlen];

    /* pass all whitespace */
    while (*start && ISSPACE(*start))
        start++;

    /* find the end of the header line */
    end = strchr(start, '\r'); /* lines end with CRLF */
    if (!end)
    {
        /* in case there's a non-standard compliant line here */
        end = strchr(start, '\n');

        if (!end)
            /* hm, there's no line ending here, use the zero byte! */
            end = strchr(start, '\0');
    }

    len = end - start; /* length of the content part of the input line */

    /* find the content string in the rest of the line */
    for (; len >= clen; len--, start++)
    {
        if (strncasecompare(start, content, clen))
            return TRUE; /* match! */
    }

    return FALSE; /* no match */
}
```

---
## Cool utility
> Check out the strcase file


---
## Error handling

### error code enum
> functions can return this enum
```
typedef enum {
	PNAME_OK = 0,
	PNAME_E1,	/* error description */
	PNAME_E2,	/* error description */
	.....
	PNAME_LAST	/* never use! only a counter */
} PNAMECode;
```
[example](./CURLCode.cpp)

### debug assert
> assert macro used in debug builds
```
/*
 * Macro used to include assertion code only in debug builds.
 */

#undef DEBUGASSERT
#if defined(DEBUGBUILD)
#define DEBUGASSERT(x) assert(x)
#else
#define DEBUGASSERT(x) do { } while(0)
#endif
```
