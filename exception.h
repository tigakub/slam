#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include <string>
#include <iostream>

using namespace std;

class Exception {
    public:
        string file;
        int line;
        string msg;

    public:
        Exception(const string &iFile, int iLine, const string &iMsg)
        : file(iFile), line(iLine), msg(iMsg) { }
};

inline ostream &operator << (ostream &os, const Exception &e) {
    return os << "Exception thrown in file \"" << e.file << "\", line \"" << e.line << "\", mesage: " << e.msg;
}

#define THROW(msg) throw Exception(string(__FILE__), __LINE__, string(msg));
#define THROW_IF(cond, msg) if(cond) { throw Exception(string(__FILE__), __LINE__, string(msg)); }
#define THROW_IF_NOT(cond, msg) if(!(cond)) { throw Exception(string(__FILE__), __LINE__, string(msg)); }
#define THROW_IF_NULL(cond, msg) if((cond) == NULL) { throw Exception(string(__FILE__), __LINE__, string(msg)); }

#endif