#include "container.h"

Container::Container()
: head(nullptr) { }

Container::~Container() { delete head; }

Link * Container::getLink(size_t iIndex, bool iCreateIfNeeded) {
    Link * link = head;
    if(link) link = link->find(iIndex);
    if(!link && iCreateIfNeeded) {
        link = create(iIndex);
        if(head) {
            if(iIndex < head->getIndex()) {
                link->next = head;
                head = link;
            }
        } else {
            head = link;
        }

    }
    return link;
}
