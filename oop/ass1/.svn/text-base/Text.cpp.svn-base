#include "Text.hpp"

using namespace xml;

const String & Text::str() const {
     return text;
}

bool Text::is_Text(const Node *node) {
     const Text * text = dynamic_cast<const Text *> (node);
     if(text == NULL) return false;
     else return true;
}

const Text * Text::to_Text(const Node *node) {
     assert(Text::is_Text(node));
     const Text * text = dynamic_cast<const Text *> (node);
     return text;
}
