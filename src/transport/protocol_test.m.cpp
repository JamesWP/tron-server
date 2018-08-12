
#include <stdio.h>
#include <protocol.h>

int main(int argc, char **argv)
{
    printf("testing\n");
    for (int i = 1; i < argc; ++i) {
        std::string input{argv[i]};
        auto parsed = protocol::parse_client_input(input);
        std::visit(protocol::input_print_visitor{}, parsed);
    }
    return 0;
}
