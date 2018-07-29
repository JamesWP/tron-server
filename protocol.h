#pragma once

namespace protocol {
enum output { ERR, VALID };

output lex(const char *YYCURSOR);
}
