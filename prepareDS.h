#ifndef _PREPARESD_
#define _PREPARESD_

#include "operations.h"
#include "data.h"

// prepare the hash set
// will add the name of the function as a key and pointer to the function
// in the memory as a data
static inline void PrepareOperationsHashSet(HashSetChain*);
// initialize the labels hash map to store the name of the label
// with the given int
static inline void PreparelabelsHashSet(HashSetChain*);
// initialize the Parts hash map to store the name of the part of code
// with the unique ID
static inline void PreparePartsHashSet(HashSetChain*);
// initialize the linkedlist which will contain the instructions
// it will conatian pointer to the given instruction only
static inline void prepareInstructionsLinkedList(SlinkedList*);
// initialize the linkedlist which will contain the lines
// it will conatian pointer to the given lines only
//these lines will be executed before the end statement
static inline void preparelaterLinkedList(SlinkedList*);
// initialize the linkedlist which will contain the lines
// it will conatian pointer to the given lines only
//these lines will be executed at first
static inline void preparelinesLinkedList(SlinkedList*);
// will be called after dealing the the all three
static inline void cleanUp(HashSetChain*, HashSetChain*, HashSetChain*
	, SlinkedList*, SlinkedList*, SlinkedList*);

#endif