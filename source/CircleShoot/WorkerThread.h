#ifndef __SEXY_WORKER_THREAD_H__
#define __SEXY_WORKER_THREAD_H__

#include <SexyAppFramework/SexyAppBase.h>
#include <SDL2/SDL.h>
#include <vector>
#include <list>

namespace Sexy
{
	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////

    typedef void SexyThreadCallback(void *);
	class WorkerThread
	{
	public:
        SDL_Thread *mTaskProc;
		void DoTask(SexyThreadCallback *func, void* param);
        void WaitForTask();
	};
};

#endif

