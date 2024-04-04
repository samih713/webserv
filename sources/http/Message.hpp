#include "../../includes/webserv.hpp"

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

/* ------------------------------ const strings ----------------------------- */
static const string CRLF = "\r\n";
static const string NEWLINE = "\n";
static const string SP = " ";
static const string HTTP_VER = "HTTP/1.1";

/* ------------------------------- METHOD ENUM ------------------------------ */
#define METHOD_ENUMS                                                                     \
    X(GET)                                                                               \
    X(HEAD)                                                                              \
    X(POST)                                                                              \
    X(PUT)                                                                               \
    X(DELETE)                                                                            \
    X(CONNECT)                                                                           \
    X(OPTIONS)                                                                           \
    X(TRACE)                                                                             \
    X(PATCH)

// X macro to define the enum
#define X(a) a,
enum METHOD
{
    METHOD_ENUMS
};
#undef X

/* ---------------------------- STATUS_CODE ENUM ---------------------------- */
#define STATUS_CODE_LIST                                                                 \
    X(CONTINUE)                                                                          \
    X(SWITCHING_PROTOCOLS)                                                               \
    X(PROCESSING)                                                                        \
    X(EARLY_HINTS)                                                                       \
    X(OK)                                                                                \
    X(CREATED)                                                                           \
    X(ACCEPTED)                                                                          \
    X(NON_AUTHORITATIVE_INFORMATION)                                                     \
    X(NO_CONTENT)                                                                        \
    X(RESET_CONTENT)                                                                     \
    X(PARTIAL_CONTENT)                                                                   \
    X(MULTI_STATUS)                                                                      \
    X(ALREADY_REPORTED)                                                                  \
    X(IM_USED)                                                                           \
    X(MULTIPLE_CHOICES)                                                                  \
    X(MOVED_PERMANENTLY)                                                                 \
    X(FOUND)                                                                             \
    X(SEE_OTHER)                                                                         \
    X(NOT_MODIFIED)                                                                      \
    X(USE_PROXY)                                                                         \
    X(TEMPORARY_REDIRECT)                                                                \
    X(PERMANENT_REDIRECT)                                                                \
    X(BAD_REQUEST)                                                                       \
    X(UNAUTHORIZED)                                                                      \
    X(PAYMENT_REQUIRED)                                                                  \
    X(FORBIDDEN)                                                                         \
    X(NOT_FOUND)                                                                         \
    X(METHOD_NOT_ALLOWED)                                                                \
    X(NOT_ACCEPTABLE)                                                                    \
    X(PROXY_AUTHENTICATION_REQUIRED)                                                     \
    X(REQUEST_TIMEOUT)                                                                   \
    X(CONFLICT)                                                                          \
    X(GONE)                                                                              \
    X(LENGTH_REQUIRED)                                                                   \
    X(PRECONDITION_FAILED)                                                               \
    X(PAYLOAD_TOO_LARGE)                                                                 \
    X(URI_TOO_LONG)                                                                      \
    X(UNSUPPORTED_MEDIA_TYPE)                                                            \
    X(RANGE_NOT_SATISFIABLE)                                                             \
    X(EXPECTATION_FAILED)                                                                \
    X(IM_A_TEAPOT)                                                                       \
    X(MISDIRECTED_REQUEST)                                                               \
    X(UNPROCESSABLE_ENTITY)                                                              \
    X(LOCKED)                                                                            \
    X(FAILED_DEPENDENCY)                                                                 \
    X(TOO_EARLY)                                                                         \
    X(UPGRADE_REQUIRED)                                                                  \
    X(PRECONDITION_REQUIRED)                                                             \
    X(TOO_MANY_REQUESTS)                                                                 \
    X(REQUEST_HEADER_FIELDS_TOO_LARGE)                                                   \
    X(UNAVAILABLE_FOR_LEGAL_REASONS)                                                     \
    X(INTERNAL_SERVER_ERROR)                                                             \
    X(NOT_IMPLEMENTED)                                                                   \
    X(BAD_GATEWAY)                                                                       \
    X(SERVICE_UNAVAILABLE)                                                               \
    X(GATEWAY_TIMEOUT)                                                                   \
    X(HTTP_VERSION_NOT_SUPPORTED)                                                        \
    X(VARIANT_ALSO_NEGOTIATES)                                                           \
    X(INSUFFICIENT_STORAGE)                                                              \
    X(LOOP_DETECTED)                                                                     \
    X(NOT_EXTENDED)

