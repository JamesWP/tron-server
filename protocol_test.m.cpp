
#include <stdio.h>
#include <protocol.h>

int main(int argc, char **argv)
{
    printf("testing\n");
    for (int i = 1; i < argc; ++i) {
        switch (protocol::lex(argv[i])) {
            case protocol::VALID: printf("valid\n"); break;
            case protocol::ERR: printf("error\n"); break;
        }
    }
    return 0;
}
