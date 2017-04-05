//
// Created by 宋辰伟 on 2017/4/2.
//

#ifndef PROJECT_PTR_H
#define PROJECT_PTR_H

#include "atomic/king_atomic.h"

struct ForceVFPTToTop {
    virtual ~ForceVFPTToTop() {}
};

////////////////////////////////////////////////////////////////////////
//
// class RefCountObj
// prototypical class for reference counting
//
////////////////////////////////////////////////////////////////////////
class RefCountObj : public ForceVFPTToTop
{
public:
    RefCountObj() : m_refcount(0) {}
    RefCountObj(const RefCountObj &s) : m_refcount(0)
    {
        (void)s;
        return;
    }

    virtual ~RefCountObj() {}
    RefCountObj &
    operator=(const RefCountObj &s)
    {
        (void)s;
        return (*this);
    }

    // Increment the reference count, returning the new count.
    int
    refcount_inc()
    {
        return katomic_increment((int *)&m_refcount, 1) + 1;
    }

    // Decrement the reference count, returning the new count.
    int
    refcount_dec()
    {
        return katomic_increment((int *)&m_refcount, -1) - 1;
    }

    int
    refcount() const
    {
        return m_refcount;
    }

    virtual void
    free()
    {
        delete this;
    }

private:
    volatile int m_refcount;
};

#define REF_COUNT_OBJ_REFCOUNT_INC(_x) (_x)->refcount_inc()
#define REF_COUNT_OBJ_REFCOUNT_DEC(_x) (_x)->refcount_dec()

////////////////////////////////////////////////////////////////////////
//
// class Ptr
//
////////////////////////////////////////////////////////////////////////
template <class T> class Ptr
{
    typedef void (Ptr::*bool_type)() const;
    void
    this_type_does_not_support_comparisons() const
    {
    }

public:
    explicit Ptr(T *p = 0);
    Ptr(const Ptr<T> &);
    ~Ptr();

    void clear();
    T *operator->() const { return (m_ptr); }
    T &operator*() const { return (*m_ptr); }
    operator bool_type() const { return m_ptr ? &Ptr::this_type_does_not_support_comparisons : 0; }
    int
    operator==(const T *p)
    {
        return (m_ptr == p);
    }

    int
    operator==(const Ptr<T> &p)
    {
        return (m_ptr == p.m_ptr);
    }

    int
    operator!=(const T *p)
    {
        return (m_ptr != p);
    }

    int
    operator!=(const Ptr<T> &p)
    {
        return (m_ptr != p.m_ptr);
    }

    // Return the raw pointer.
    T *
    get() const
    {
        return m_ptr;
    }

    // Return the raw pointer as a RefCount object. Typically
    // this is for keeping a collection of heterogenous objects.
    RefCountObj *
    object() const
    {
        return static_cast<RefCountObj *>(m_ptr);
    }

    // Return the stored pointer, storing NULL instead. Do not increment
    // the refcount; the caller is now responsible for owning the RefCountObj.
    T *
    detach()
    {
        T *tmp = m_ptr;
        m_ptr  = NULL;
        return tmp;
    }

    // XXX Clearly this is not safe. This is used in HdrHeap::unmarshal() to swizzle
    // the refcount of the managed heap pointers. That code needs to be cleaned up
    // so that this can be removed. Do not use this in new code.
    void
    swizzle(RefCountObj *ptr)
    {
        m_ptr = ptr;
    }
private:
    T *m_ptr;

//    friend class CoreUtils;
};

template <typename T>
Ptr<T>
make_ptr(T *p)
{
    return Ptr<T>(p);
}

////////////////////////////////////////////////////////////////////////
//
// inline functions definitions
//
////////////////////////////////////////////////////////////////////////
template <class T> inline Ptr<T>::Ptr(T *ptr /* = 0 */) : m_ptr(ptr)
{
    if (m_ptr) {
        m_ptr->refcount_inc();
    }
}

template <class T> inline Ptr<T>::Ptr(const Ptr<T> &src) : m_ptr(src.m_ptr)
{
    if (m_ptr) {
        m_ptr->refcount_inc();
    }
}

template <class T> inline Ptr<T>::~Ptr()
{
    if (m_ptr && m_ptr->refcount_dec() == 0) {
        m_ptr->free();
    }
}

template <class T>
inline Ptr<T> &
Ptr<T>::operator=(T *p)
{
    T *temp_ptr = m_ptr;

    if (m_ptr == p) {
        return (*this);
    }

    m_ptr = p;

    if (m_ptr) {
        m_ptr->refcount_inc();
    }

    if (temp_ptr && temp_ptr->refcount_dec() == 0) {
        temp_ptr->free();
    }

    return (*this);
}

template <class T>
inline void
Ptr<T>::clear()
{
    if (m_ptr) {
        if (!m_ptr->refcount_dec())
            m_ptr->free();
        m_ptr = NULL;
    }
}

template <class T>
inline Ptr<T> &
Ptr<T>::operator=(const Ptr<T> &src)
{
    return (operator=(src.m_ptr));
}



#endif //PROJECT_PTR_H