enum STATUS_CODE
{
    CONTINUE = 100,
    SWITCHING_PROTOCOLS = 101,
    PROCESSING = 102,
    EARLY_HINTS = 103,
    OK = 200,
    CREATED = 201,
    ACCEPTED = 202,
    NON_AUTHORITATIVE_INFORMATION = 203,
    NO_CONTENT = 204,
    RESET_CONTENT = 205,
    PARTIAL_CONTENT = 206,
    MULTI_STATUS = 207,
    ALREADY_REPORTED = 208,
    IM_USED = 226,
    MULTIPLE_CHOICES = 300,
    MOVED_PERMANENTLY = 301,
    FOUND = 302,
    SEE_OTHER = 303,
    NOT_MODIFIED = 304,
    USE_PROXY = 305,
    TEMPORARY_REDIRECT = 307,
    PERMANENT_REDIRECT = 308,
    BAD_REQUEST = 400,
    UNAUTHORIZED = 401,
    PAYMENT_REQUIRED = 402,
    FORBIDDEN = 403,
    NOT_FOUND = 404,
    METHOD_NOT_ALLOWED = 405,
    NOT_ACCEPTABLE = 406,
    PROXY_AUTHENTICATION_REQUIRED = 407,
    REQUEST_TIMEOUT = 408,
    CONFLICT = 409,
    GONE = 410,
    LENGTH_REQUIRED = 411,
    PRECONDITION_FAILED = 412,
    PAYLOAD_TOO_LARGE = 413,
    URI_TOO_LONG = 414,
    UNSUPPORTED_MEDIA_TYPE = 415,
    RANGE_NOT_SATISFIABLE = 416,
    EXPECTATION_FAILED = 417,
    IM_A_TEAPOT = 418,
    MISDIRECTED_REQUEST = 421,
    UNPROCESSABLE_ENTITY = 422,
    LOCKED = 423,
    FAILED_DEPENDENCY = 424,
    TOO_EARLY = 425,
    UPGRADE_REQUIRED = 426,
    PRECONDITION_REQUIRED = 428,
    TOO_MANY_REQUESTS = 429,
    REQUEST_HEADER_FIELDS_TOO_LARGE = 431,
    UNAVAILABLE_FOR_LEGAL_REASONS = 451,
    INTERNAL_SERVER_ERROR = 500,
    NOT_IMPLEMENTED = 501,
    BAD_GATEWAY = 502,
    SERVICE_UNAVAILABLE = 503,
    GATEWAY_TIMEOUT = 504,
    HTTP_VERSION_NOT_SUPPORTED = 505,
    VARIANT_ALSO_NEGOTIATES = 506,
    INSUFFICIENT_STORAGE = 507,
    LOOP_DETECTED = 508,
    NOT_EXTENDED
};

