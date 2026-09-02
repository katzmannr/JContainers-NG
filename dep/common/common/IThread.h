#pragma once

#include <wtypes.h>

// TODO: I really don't like the interface for this

class IThread
{
	public:
		typedef void (* MainProcPtr)(void * param);

				IThread();
				~IThread();

		void	Start(MainProcPtr proc, void * procParam = NULL);
		void	Stop(void);
		void	ForceStop(void);

		bool	IsRunning(void)		{ return isRunning; }
		bool	StopRequested(void)	{ return stopRequested; }

		HANDLE	GetHandle(void)		{ return theThread; }

	protected:
		MainProcPtr		mainProc;
		void			* mainProcParam;
		volatile bool	stopRequested;
		bool			isRunning;
		HANDLE			theThread;
        DWORD			threadID;

	private:
        static DWORD WINAPI _ThreadProc(void * param);
};
