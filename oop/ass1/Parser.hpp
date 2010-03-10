#ifndef XMLP_PARSER_HPP
#define XMLP_PARSER_HPP

#include <string>
#include <assert.h>
#include <string.h>
#include <iostream>
#include <stack>
#include <map>
#include <stdlib.h>

#include "Element.hpp"
#include "Node.hpp"
#include "String.hpp"
#include "Text.hpp"

namespace xml {

class Parser {
     
     public:
          Parser() { pool = new Pool(); };
          Element * parse(const char *, int size);
          ~Parser() { delete pool; };
     private:
          Pool * pool;
};
}

#endif