/* --------------------------- Field Name Registry --------------------------- */
static const std::pair<std::string, int> init_values[] = {
    std::make_pair("A-IM", 0),
    std::make_pair("Accept", 1),
    std::make_pair("Accept-Additions", 2),
    std::make_pair("Accept-CH", 3),
    std::make_pair("Accept-Charset", 4),
    std::make_pair("Accept-Datetime", 5),
    std::make_pair("Accept-Encoding", 6),
    std::make_pair("Accept-Features", 7),
    std::make_pair("Accept-Language", 8),
    std::make_pair("Accept-Patch", 9),
    std::make_pair("Accept-Post", 10),
    std::make_pair("Accept-Ranges", 11),
    std::make_pair("Accept-Signature", 12),
    std::make_pair("Access-Control", 13),
    std::make_pair("Access-Control-Allow-Credentials", 14),
    std::make_pair("Access-Control-Allow-Headers", 15),
    std::make_pair("Access-Control-Allow-Methods", 16),
    std::make_pair("Access-Control-Allow-Origin", 17),
    std::make_pair("Access-Control-Expose-Headers", 18),
    std::make_pair("Access-Control-Max-Age", 19),
    std::make_pair("Access-Control-Request-Headers", 20),
    std::make_pair("Access-Control-Request-Method", 21),
    std::make_pair("Age", 22),
    std::make_pair("Allow", 23),
    std::make_pair("ALPN", 24),
    std::make_pair("Alt-Svc", 25),
    std::make_pair("Alt-Used", 26),
    std::make_pair("Alternates", 27),
    std::make_pair("AMP-Cache-Transform", 28),
    std::make_pair("Apply-To-Redirect-Ref", 29),
    std::make_pair("Authentication-Control", 30),
    std::make_pair("Authentication-Info", 31),
    std::make_pair("Authorization", 32),
    std::make_pair("C-Ext", 33),
    std::make_pair("C-Man", 34),
    std::make_pair("C-Opt", 35),
    std::make_pair("C-PEP", 36),
    std::make_pair("C-PEP-Info", 37),
    std::make_pair("Cache-Control", 38),
    std::make_pair("Cache-Status", 39),
    std::make_pair("Cal-Managed-ID", 40),
    std::make_pair("CalDAV-Timezones", 41),
    std::make_pair("Capsule-Protocol", 42),
    std::make_pair("CDN-Cache-Control", 43),
    std::make_pair("CDN-Loop", 44),
    std::make_pair("Cert-Not-After", 45),
    std::make_pair("Cert-Not-Before", 46),
    std::make_pair("Clear-Site-Data", 47),
    std::make_pair("Client-Cert", 48),
    std::make_pair("Client-Cert-Chain", 49),
    std::make_pair("Close", 50),
    std::make_pair("Configuration-Context", 51),
    std::make_pair("Connection", 52),
    std::make_pair("Content-Base", 53),
    std::make_pair("Content-Digest", 54),
    std::make_pair("Content-Disposition", 55),
    std::make_pair("Content-Encoding", 56),
    std::make_pair("Content-ID", 57),
    std::make_pair("Content-Language", 58),
    std::make_pair("Content-Length", 59),
    std::make_pair("Content-Location", 60),
    std::make_pair("Content-MD5", 61),
    std::make_pair("Content-Range", 62),
    std::make_pair("Content-Script-Type", 63),
    std::make_pair("Content-Security-Policy", 64),
    std::make_pair("Content-Security-Policy-Report-Only", 65),
    std::make_pair("Content-Style-Type", 66),
    std::make_pair("Content-Type", 67),
    std::make_pair("Content-Version", 68),
    std::make_pair("Cookie", 69),
    std::make_pair("Cookie2", 70),
    std::make_pair("Cross-Origin-Embedder-Policy", 71),
    std::make_pair("Cross-Origin-Embedder-Policy-Report-Only", 72),
    std::make_pair("Cross-Origin-Opener-Policy", 73),
    std::make_pair("Cross-Origin-Opener-Policy-Report-Only", 74),
    std::make_pair("Cross-Origin-Resource-Policy", 75),
    std::make_pair("DASL", 76),
    std::make_pair("Date", 77),
    std::make_pair("DAV", 78),
    std::make_pair("Default-Style", 79),
    std::make_pair("Delta-Base", 80),
    std::make_pair("Depth", 81),
    std::make_pair("Derived-From", 82),
    std::make_pair("Destination", 83),
    std::make_pair("Differential-ID", 84),
    std::make_pair("Digest", 85),
    std::make_pair("DPoP", 86),
    std::make_pair("DPoP-Nonce", 87),
    std::make_pair("Early-Data", 88),
    std::make_pair("EDIINT-Features", 89),
    std::make_pair("ETag", 90),
    std::make_pair("Expect", 91),
    std::make_pair("Expect-CT", 92),
    std::make_pair("Expires", 93),
    std::make_pair("Ext", 94),
    std::make_pair("Forwarded", 95),
    std::make_pair("From", 96),
    std::make_pair("GetProfile", 97),
    std::make_pair("Hobareg", 98),
    std::make_pair("Host", 99),
    std::make_pair("HTTP2-Settings", 100),
    std::make_pair("If", 101),
    std::make_pair("If-Match", 102),
    std::make_pair("If-Modified-Since", 103),
    std::make_pair("If-None-Match", 104),
    std::make_pair("If-Range", 105),
    std::make_pair("If-Schedule-Tag-Match", 106),
    std::make_pair("If-Unmodified-Since", 107),
    std::make_pair("IM", 108),
    std::make_pair("Include-Referred-Token-Binding-ID", 109),
    std::make_pair("Isolation", 110),
    std::make_pair("Keep-Alive", 111),
    std::make_pair("Label", 112),
    std::make_pair("Last-Event-ID", 113),
    std::make_pair("Last-Modified", 114),
    std::make_pair("Link", 115),
    std::make_pair("Location", 116),
    std::make_pair("Lock-Token", 117),
    std::make_pair("Man", 118),
    std::make_pair("Max-Forwards", 119),
    std::make_pair("Memento-Datetime", 120),
    std::make_pair("Meter", 121),
    std::make_pair("Method-Check", 122),
    std::make_pair("Method-Check-Expires", 123),
    std::make_pair("MIME-Version", 124),
    std::make_pair("Negotiate", 125),
    std::make_pair("NEL", 126),
    std::make_pair("OData-EntityId", 127),
    std::make_pair("OData-Isolation", 128),
    std::make_pair("OData-MaxVersion", 129),
    std::make_pair("OData-Version", 130),
    std::make_pair("Opt", 131),
    std::make_pair("Optional-WWW-Authenticate", 132),
    std::make_pair("Ordering-Type", 133),
    std::make_pair("Origin", 134),
    std::make_pair("Origin-Agent-Cluster", 135),
    std::make_pair("OSCORE", 136),
    std::make_pair("OSLC-Core-Version", 137),
    std::make_pair("Overwrite", 138),
    std::make_pair("P3P", 139),
    std::make_pair("PEP", 140),
    std::make_pair("PEP-Info", 141),
    std::make_pair("Permissions-Policy", 142),
    std::make_pair("PICS-Label", 143),
    std::make_pair("Ping-From", 144),
    std::make_pair("Ping-To", 145),
    std::make_pair("Position", 146),
    std::make_pair("Pragma", 147),
    std::make_pair("Prefer", 148),
    std::make_pair("Preference-Applied", 149),
    std::make_pair("Priority", 150),
    std::make_pair("ProfileObject", 151),
    std::make_pair("Protocol", 152),
    std::make_pair("Protocol-Info", 153),
    std::make_pair("Protocol-Query", 154),
    std::make_pair("Protocol-Request", 155),
    std::make_pair("Proxy-Authenticate", 156),
    std::make_pair("Proxy-Authentication-Info", 157),
    std::make_pair("Proxy-Authorization", 158),
    std::make_pair("Proxy-Features", 159),
    std::make_pair("Proxy-Instruction", 160),
    std::make_pair("Proxy-Status", 161),
    std::make_pair("Public", 162),
    std::make_pair("Public-Key-Pins", 163),
    std::make_pair("Public-Key-Pins-Report-Only", 164),
    std::make_pair("Range", 165),
    std::make_pair("Redirect-Ref", 166),
    std::make_pair("Referer", 167),
    std::make_pair("Referer-Root", 168),
    std::make_pair("Refresh", 169),
    std::make_pair("Repeatability-Client-ID", 170),
    std::make_pair("Repeatability-First-Sent", 171),
    std::make_pair("Repeatability-Request-ID", 172),
    std::make_pair("Repeatability-Result", 173),
    std::make_pair("Replay-Nonce", 174),
    std::make_pair("Reporting-Endpoints", 175),
    std::make_pair("Repr-Digest", 176),
    std::make_pair("Retry-After", 177),
    std::make_pair("Safe", 178),
    std::make_pair("Schedule-Reply", 179),
    std::make_pair("Schedule-Tag", 180),
    std::make_pair("Sec-GPC", 181),
    std::make_pair("Sec-Purpose", 182),
    std::make_pair("Sec-Token-Binding", 183),
    std::make_pair("Sec-WebSocket-Accept", 184),
    std::make_pair("Sec-WebSocket-Extensions", 185),
    std::make_pair("Sec-WebSocket-Key", 186),
    std::make_pair("Sec-WebSocket-Protocol", 187),
    std::make_pair("Sec-WebSocket-Version", 188),
    std::make_pair("Security-Scheme", 189),
    std::make_pair("Server", 190),
    std::make_pair("Server-Timing", 191),
    std::make_pair("Set-Cookie", 192),
    std::make_pair("Set-Cookie2", 193),
    std::make_pair("SetProfile", 194),
    std::make_pair("Signature", 195),
    std::make_pair("Signature-Input", 196),
    std::make_pair("SLUG", 197),
    std::make_pair("SoapAction", 198),
    std::make_pair("Status-URI", 199),
    std::make_pair("Strict-Transport-Security", 200),
    std::make_pair("Sunset", 201),
    std::make_pair("Surrogate-Capability", 202),
    std::make_pair("Surrogate-Control", 203),
    std::make_pair("TCN", 204),
    std::make_pair("TE", 205),
    std::make_pair("Timeout", 206),
    std::make_pair("Timing-Allow-Origin", 207),
    std::make_pair("Topic", 208),
    std::make_pair("Traceparent", 209),
    std::make_pair("Tracestate", 210),
    std::make_pair("Trailer", 211),
    std::make_pair("Transfer-Encoding", 212),
    std::make_pair("TTL", 213),
    std::make_pair("Upgrade", 214),
    std::make_pair("Urgency", 215),
    std::make_pair("URI", 216),
    std::make_pair("User-Agent", 217),
    std::make_pair("Variant-Vary", 218),
    std::make_pair("Vary", 219),
    std::make_pair("Via", 220),
    std::make_pair("Want-Content-Digest", 221),
    std::make_pair("Want-Digest", 222),
    std::make_pair("Want-Repr-Digest", 223),
    std::make_pair("Warning", 224),
    std::make_pair("WWW-Authenticate", 225),
    std::make_pair("X-Content-Type-Options", 226),
    std::make_pair("X-Frame-Options", 227),
    std::make_pair("*", 228),
};

