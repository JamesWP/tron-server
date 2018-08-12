
#include <protocol.h>

#include <gtest/gtest.h>
#include <stdio.h>

TEST(Protocol, parse_client_input)
{
    std::string input{"HELLO 1 1 1 username"}; 
    auto parsed = protocol::parse_client_input(input);
    ASSERT_TRUE(std::holds_alternative<protocol::hello>(parsed));
}

TEST(Protocol, parse_client_input_err)
{
    std::string input{"sdf;lsajdf"}; 
    auto parsed = protocol::parse_client_input(input);
    ASSERT_TRUE(std::holds_alternative<protocol::err>(parsed));
}
