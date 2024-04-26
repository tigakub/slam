#ifndef __LINK_H__
#define __LINK_H__

#include <stdio.h>

using namespace std;

class Link {
    friend class Container;

    protected:
        Link * next;
        size_t index;

    public:
        Link(size_t iIndex);
        virtual ~Link();

        size_t getIndex();

        void insert(Link * iOther);

        Link * find(size_t iIndex);
};

#endif
