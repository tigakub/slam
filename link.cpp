#include "link.h"

Link::Link(size_t iIndex)
: next(nullptr), index(iIndex) { }

Link::~Link() { if(next) delete next; }

size_t Link::getIndex() {
    return index;
}

Link * Link::getNext() {
    return next;
}

bool Link::insert(Link * iOther) {
    if(index == iOther->index) return false;
    if(index < iOther->index) {
        if(next && next->index < iOther->index) {
            return next->insert(iOther);
        } else {
            iOther->next = next;
            next = iOther;
            return true;
        }
    }
    return false;
}

Link * Link::find(size_t iIndex) {
    if(iIndex == index) return this;
    if(next) return next->find(iIndex);
    return nullptr;
}
