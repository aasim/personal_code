#include <string.h>
#include <iostream>
#include <typeinfo>
#include <stdlib.h>

class A {
    public:
        int i_member;
};

class B {
    public:
        int i_member;
        double x_member;
};

class MemberHelperBase {
    public:
        virtual ~MemberHelperBase() { }
};

template <typename T>
class MemberHelper : public MemberHelperBase {
    public:
        MemberHelper(MemberHelper & helper_) {
            obj = helper_.get();
        };

        MemberHelper(const T & obj_) : obj(obj_)  { 
        }

        T get() { return obj; };

    private:
        T obj;
};

class Member {

    public:
        template <typename T>
        T 
        get() const {

            MemberHelper<T> * h = dynamic_cast<MemberHelper<T> *>(helper);

            if(!h) {
                std::cout << "Invalid cast" << std::endl;
                assert(false);
            }

            return h->get();
        };
        
        template <typename T>
        void
        set_helper(MemberHelper<T> & helper_) {
            helper = new MemberHelper<T>(helper_);
        };

        ~Member() {
            delete helper;
        };

    private:
        MemberHelperBase * helper;
};

class Binding {
    public:
        virtual Member get_member(const char * m_name) = 0;
};

template <typename T>
class Binding_tmpl: public Binding {
};

template <>
class Binding_tmpl<A>: public Binding {
    public:
        Binding_tmpl(A * a_) : a(a_) {
        };

        virtual Member get_member(const char * m_name) {
            Member m;
            if(strcmp(m_name, "i_member") == 0) {

                MemberHelper<int> m_helper(a->i_member);
                m.set_helper(m_helper);
            } else {
                std::cout << "Invalid member" << std::endl;
                assert(false);
            }
            return m;
        };

    private:
        A * a;
};

template <>
class Binding_tmpl<B>: public Binding {
    public:
        Binding_tmpl(B * b_) : b(b_) {
        };

        virtual Member get_member(const char * m_name) {
            Member m;
            if(strcmp(m_name, "i_member") == 0) {
                MemberHelper<int> m_helper(b->i_member);
                m.set_helper(m_helper);
            } else if (strcmp(m_name, "x_member") == 0) {
                MemberHelper<double> m_helper(b->x_member);
                m.set_helper(m_helper);
            } else {
                std::cout << "Invalid member" << std::endl;
                assert(false);
            }
            return m;
        };
    private:
        B * b;

};
