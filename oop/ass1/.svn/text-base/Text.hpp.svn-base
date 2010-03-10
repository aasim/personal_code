#ifndef XMLP_TEXT_HPP
#define XMLP_TEXT_HPP

#include "String.hpp"
#include "Node.hpp"
#include "Pool.hpp"
#include <assert.h>
namespace xml {

class Text : public Node {
    public:
         Text(const char * text_) : text(text_) { };
         Text(String text_) : text(text_) { };
         ~Text() { };
        // Returns the text content as a string.
        const String &str() const;
        // Convenience functions that internally will use RTTI.
        static bool is_Text(const Node *);
        static const Text *to_Text(const Node *);

        void * operator new (unsigned int size) { return Pool::get_mem(size, false); };
        void operator delete(void * mem) { Pool::free_mem(mem, false); };
    public:
        // You could add more to the public interface, if you wanted to.
    private:
        String text;
};
}

#endif
