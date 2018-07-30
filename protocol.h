#pragma once

#include <string>
#include <variant>
#include <iostream>

namespace protocol {

struct colour   { unsigned char r,g,b; };

class move 
{ 
  enum {Left, Right, None};

public:
  friend std::ostream& operator<<(std::ostream&, const move&);
};

class chat 
{
  std::string message; 

public:
  friend std::ostream& operator<<(std::ostream&, const chat&);
};

class hello     
{ 
  std::string name; colour col;

public:
  friend std::ostream& operator<<(std::ostream& out, const hello&);
};

class quit
{
public:
  friend std::ostream& operator<<(std::ostream& out, const quit&);
};

class err       
{
public:
  friend std::ostream& operator<<(std::ostream& out, const err&);
};

class input_print_visitor {
  public:
    void operator()(const move& m)  { std::cout << m << '\n'; }
    void operator()(const chat& c)  { std::cout << c << '\n'; }
    void operator()(const hello& h) { std::cout << h << '\n'; }
    void operator()(const quit& q)  { std::cout << q << '\n'; }
    void operator()(const err& e)   { std::cout << e << '\n'; }
};

using client_input = std::variant<move, chat, hello, quit, err>;

client_input parse_client_input(const std::string& input);

}; // protocol
