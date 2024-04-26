#ifndef __CONTAINER_H__
#define __CONTAINER_H__

#include <stdio.h>

#include "link.h"

using namespace std;

class Container {
    protected:
        Link * head;

    public:
        Container();
        virtual ~Container();

        Link * getLink(size_t iIndex, bool iCreateIfNeeded = false);

    protected:
        virtual Link * create(size_t iIndex) = 0;
};

#endif
