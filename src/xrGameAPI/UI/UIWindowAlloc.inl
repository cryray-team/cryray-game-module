#pragma once

namespace UIWinAlloc
{
    struct _24b
    {
        DWORD _[6];
    };

    extern poolSS<_24b, 128> ui_allocator;

    template <class T>
    class uialloc
    {
    public:
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef T& reference;
        typedef const T& const_reference;
        typedef T value_type;

    public:
        template <class _Other>
        struct rebind
        {
            typedef uialloc<_Other> other;
        };

    public:
        pointer address(reference _Val) const { return (&_Val); }
        const_pointer address(const_reference _Val) const { return (&_Val); }
        uialloc() {}
        uialloc(const uialloc<T>&) {}
        template <class _Other>
        uialloc(const uialloc<_Other>&)
        {
        }
        template <class _Other>
        uialloc<T>& operator=(const uialloc<_Other>&)
        {
            return (*this);
        }
        pointer allocate(size_type n, const void* p = 0) const
        {
            VERIFY(1 == n);
            return (pointer)ui_allocator.create();
        };
        char* __charalloc(size_type n)
        {
            VERIFY(1 == n);
            return (char*)ui_allocator.create();
        };
        void deallocate(pointer p, size_type n) const
        {
            _24b* p_ = (_24b*)p;
            ui_allocator.destroy(p_);
        }
        void deallocate(void* p, size_type n) const
        {
            VERIFY(1 == n);
            _24b* p_ = (_24b*)p;
            ui_allocator.destroy(p_);
        }
        void construct(pointer p, const T& _Val) { std::_Construct(p, _Val); }
        void destroy(pointer p) { std::_Destroy(p); }
        size_type max_size() const
        {
            size_type _Count = (size_type)(-1) / sizeof(T);
            return (0 < _Count ? _Count : 1);
        }
    };
    template <class _Ty, class _Other>
    IC bool operator==(const uialloc<_Ty>&, const uialloc<_Other>&)
    {
        return (true);
    }
    template <class _Ty, class _Other>
    IC bool operator!=(const uialloc<_Ty>&, const uialloc<_Other>&)
    {
        return (false);
    }

    #define ui_list xr_vector

    #define DEF_UILIST(N, T)                                                                                               \
        typedef ui_list<T> N;                                                                                              \
        typedef N::iterator N##_it;
} // namespace UIWinAlloc