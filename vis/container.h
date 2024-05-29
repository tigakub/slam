#ifndef __CONTAINER_H__
#define __CONTAINER_H__

#include <stdio.h>
#include <utility>

#include "link.h"

using namespace std;

class Container {
    protected:
        Link * head;

    public:
        Container();
        Container(Container &&iOther);
        virtual ~Container();

        Link * getLink(size_t iIndex, bool &ioCreateIfNeeded);
        Link * getHead();

        void removeLink(size_t iIndex);

    protected:
        virtual Link * create(size_t iIndex) = 0;
};

#endif
