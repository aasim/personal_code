#ifndef XMLP_STRING_HPP
#define XMLP_STRING_HPP 

#include <iostream>

class String {
     public:
          String(const char *str_) { 
               m_begin = const_cast<char *>(str_);
               m_end = m_begin + 1;
          };

          String(const String &str_): m_begin(str_.m_begin), m_end(str_.m_end) {
          }
        
          // Conversion to std::string.
          operator std::string() const {
               std::string str(m_end - m_begin, '&');
               char * c_str = m_begin;
               int i = 0;
               while(c_str != m_end ) {
                    str[i] = *c_str;
                    c_str ++;
                    i ++;
               }
               return str;
          }

          // Assignment.
          String & operator=(const String &str_) {
               m_begin = str_.m_begin;
               m_end = str_.m_end;
               return *this;
          };

          String & operator += (const char * ch) {
               if(!m_begin) {
                    m_begin = const_cast<char *>(ch);
               }
               m_end = const_cast<char *>(ch) + 1;
               return (*this);
          };

          void clear() { m_begin = m_end = NULL; };

          String & operator *() { return *this; };

          char * m_begin;
          char * m_end;
};

// Comparison with std::string.
bool operator==(const std::string &, const String &);
bool operator==(const String &, const std::string &);
bool operator!=(const std::string &, const String &);
bool operator!=(const String &, const std::string &);

// Comparison with C string.
bool operator==(const char *, const String &);
bool operator==(const String &, const char *);
bool operator!=(const char *, const String &);
bool operator!=(const String &, const char *);

// Comparison with another string.
bool operator==(const String &, const String &);
bool operator!=(const String &, const String &);

// Output operator
std::ostream &operator<<(std::ostream &, const String &);     

// Less than operator
bool operator<(const String &, const String &);

#endif
