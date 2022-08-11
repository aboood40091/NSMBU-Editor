#include <eft.h>

#if RIO_IS_CAFE
    #include <misc/rio_MemUtil.h>
#else
    #include <misc/rio_Types.h>
#endif

#include <nw/eft/eft_Handle.h>


EftHeap g_EftRootHeap;
nw::eft::System* g_EftSystem = nullptr;
nw::eft::Handle g_EftHandle;

void* EftHeap::Alloc(u32 size, s32 alignment)
{
#if RIO_IS_CAFE
    void* mem = rio::MemUtil::alloc(size, alignment);
    rio::MemUtil::set(mem, 0, size);
    return mem;
#else
    return calloc(size, 1);
#endif
}

void EftHeap::Free(void* ptr)
{
#if RIO_IS_CAFE
    rio::MemUtil::free(ptr);
#else
    free(ptr);
#endif
}
