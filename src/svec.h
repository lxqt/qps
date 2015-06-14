// svec.h					emacs, this is a -*-c++-*- file
//
// This program is free software. See the file COPYING for details.
// Author: Mattias Engdegård, 1997-1999

#ifndef SVEC_H
#define SVEC_H

#include <map>
#include <iostream>

template<typename Key,typename T>
class SHash : public std::map <Key,T>
{
	public:
		T value(Key k,T r)
		{	
			using namespace std;
			typename map<Key,T>::const_iterator it;
			it=map<Key,T>::find(k);
			if(it==map<Key,T>::end()) 
				return r;
			return (*it).second;
		}
		void insert(Key k,T r)
		{
			using namespace std;
			map<Key,T>::insert(pair<Key,T>(k,r));
		}
		bool contains(Key k)
		{
			if(value(k,NULL)==NULL) return false;
			else return true;
		}
		
};


#include <stdlib.h>

//#ifndef NDEBUG
//#define CHECK_INDICES		// Check invalid vector indices (the default)
//#endif

template<class T>
class Svec
{
public:
    Svec(int max = 16);
    Svec(const Svec<T> &s);
    ~Svec();

    Svec<T> &operator=(const Svec<T> &s);
    int size() const;
    void setSize(int newsize);
    T &operator[](int i);
    T operator[](int i) const;
    void set(int i, T val);
    void sort(int (*compare)(const T *a, const T *b));
    void add(T x);
    void insert(int index,T val);     
    void remove(int index);
    void Delete(int index);
    void clear();
    void purge();		// like clear() but deletes all contents

private:
    void grow();
    void setextend(int index, T value);
    void indexerr(int index) const;

    T *vect;
    int alloced;		// # of entries allocated
    int used;			// # of entries actually used (size)
};

template<class T>
inline Svec<T>::Svec(int max): alloced(max), used(0)
{
    vect = (T *)malloc(max * sizeof(T));
}

template<class T>
inline Svec<T>::~Svec() { free(vect); }

template<class T>
inline int Svec<T>::size() const { return used; }

template<class T>
inline T &Svec<T>::operator[](int i)
{
#ifdef CHECK_INDICES
    if(i < 0 || i >= used)
	indexerr(i);
#endif
    return vect[i];
}

template<class T>
inline T Svec<T>::operator[](int i) const
{
#ifdef CHECK_INDICES
    if(i < 0 || i >= used)
	indexerr(i);
#endif
    return vect[i];
}

template<class T>
inline void Svec<T>::set(int i, T val)
{
    if(i < 0 || i >= used)
		setextend(i, val);
    else
		vect[i] = val;
}

template<class T>
inline void Svec<T>::add(T x)
{
    if(++used > alloced)
	grow();
    vect[used - 1] = x;
}

/*
template<class T>
inline void Svec<T>::add(T x)
{
    if(++used > alloced)
    {

    }

    vect[used - 1] = x;
}
*/

template<class T>
inline void Svec<T>::clear() 
{ 
    used = 0; 
}

template<class T>
inline void Svec<T>::grow()
{
//	printf("size=%d\n",sizeof(T));
    vect = (T *)realloc(vect, (alloced *= 2) * sizeof(T));
}

#endif	// SVEC_H



