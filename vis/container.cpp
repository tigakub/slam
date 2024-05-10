#include "vis/container.h"

Container::Container()
: head(nullptr) { }

Container::Container(Container &&iOther)
: head(iOther.head) {
    iOther.head = nullptr;
}

Container::~Container() { delete head; }

void Container::removeLink(size_t iIndex) {
    if(head) {
        if(head->index == iIndex) {
            auto cell = head;
            head = head->next;
            cell->next = nullptr;
            delete cell;
        } else {
            head->remove(iIndex);
        }
    }
}

Link * Container::getLink(size_t iIndex, bool &ioCreateIfNeeded) {
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
    } else {
        ioCreateIfNeeded = false;
    }
    return link;
}

Link * Container::getHead() {
    return head;
}