#include "Parser.hpp"

using namespace std;
using namespace xml;


Element * Parser::parse(const char * in, int size) {
     enum {
          WHITESPACE=1,
          NS_WHITESPACE,
          START_TAG_WHITESPACE,
          START_TAG,
          END_TAG,
          NAMESPACE,
          CONTENT,
          END,
          ERROR
     }state = WHITESPACE;

     enum {
          NMS_TAG_WHITESPACE=1,
          NMS_NAME_WHITESPACE,
          NMS_URI_WHITESPACE,
          TAG,
          URI,
          NAME,
          GT,
          NMS_END
     }nms_state = TAG;

     char * s = strdup("xmlns");
     String empty_string(s), xmlns_string(s);
     String str(s), nmspsc(s), elem(s), ns_name(s), ns_tag(s);
     empty_string.clear();
     str.clear();
     nmspsc.clear();
     elem.clear(); 
     ns_name.clear();
     ns_tag.clear();
     xmlns_string += &s[4];

     stack<Element *> elem_stack;
     stack<String> ns_stack;
     
     map<String, stack<String> > ns_map;
     int i = 0, n_nmspaces=0;
     Element * elem_tree;
     Element * element;
     Text * text;
    

     while(state != END) {
          if(i > size) {
               cout << "Error" << endl;
               exit(-1);
          }

          char ch = in[i];
          const char * ch_p = &in[i++];

          switch(state) {
               case ERROR:
                    exit(-1);
                    break;
               case WHITESPACE:
                    if(isspace(ch)) {
                         // Just consume this space, do nothing
                    } else if(ch == '<') {
                         if(in[i] == '/') {
                              i++;
                              state = END_TAG;
                         } else {
                              state = START_TAG_WHITESPACE;
                         }
                    } else if (ch == '\0') {
                         state = END;
                    } else {
                         cout << "Error: content outside a tag" << endl;
                         state = ERROR;
                    }

                    break;
               case CONTENT:
                    if(elem_stack.empty()) { 
                         // This content is just whitespace, that needs to be
                         // consumed.
                         state = WHITESPACE;
                    } else if(ch != '<') {
                         str += ch_p;
                    } else {
                         if(str != empty_string) {
                              // if non empty, make a text node, and attach
                              // it to the top most element on the stack
                              text = new Text(str);
                              elem_stack.top()->add_child(text);
                              str.clear();
                                 
                         } 
                    i--;
                    state = WHITESPACE;
                    }
                    break;

               case START_TAG_WHITESPACE:
                    if(isspace(ch)) {
                         // Do nothing
                    } else if (isalpha(ch) || ch == '_') {
                         i--;
                         state = START_TAG;
                    } else if (ch == '>') {
                         if(elem_stack.empty()) {
                              // Root node
                              element  = new Element(elem, empty_string, empty_string);
                              elem_tree = element;
                              elem_stack.push(element);

                         } else {
                              if(nmspsc != empty_string) {
                                   if(!ns_map[nmspsc].empty()) {
                                        element = new Element(elem, nmspsc, ns_map[nmspsc].top());
                                   } else {
                                        cout << "Error, in namespace definition" << endl;
                                        state = ERROR;
                                   }
                              } else {
                                   element = new Element(elem, empty_string, empty_string);
                              }

                              //elem_tree->add_child(element);
                              elem_stack.top()->add_child(element);
                              elem_stack.push(element);
                         }
                         element->set_n_nmspaces(n_nmspaces);
                         n_nmspaces = 0;
                         elem.clear();
                         nmspsc.clear();
                         state = CONTENT;
                    }
                    break;

               case NS_WHITESPACE:
                    if(isalpha(ch) || ch == '_') {
                         i-=2;
                         state = NAMESPACE;
                    } else if (ch == '>') {
                         i--;
                         state = START_TAG_WHITESPACE;
                    } else if (isspace(ch)) {
                         // Do nothing
                    } else {
                         assert(false);
                    }
                    break;

               case START_TAG:
                    if(isalpha(ch) || isdigit(ch) || ch == '_') {
                         elem += ch_p;
                    } else if(ch == ':') {
                         // the str stored before this
                         // was actually the namespace
                         // in which this element is scoped
                         // Thus, bind this element to the
                         // namespace scope.
                         // clear str for storing the remaining 
                         // name now.
                         nmspsc = elem;
                         elem.clear();
                         
                    } else if(isspace(ch))  {
                         state = NS_WHITESPACE;
                    } else if(ch == '>') {
                         i--;
                         state = START_TAG_WHITESPACE;
                    } else {
                         assert(false);
                    }
                         
                    break;
               case END_TAG:
                    if(isalpha(ch) || isdigit(ch) || ch == '_') {
                         str += ch_p;
                    } else if (ch == ':') {
                         // the string we read was the namespace
                         // in which this tag was scoped.
                         // clear the str tag, and read the name
                         // of the element
                         // Also, store the active namespace
                         nmspsc = str;
                         str.clear();
                    } else if(isspace(ch)) {
                         // Do nothing, just ignore whitespace
                    } else if(ch == '>') {
                         // Pop out the top most entry from the 
                         // stack and compare it with our string
                         // it should match, throw error otherwise
                         // check namespace too.

                         if(elem_stack.top()->name() == str &&
                            elem_stack.top()->nmspace_name() == nmspsc) {
                              int i = elem_stack.top()->get_n_nmspaces();
                              while( i > 0 ) { 
                                   ns_map[ns_stack.top()].pop();
                                   ns_stack.pop();
                                   i--;
                              }
                              elem_stack.pop();
                              
                         } else {
                              cout << "error with stack, wrong element popped out" << endl;
                              state = ERROR;
                              break;
                         }

                         str.clear();
                         nmspsc.clear();

                         // change state depending on current state of stack
                         if(elem_stack.empty()) {
                              state = WHITESPACE;
                         } else {
                              state = CONTENT;
                         }

                    } else {
                         assert(false);
                    }
                    break;
               case NAMESPACE:
                    // Read all namespaces
                    //
                    nms_state = TAG;
                    str.clear();
                    n_nmspaces = 0;

                    while(nms_state != NMS_END) {

                         if( i > size ) {
                              cout << "error" << endl;
                              state = ERROR;
                              break;
                         }
                         
                         ch = in[i];
                         ch_p = &in[i++];

                         switch(nms_state) {
                              case NMS_TAG_WHITESPACE:
                                   if(ch == '"') {
                                        nms_state = URI;
                                   } else if (isspace(ch) || ch == '=') {
                                        // Ignore spaces and =
                                   } else if (isalpha(ch)) {
                                        i--;
                                        nms_state = TAG;
                                   } else if (ch == '>') {
                                        i--;
                                        nms_state = NMS_END;
                                        state = START_TAG_WHITESPACE;
                                   } else {
                                        cout << "Error in nms_whitespace" << endl;
                                        nms_state = NMS_END;
                                        state = ERROR;
                                   }
                                   break;
                              case NMS_NAME_WHITESPACE:
                                   if(ch == '=') {
                                        // Ignore '=' 
                                   } else if(isalpha(ch) || ch == '_') {
                                        nms_state = NAME;
                                        i--;
                                   } else if(ch == '"') {
                                        nms_state = URI;
                                   } else {
                                        cout << "Error in nms_name_whitespace" << endl;
                                        nms_state = NMS_END;
                                        state = ERROR;
                                   }
                                   break;
                              case TAG:
                                   if(isalpha(ch) || isspace(ch)) {
                                        ns_tag += ch_p;
                                   } else if (ch == ':') {
                                        if(ns_tag != xmlns_string) {
                                             cout << "XML tag error" << endl;
                                             nms_state = NMS_END;
                                             state = ERROR;
                                        } else { 
                                             nms_state = NMS_NAME_WHITESPACE;
                                             ns_tag.clear();
                                        }
                                   } else {
                                        cout << "Error in nms_tag" << endl;
                                        nms_state = NMS_END;
                                        state = ERROR;
                                   }
                                   break;
                              case NAME:
                                   if(isalpha(ch) || isdigit(ch) || ch == '_') {
                                        str += ch_p;
                                   } else if( ch == '=') {
                                        // store namespace name in string
                                        ns_name = str;
                                        str.clear();
                                        nms_state = NMS_NAME_WHITESPACE;
                                   } else {
                                        cout << "Error in name" << endl;
                                        nms_state = NMS_END;
                                        state = ERROR;
                                   }
                                   break;
                              case URI:
                                   if(ch != '"') {
                                        str += ch_p;
                                   } else {
                                        // store uri for namespace and name in
                                        // map
                                        ns_map[ns_name].push(str);
                                        n_nmspaces ++;
                                        ns_stack.push(ns_name);
                                        str.clear();
                                        nms_state = NMS_URI_WHITESPACE;
                                   }
                                   break;
                              case NMS_URI_WHITESPACE:
                                   if(isspace(ch) || ch == '>') {
                                        nms_state = NMS_TAG_WHITESPACE;
                                        i--;
                                   } else {
                                        cout << "Error in URI" << endl;
                                        nms_state = NMS_END;
                                        state = ERROR;
                                   }
                                   break;
                              default:
                                   break;
                         }
                    }

                    break;
               default:
                    assert(false);
                    break;
          }
     }
     free(s);
     return elem_tree;
}
