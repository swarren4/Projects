#ifndef __gauntmore_macosx__NodeArray__
#define __gauntmore_macosx__NodeArray__

#include <stdio.h>
#include "PathNode.h"

class NodeArray {
public:
    
    NodeArray();
    
    ~NodeArray();
    
    void push_back(PathNode*);
    
    PathNode* pop_back();
    
    bool isEmpty();
    
    void setFront(PathNode*);
    
    PathNode* getFront();
    
    PathNode* getBack();
    
    int getSize();
    
    void swap(int, int);
    
    bool compare(int, int);
    
    
private:
    PathNode theHeap[1000];
    int size;

    void expand();
    
};

#endif /* defined(__gauntmore_macosx__NodeArray__) */
