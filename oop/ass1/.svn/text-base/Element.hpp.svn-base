#ifndef XMLP_ELEMENT_HPP
#define XMLP_ELEMENT_HPP

#include "String.hpp"
#include "Node.hpp"
#include "Visitor.hpp"
#include "Text.hpp"
#include "Pool.hpp"
#include <vector>
#include <assert.h>

namespace xml {

class Element : public Node {
     public:
          // Returns the name of the element.  This is just
          // the part after the colon.  If there is no colon, then
          // it is the whole thing, of course.
          const String &name() const;
          // Returns the full namespace URI (not the prefix) of the element.
          // If the element is not in any namespace, this should simply
          // return an empty String.  Note that 'namespace' is a reserved
          // keyword, so we don't fully spell it out.
          const String &nmspace() const;
          // Returns the number of children nodes.
          size_t n_children() const;
          // Returns the i'th child.
          const Node *child(size_t i) const;
          // Accepts a visitor.
          void accept(Visitor *) const;
          // Convenience functions that internally will use RTTI.
          static bool is_Element(const Node *);
          static const Element *to_Element(const Node *);
          //Element(): m_children(), m_name(""), m_nmspace(""), m_uri(""), m_n_children(0), m_n_nmspaces(0) { };
          Element(String name_, String nmspace_, String uri_): 
               m_name(name_), m_nmspace(nmspace_), m_uri(uri_), m_n_children(0), m_n_nmspaces(0) { };
          ~Element();
     	void add_child(Node *);

          void set_n_nmspaces(int n_) { m_n_nmspaces = n_; };
          int get_n_nmspaces() { return m_n_nmspaces; };
          
          void * operator new (unsigned int size) { return Pool::get_mem(size, true); };
          void operator delete(void * mem) { Pool::free_mem(mem, true); };

          const String &nmspace_name() const { return m_nmspace; };
    private:
	     size_t m_n_children;
	     String m_name, m_nmspace, m_uri;
	     std::vector<Node *> m_children;
          int m_n_nmspaces;
};
}

#endif
