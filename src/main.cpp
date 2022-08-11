#include <rio.h>

#include "editor.h"

int main()
{
    if (!rio::Initialize<Editor>())
        return -1;

    rio::EnterMainLoop();

    rio::Exit();
    return 0;
}
