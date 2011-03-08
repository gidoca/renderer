#ifndef LINKEDLISTELEMENT_H_INCLUDED
#define LINKEDLISTELEMENT_H_INCLUDED

template <class T>
class LinkedListElement {
    public:
        T *element;
        LinkedListElement *next;
        LinkedListElement *back;

        LinkedListElement(T& newElement, LinkedListElement *elementBefore) {
            element=&newElement;
            next=0;
            back=elementBefore;
        };

        LinkedListElement(T& newElement, LinkedListElement *elementBefore, LinkedListElement *newNext) {
            element=&newElement;
            back=elementBefore;
            next=newNext;
        };

        void deleteAll() {
            if(element)
                delete element;
            delete this;
        }

        ~LinkedListElement() {
            // element has to be deleted manually
        }
};

#endif // LINKEDLISTELEMENT_H_INCLUDED
