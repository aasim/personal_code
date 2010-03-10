#include "Element.hpp"
using namespace xml;

bool Element::is_Element(const Node *node) {
     const Element * element = dynamic_cast<const Element *> (node);
     if(element == NULL) return false;
     else return true;		
}

const Element* Element::to_Element(const Node *node) {
	assert(Element::is_Element(node));
     const Element *element = dynamic_cast<const Element *> (node);
     return element;
}

const String & Element::name() const {
	return m_name;
}

const String & Element::nmspace() const {
	return m_uri;
}

size_t Element::n_children() const {
	return m_n_children;
}

const Node * Element::child(size_t i) const {
	return m_children.at(i);
}

Element::~Element() {
     for(size_t i = 0; i < n_children(); i++) {
          delete this->child(i);
     }
}

void Element::accept(Visitor *v) const {
     v->start_element_visit(*this);

     for(size_t i = 0; i < this->n_children(); i++) {
          if(Element::is_Element(this->child(i))) {
               const Element * element = Element::to_Element(this->child(i)); 
               element->accept(v);
          } else {
               const Text * text = Text::to_Text(this->child(i));
               v->visit_text(*text);
          }
     }

     v->end_element_visit(*this);
}

void Element::add_child(Node *node)  {
     m_children.push_back(node);
     m_n_children++;
}
