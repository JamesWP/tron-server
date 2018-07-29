// vim: set filetype=cpp :
#include "protocol.h" 
#include <stdio.h>

namespace protocol {
output lex(const char *YYCURSOR)
{
    const char *YYMARKER;
    /*!re2c
        re2c:define:YYCTYPE = char;
        re2c:yyfill:enable = 0;
        
        end = '\x00';

        join_args = "";
        move_args = "";
        chat_args = "";
 
        command = "JOIN" join_args | "LEAVE" | "MOVE" move_args | "CHAT" chat_args; 

        command end { return VALID; }

        *       { return ERR; }
    */
}
}
