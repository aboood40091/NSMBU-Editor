#pragma once

#include <nw/eft/eft_Heap.h>

class EftHeap : public nw::eft::Heap
{
public:
    EftHeap() { }
    virtual ~EftHeap() { }

    virtual void* Alloc(u32 size, s32 alignment = nw::eft::Heap::EFT_HEAP_DEAFULT_ALIGNMENT);
    virtual void Free(void* ptr);
};

extern EftHeap g_EftRootHeap;

namespace nw { namespace eft {

class Handle;
class System;

} }

extern nw::eft::System* g_EftSystem;
extern nw::eft::Handle g_EftHandle;
