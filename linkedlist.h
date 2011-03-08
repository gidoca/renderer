#ifndef LINKEDLIST_H_INCLUDED
#define LINKEDLIST_H_INCLUDED

#include "linkedlistelement.h"
//#define NDEBUG
#include <assert.h>

/*
    classic double-linked linked list (with next and back, start and end)
    (c) by Joerg Benedikt Schenker
*/

template <class T>
class LinkedList {
    private:
        LinkedListElement<T> *start;
        LinkedListElement<T> *end;
        unsigned int size;

        LinkedListElement<T> *iteratorPosition;
        LinkedListElement<T> *savedIteratorPosition;

        bool invariant() {
            assert(start>=0);

            if(start==0) {
                assert(end==0);
                assert(size==0);
            }

            if(start!=0) {
                assert(end!=0);
                assert(size!=0);
                assert(end->next==0);
                assert((start->back)==0);
            }

            if(size==1) {
                assert(end->back==0);
            }

            if(size==2) {
                assert(end->back==start);
                assert(end->back->next==end);
                assert(start->next==end);
            }

            if(size>1) {
                assert(start);
                assert(end);
                assert(end->back!=0);
                assert(testBack());
                assert(end->next==0);
                assert(start->back==0);
            }

            assert(testSize());
            return true;
        };

        bool testBack() {
            LinkedListElement<T> *position=start;
            LinkedListElement<T> *oneBefore=start;
            assert(position->back==0);
            position=position->next;
            while(position) {
                assert(position->back==oneBefore);
                position=position->next;
                oneBefore=oneBefore->next;
            }
            return true;
        };

        bool testSize() {
            unsigned int count = 0;
            LinkedListElement<T> *position=start;
            while(position) {
                ++count;
                position=position->next;
            }
            return count==size;
        };

        void recursiveDelete(LinkedListElement<T> *position) {
            if(position) {
                recursiveDelete(position->next);
                delete position->element;
                delete position;
            }
        }

    public:
        LinkedList() {
            size=0;
            start=0;
            end=0;
            assert(invariant());
        };

        ~LinkedList() {
            recursiveDelete(start);
        }

        inline const unsigned int& getSize() {
            return size;
        }

        inline void addAtEnd(T& newElement) {
            assert(invariant());
            if(size==0) {
                start=new LinkedListElement<T>(newElement,0);
                end=start;
            }
            else {
                end->next=new LinkedListElement<T>(newElement,end);
                end=end->next;
            }
            ++size;
            assert(invariant());
        };

        inline void addAtStart(T& newElement) {
            assert(invariant());
            start=new LinkedListElement<T> (newElement,0,start);
            if(size>0) {
                assert(start->next!=0);
                start->next->back=start;
            }
            ++size;
            if(!end) {
                end=start;
            }
            assert(invariant());
        };

        inline void addAtIteratorPosition(T& newElement) {
            assert(invariant());
            assert(iteratorPosition);
            if(iteratorPosition==start) {
                addAtStart(newElement);
                return;
            }
            LinkedListElement<T>* elementBefore;
            if(iteratorPosition)
                elementBefore=iteratorPosition->back;
            else
                elementBefore=end;
            elementBefore->next=new LinkedListElement<T> (newElement, elementBefore, iteratorPosition);
            if(iteratorPosition)
                iteratorPosition->back=iteratorPosition->back->next;
            else
                end=end->next;
            ++size;
            assert(invariant());
        };

        inline void deleteAtIteratorPosition() {
            assert(invariant());
            assert(iteratorPosition);
            assert(size>0);
            if(iteratorPosition==start) {
                deleteStart();
                return;
            }
            if(iteratorPosition==end) {
                deleteEnd();
                return;
            }
            assert(size>2); // if iteratorPosition wasn't start or end, it must be something inbetween
            iteratorPosition->back->next=iteratorPosition->next;
            iteratorPosition->next->back=iteratorPosition->back;
            LinkedListElement<T>* temp=iteratorPosition;
            iteratorPosition=iteratorPosition->next;
            temp->deleteAll();
            --size;
            assert(invariant());
        };

    private:
        inline void deleteStart() {
            assert(invariant());
            assert(size>0);
            start=start->next;
            --size;
            iteratorPosition->deleteAll();
            iteratorPosition=start;
            if(start) {
                start->back=0;
            }
            assert(invariant());
        };

        inline void deleteEnd() {
            assert(invariant());
            assert(size>0);
            end=end->back;
            if(end)
                end->next=0;
            --size;
            iteratorPosition->deleteAll();
            iteratorPosition=0;
            assert(invariant());
        };

    public:
        inline T& getElementAtEnd() {
            assert(end!=0);
            assert(size!=0);
            return *(end->element);
        };

        inline T& getSecondLastElement() {
            assert(end!=0);
            assert(size>1);
            return *(end->back->element);
        }

        inline T& getElementAtStart() {
            assert(start!=0);
            assert(size!=0);
            return *(start->element);
        };

        void inline resetIteratorPosition() {
            iteratorPosition=start;
        };

        bool inline iteratorHasNext() const {
            return iteratorPosition;
        };

        inline T& iteratorGetNext() {
            assert(iteratorPosition!=0); //h
            LinkedListElement<T> *temp=iteratorPosition;
            iteratorPosition=iteratorPosition->next;
            return *(temp->element);
        };

        inline T& getElementAtIteratorPosition() {
            assert(iteratorPosition);
            return *(iteratorPosition->element);
        };

        inline bool iteratorPositionIsCloseToStart() {
            if(!iteratorPosition)
                return false;
            return iteratorPosition==start || iteratorPosition->back==start;
        };

        inline void push(T& myElement) {
            addAtEnd(myElement);
        };

        inline T& pop() {
            assert(invariant());
            assert(size > 0);

            LinkedListElement<T> &tempReturnListElement = *end;
            if(size>1) {
                end=end->back;
                end->next=0;
            }
            else
                end=start=0;
            --size;
            T& returnElement = *(tempReturnListElement.element);
            delete &tempReturnListElement;

            assert(invariant());
            return returnElement;
        };

        inline void saveIteratorPositionNow() {
            savedIteratorPosition=iteratorPosition;
        };

        inline void loadOldIteratorPosition() {
            iteratorPosition=savedIteratorPosition;
        };
};

#endif // LINKEDLIST_H_INCLUDED
