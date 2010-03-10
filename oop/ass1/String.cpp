#include "String.hpp"

bool operator<(const String &l_str, const String &r_str) {
     char * l_ptr = l_str.m_begin;
     char * r_ptr = r_str.m_begin;

     int length = ((l_str.m_end - l_str.m_begin) - (r_str.m_end - r_str.m_begin));
     
     if(length < 0) return true;
     else if(length > 0) return false;

     while(l_ptr != l_str.m_end && r_ptr != r_str.m_end) {
          if(*l_ptr < *r_ptr) {
               return true;
         // } else if(*l_ptr == *r_ptr) {
         //      l_ptr++;
         //      r_ptr++;
          } else {
               return false;
          }
          l_ptr++, r_ptr++;
     }

     return false;

}

// Comparison with std::string.
bool operator==(const std::string &l_str, const String &r_str) {
	return l_str == r_str;
}

bool operator==(const String &l_str, const std::string &r_str) {
     return (l_str == r_str);
}

bool operator!=(const std::string &l_str, const String &r_str) {
     return (l_str != r_str);
}

bool operator!=(const String &l_str, const std::string &r_str) {
     return (l_str != r_str);
}

/*
// Comparison with C string.
bool operator==(const char *l_str, const String &r_str) {
     if(strcmp(l_str, r_str.c_str())) return false;
     else return true;
}

bool operator==(const String &l_str, const char *r_str) {
     if(strcmp(l_str.c_str(), r_str)) return false;
     else return true;
}

bool operator!=(const char *l_str, const String &r_str) {
     if(strcmp(l_str, r_str.c_str())) return true;
     else return false;
}

bool operator!=(const String &l_str, const char *r_str) {
     if(strcmp(l_str.c_str(), r_str)) return true;
     else return false;
}
*/
bool operator==(const String &l_str, const String &r_str) {
     char * l_ptr = l_str.m_begin;
     char * r_ptr = r_str.m_begin;

     while(l_ptr != l_str.m_end && r_ptr != r_str.m_end) {

          if(*l_ptr == *r_ptr) {
               l_ptr ++;
               r_ptr ++;
          } else {
               return false;
          }
     }

     if(l_ptr == l_str.m_end && r_ptr == r_str.m_end) {
          return true;
     } else {
          return false;
     }

}


bool operator!=(const String &l_str, const String &r_str) {
     return !(l_str == r_str);
}

// Output operator
std::ostream & operator<<(std::ostream & out, const String & str_) {
     char * ptr = str_.m_begin;
     while(ptr != str_.m_end) {
          out << *ptr;
          ptr ++;
     }
     return out;
}

