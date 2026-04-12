#include "WorkerThread.h"

using namespace Sexy;


SDL_Thread *xCreateThread(SexyThreadCallback *cb, void *p)
{
    SDL_Thread *result = NULL;
    struct Glue
    {
        SexyThreadCallback *cb;
        void *params;
    };
    const SDL_ThreadFunction Callback = [](void *params) -> int
    {
        Glue *glue = (Glue *)params;
        glue->cb(glue->params);
        delete glue;
        return 0;
    };
    Glue *glue = new Glue;
    glue->cb = cb;
    glue->params = p;
    
    result = SDL_CreateThreadWithStackSize(Callback, "SexyThread", 4*1024*1024, glue);
    return result;
}
void WorkerThread::WaitForTask()
{
    SDL_WaitThread(mTaskProc, NULL);
    mTaskProc = NULL;
}
void WorkerThread::DoTask(SexyThreadCallback *func, void *param)
{
    WaitForTask();
    mTaskProc = xCreateThread(func, param);
}
