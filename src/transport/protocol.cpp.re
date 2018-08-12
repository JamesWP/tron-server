// vim: set filetype=cpp :
#include "protocol.h" 
#include <stdio.h>

namespace protocol {

std::ostream& operator<<(std::ostream& out, const move&) { return (out << "move"); }
std::ostream& operator<<(std::ostream& out, const chat& ch) { return (out <<
"chat{" << ch.message()) << "}"; }
std::ostream& operator<<(std::ostream& out, const hello&) { return (out << "hello"); }
std::ostream& operator<<(std::ostream& out, const quit&) { return (out << "quit"); }
std::ostream& operator<<(std::ostream& out, const err&) { return (out << "err"); }

client_input parse_client_input(const std::string& input)
{

    const char *YYCURSOR = input.c_str();

    const char *text_start,*text_end;
    /*!stags:re2c format = 'const char *@@;'; */

    const char *YYMARKER;
    /*!re2c
        re2c:define:YYCTYPE = char;
        re2c:yyfill:enable = 0;
        end = '\x00';
  
        text = [^\x00]*;

        colour = [0-9]+ " " [0-9]+ " " [0-9]+;
        name   = text; 

        move_args = "L" | "R" | "N";
        chat_args = @text_start text @text_end;
        hello_args = colour " " text;

        move   = "MOVE" " " move_args;
        chat   = "CHAT" " " chat_args;
        hello  = "HELLO" " " hello_args;
        quit   = "QUIT";

        move  end { return client_input{move{}}; }
        chat  end { std::string_view content{
                      text_start,
                      static_cast<long unsigned int>(text_end - text_start)};
                    return client_input{chat{content}}; }
        hello end { return client_input{hello{}}; }
        quit  end { return client_input{quit{}}; }

        *         { return client_input{err{}}; }
    */
}
}
