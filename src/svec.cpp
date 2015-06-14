// svec.C
//
// This program is free software. See the file COPYING for details.
// Author: Mattias Engdegård, 1997-1999

// implement a stretchy vector class:
// An Svec<T> grows automatically (doubles when full), so that adding
// elements to the end has an amortized cost of O(1).
// For now, only use this for types not requiring a con/destructor.

#ifndef SVEC_C
#define SVEC_C

#include "svec.h"
////#define fatal(str) { printf("fatal error: %s\n",str); exit(0);} 
#define fatal  printf 

template<class T>
Svec<T>::Svec(const Svec<T> &s)
{
    int n = s.size();
    if(n < 8) n = 8;
    vect = (T *)malloc(n * sizeof(T));
    alloced = n;
    used = s.size();
    for(int i = 0; i < used; i++)
	vect[i] = s[i];
}

template<class T>
Svec<T> &Svec<T>::operator=(const Svec<T> &s)
{
    if(this != &s) {
	if(alloced < s.size()) {
	    alloced = s.size();
	    vect = (T *)realloc(vect, alloced * sizeof(T));
	}
	for(int i = 0; i < s.size(); i++) {
	    vect[i] = s.vect[i];
	}
	used = s.size();
    }
    return *this;
}

/*
template<class T>
void Svec<T>::indexerr(int index) const
{
    fatal("Svec: index out of range (%d, valid is 0..%d)", index, used - 1);
}
*/

template<class T>
void Svec<T>::setSize(int newsize)
{
    while(newsize > alloced) grow();
    used = newsize;
}

template<class T>
void Svec<T>::setextend(int index, T value)
{
#ifdef CHECK_INDICES
    if(index < 0)
	fatal("const Svec: negative index");
#endif
    while(index >= alloced) grow();
    if(index >= used) used = index + 1;
    vect[index] = value;
}

template<class T>
void Svec<T>::insert(int index,T value)
{
#ifdef CHECK_INDICES
    if(index < 0 || index > used)
	fatal("Svec: index out of range");
#endif
    if((used +1)> alloced)
	grow();


    /*int i;
    T v;
	  v=vect[i+1];
    for(i = index; j < =used ; i++)
	vect[i+1]=v;
    	vect[i+1]=vect[i]; //vect[index+1]=vect[index];
	*/	
	
    // old
    for(int j = used-1; j>=index ; j--)
	vect[j+1] = vect[j];

    vect[index]=value;

    used++;
}


// for int,float type , no delete
template<class T>  
void Svec<T>::remove(int index)
{
#ifdef CHECK_INDICES
    if(index < 0 || index >= used)
	fatal("Svec: index out of range");
#endif
   	for(int j = index; j < used - 1; j++)
		vect[j] = vect[j + 1];
	used--;
}

// for class type 
template<class T>  
void Svec<T>::Delete(int index)
{
#ifdef CHECK_INDICES
	if(index < 0 || index >= used)
		fatal("Svec: index out of range");
#endif
	delete vect[index];
	for(int j = index; j < used - 1; j++)
		vect[j] = vect[j + 1];
	used--;
}

// Assuming T is "pointer to U", delete all contents.
// Warning: duplicated objects will be deleted twice --- doubleplusungood
template<class T>
void Svec<T>::purge()
{
    for(int i = 0; i < used; i++)
		delete vect[i];
    used = 0;
}

extern "C"
{
    typedef int (*compare_func)(const void *, const void *);
}

template<class T>
void Svec<T>::sort(int (*compare)(const T *a, const T *b))
{
    qsort(vect, used, sizeof(T), (compare_func)compare);
}

#endif // SVEC_C
