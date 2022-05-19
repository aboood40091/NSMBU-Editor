#include <cstdio>
#include <cassert>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <file.hpp>
#include <globals.hpp>

#include <nw/eft/eft_Config.h>
#include <nw/eft/eft_Emitter.h>
#include <nw/eft/eft_EmitterSet.h>
#include <nw/eft/eft_Resource.h>
#include <nw/eft/eft_System.h>

static GLFWwindow* WindowHandle = NULL;

bool WindowInit()
{
    if (WindowHandle)
        return false;

    // Initialize GLFW
    if (!glfwInit())
        return false;

    // Disable resizing
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // Request OpenGL v4.4 Compatibility Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    // Enforce double-buffering
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

    // Create the window instance
    WindowHandle = glfwCreateWindow(960, 720, "TEST", NULL, NULL);
    if (!WindowHandle)
    {
        glfwTerminate();
        return false;
    }

    // Query the Frame Buffer size
    glfwGetFramebufferSize(WindowHandle, &g_WindowWidth, &g_WindowHeight);

    // Make context of window current
    glfwMakeContextCurrent(WindowHandle);

    // Set swap interval to 1 by default
    glfwSwapInterval(1);

    // Initialize GLEW
    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        return false;
    }

    // Check clip control extension
    if (!GLEW_VERSION_4_5 && !GLEW_ARB_clip_control)
    {
        glfwTerminate();
        return false;
    }

    // Change coordinate-system to be compliant with GX2
    glClipControl(GL_UPPER_LEFT, GL_NEGATIVE_ONE_TO_ONE);

    // Enable scissor test
    glEnable(GL_SCISSOR_TEST);

    // Set the default viewport and scissor
    glViewport(0, 0, g_WindowWidth, g_WindowHeight);
    glScissor(0, 0, g_WindowWidth, g_WindowHeight);

    return true;
}

bool InitEftSystem()
{
    if (g_EftSystem)
        return false;

    nw::eft::Config config;
    config.SetEffectHeap(&g_EftRootHeap);

    g_EftSystem = new (g_EftRootHeap.Alloc(sizeof(nw::eft::System))) nw::eft::System(config);
    if (!g_EftSystem)
        return false;

    return true;
}

bool DeInitEftSystem()
{
    if (!g_EftSystem)
        return false;

    //g_EftSystem->Finalize();
    g_EftSystem->~System();
    g_EftRootHeap.Free(g_EftSystem);
    g_EftSystem = NULL;

    return true;
}

int main()
{
    printf("My directory is %s\n", g_CWD.c_str());

    if (!WindowInit())
        return -1;

    if (!InitEftSystem())
    {
        glfwTerminate();
        return 1;
    }

    printf("Renderer: %s\n", glGetString(GL_RENDERER));

    u8* ptcl_file_data = NULL;
    u32 ptcl_file_len = 0;

    if (!ReadFile((g_CWD + "/Eset_Cafe.ptcl").c_str(), &ptcl_file_data, &ptcl_file_len))
    {
        glfwTerminate();
        return 2;
    }

    g_EftSystem->EntryResource(&g_EftRootHeap, ptcl_file_data, 0);

    if (!g_EftSystem->CreateEmitterSetID(&g_EftHandle, nw::math::MTX34::Identity(), 0, 0, 0))
    {
        glfwTerminate();
        return 3;
    }

    printf("Current EmitterSet: %s\n", g_EftSystem->resources[0]->emitterSets[g_EftHandle.emitterSet->emitterSetID].name);

    u32 timer = 0;

    while (!glfwWindowShouldClose(WindowHandle))
    {
        glDepthMask(GL_TRUE);

        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClearDepth(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        g_EftSystem->BeginFrame();
        g_EftSystem->SwapDoubleBuffer();

        g_EftSystem->CalcEmitter(0);
        g_EftSystem->CalcParticle(true);

        // Camera and projection are hardcoded atm
        g_EftSystem->BeginRender(nw::math::MTX44::Identity(), nw::math::MTX34::Identity(), nw::math::VEC3::Zero(), -1.0f, 1.0f);

        for (nw::eft::EmitterInstance* emitter = g_EftSystem->emitterGroups[0]; emitter != NULL; emitter = emitter->next)
            g_EftSystem->RenderEmitter(emitter, true, NULL);

        g_EftSystem->EndRender();

        // IIRC This is used to profile performance
        g_EftSystem->Calc(true);

        {
            timer += 1;

            if (g_EftHandle.emitterSet->numEmitter == 0)
            {
                s32 nextEmitterSetId = g_EftHandle.emitterSet->emitterSetID + 1;
                printf("Next emitterSet id: %d\n", nextEmitterSetId);

                g_EftHandle.emitterSet->Kill();
                timer = 0;

                if (nextEmitterSetId == g_EftSystem->resources[0]->resource->numEmitterSet)
                    break;

                assert(g_EftSystem->CreateEmitterSetID(&g_EftHandle, nw::math::MTX34::Identity(), nextEmitterSetId, 0, 0));
            }

            if (timer == 5*60) // 5 seconds or effect is over
                g_EftHandle.emitterSet->Fade();
        }

        glfwSwapBuffers(WindowHandle);
        glfwPollEvents();
    }

    g_EftHandle.emitterSet->Kill();
    DeInitEftSystem();

    FreeFile(ptcl_file_data);

    glfwTerminate();
    return 0;
}
