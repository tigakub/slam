#include "vis/container.h"

Container::Container()
: head(nullptr) { }

Container::Container(Container &&iOther)
: head(iOther.head) {
    iOther.head = nullptr;
}

Container::~Container() { delete head; }

Link * Container::getLink(size_t iIndex, bool &ioCreateIfNeeded) {
    ioCreateIfNeeded = false;
    Link * link = head;
    if(link) link = link->find(iIndex);
    if(!link && ioCreateIfNeeded) {
        link = create(iIndex);
        if(head) {
            if(iIndex < head->getIndex()) {
                link->next = head;
                head = link;
                ioCreateIfNeeded = true;
            } else {
                if(head->insert(link))
                    ioCreateIfNeeded = true;
            }
        } else {
            head = link;
            ioCreateIfNeeded = true;
        }
    }
    return link;
}

Link * Container::getHead() {
    return head;
}