static const map<string, int> fieldNameList(init_values,
                                            init_values + sizeof(init_values) /
                                                              sizeof(init_values[0]));

// "GET /docs/tutorials/linux/shellscripts/howto.html HTTP/1.1\r\n" // request-line
static const std::string sample_request =
    "GET ../resources/sample_pages/index.html HTTP/1.1\r\n" // request-line
    "Host: Linode.com\r\n"                                  // headers fields
    "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.8) "
    "Gecko/20091102 Firefox/3.5.5 \r\n"
    "Accept-Encoding: gzip,deflate\r\n"
    "Accept-Charset: ISO-8859-1,utf-8\r\n"
    "Cache-Control: no-cache\r\n"
    "\r\n" //
    "Lorem ipsum dolor sit amet, officia excepteur ex fugiat reprehenderit\r\n"
    "enim labore culpa sint ad nisi Lorem pariatur mollit ex esse exercitation amet.\r\n"
    "Nisi anim cupidatat excepteur officia. Reprehenderit nostrud nostrud ipsum Lorem\r\n"
    "est aliquip amet voluptate voluptate dolor minim nulla est proident. Nostrud\r\n"
    "officia pariatur ut officia. Sit irure elit esse ea nulla sunt ex occaecat "
    "reprehenderit\r\n"
    "commodo officia dolor Lorem duis laboris cupidatat officia voluptate.\r\n"
    "Culpa proident adipisicing id nulla nisi laboris ex in Lorem sunt duis officia "
    "eiusmod.\r\n"
    "Aliqua reprehenderit commodo ex non excepteur duis sunt velit enim.\r\n"
    "Voluptate laboris sint cupidatat ullamco ut ea consectetur et est culpa et culpa "
    "duis.\r\n"
    "\r\n";

static const std::string sample_response =
    "HTTP/1.1 200 OK\r\n"                        // status line
    "Content-Type: text/html; charset=UTF-8\r\n" // headers
    "Content-Length: 123\r\n"
    "\r\n"
    "<html>\r\n"
    "<head><title>Sample Page</title></head>\r\n"
    "<body><h1>Hello, World!</h1><p>This is a sample webpage.</p></body>\r\n"
    "</html>\r\n";

#endif // MESSAGE_HPP
