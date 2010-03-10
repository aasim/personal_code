#ifndef XMLP_VISITOR_HPP
#define XMLP_VISITOR_HPP

namespace xml {
     class Element;
     class Text;
     class Visitor {
public:
          virtual void start_element_visit(const Element &) = 0;
          virtual void end_element_visit(const Element &) = 0;
          virtual void visit_text(const Text &) = 0;
     };
}   

#endif
