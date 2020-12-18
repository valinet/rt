#define _WIN32_WINNT 0x0500
#include <stdio.h>
#include <Windows.h>
#include <shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

typedef struct _ParamsReadOnly
{
	HANDLE hJobObject;
	JOBOBJECT_BASIC_LIMIT_INFORMATION stJobInfoBasic;
	JOBOBJECT_EXTENDED_LIMIT_INFORMATION stJobInfo;
	TCHAR* pCmdLineOrig;
	TCHAR* pCmdLine;
	SIZE_T dwLen;
	int hExceptionHandler;
} ParamsReadOnly;

typedef struct _ParamsNoAccessDuringExec
{
	STARTUPINFO stSI;
	PROCESS_INFORMATION stPI;
} ParamsNoAccessDuringExec;

LONG WINAPI ExceptionHandler(
	struct _EXCEPTION_POINTERS* ExceptionInfo
)
{
	TerminateProcess(GetCurrentProcess(), 0xF);
	return EXCEPTION_CONTINUE_SEARCH;
}

int WINAPI wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	HANDLE hProcess;
	DWORD dwOldProtect;
	ParamsReadOnly* pRO;
	ParamsNoAccessDuringExec* pNAE;

	if (!(pRO = VirtualAlloc(
		NULL,
		sizeof(ParamsReadOnly),
		MEM_COMMIT | MEM_RESERVE,
		PAGE_READWRITE
	)))
	{
		return GetLastError();
	}
	if (!(pNAE = VirtualAlloc(
		NULL,
		sizeof(ParamsNoAccessDuringExec),
		MEM_COMMIT | MEM_RESERVE,
		PAGE_READWRITE
	)))
	{
		return GetLastError();
	}

	if (__argc <= 1)
	{
		return 0xA;
	}
	if (!(pRO->hExceptionHandler = AddVectoredExceptionHandler(
		1,
		ExceptionHandler
	)))
	{
		return 0xB;
	}
	pRO->pCmdLineOrig = PathGetArgs(GetCommandLine());
	pRO->dwLen = sizeof(TCHAR) * (1 + (SIZE_T)lstrlen(pRO->pCmdLineOrig));
	if (!(pRO->pCmdLine = VirtualAlloc(
		NULL,
		pRO->dwLen,
		MEM_COMMIT | MEM_RESERVE,
		PAGE_READWRITE
	)))
	{
		return GetLastError();
	}
	CopyMemory(
		pRO->pCmdLine,
		pRO->pCmdLineOrig,
		pRO->dwLen
	);
	if (!(pRO->hJobObject = CreateJobObject(
		NULL,
		NULL
	)))
	{
		return GetLastError();
	}
	pRO->stJobInfoBasic.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
	pRO->stJobInfo.BasicLimitInformation = pRO->stJobInfoBasic;
	if (!SetInformationJobObject(
		pRO->hJobObject,
		JobObjectExtendedLimitInformation, &pRO->stJobInfo,
		sizeof(JOBOBJECT_EXTENDED_LIMIT_INFORMATION)
	))
	{
		return GetLastError();
	}
	if (!VirtualProtect(
		pRO,
		sizeof(ParamsReadOnly),
		PAGE_READONLY,
		&dwOldProtect
	))
	{
		return GetLastError();
	}

	while (TRUE)
	{
		ZeroMemory(
			&pNAE->stSI,
			sizeof(pNAE->stSI)
		);
		pNAE->stSI.cb = sizeof(pNAE->stSI);
		ZeroMemory(
			&pNAE->stPI,
			sizeof(pNAE->stPI)
		);
		if (!CreateProcess(
			NULL,
			pRO->pCmdLine,
			NULL,
			NULL,
			FALSE,
			0,
			NULL,
			NULL,
			&pNAE->stSI,
			&pNAE->stPI
		))
		{
			return GetLastError();
		}
		if (!(AssignProcessToJobObject(
			pRO->hJobObject,
			pNAE->stPI.hProcess
		)))
		{
			if (!TerminateProcess(pNAE->stPI.hProcess, 0))
			{
				return GetLastError();
			}
			return GetLastError();
		}
		hProcess = pNAE->stPI.hProcess;
		if (!VirtualProtect(
			pNAE,
			sizeof(ParamsNoAccessDuringExec),
			PAGE_NOACCESS,
			&dwOldProtect
		))
		{
			if (!TerminateProcess(hProcess, 0))
			{
				return GetLastError();
			}
			return GetLastError();
		}
		if (!VirtualProtect(
			pRO->pCmdLine,
			pRO->dwLen,
			PAGE_NOACCESS,
			&dwOldProtect
		))
		{
			if (!TerminateProcess(hProcess, 0))
			{
				return GetLastError();
			}
			return GetLastError();
		}
		if (!VirtualProtect(
			pRO,
			sizeof(ParamsReadOnly),
			PAGE_NOACCESS,
			&dwOldProtect
		))
		{
			if (!TerminateProcess(hProcess, 0))
			{
				return GetLastError();
			}
			return GetLastError();
		}
		if (WaitForSingleObject(
			hProcess,
			INFINITE
		) != WAIT_OBJECT_0)
		{
			return 0xC;
		}
		if (!VirtualProtect(
			pRO,
			sizeof(ParamsReadOnly),
			PAGE_READONLY,
			&dwOldProtect
		))
		{
			return GetLastError();
		}
		if (!VirtualProtect(
			pNAE,
			sizeof(ParamsNoAccessDuringExec),
			PAGE_READONLY,
			&dwOldProtect
		))
		{
			return GetLastError();
		}
		if (!CloseHandle(pNAE->stPI.hProcess))
		{
			return GetLastError();
		}
		if (!CloseHandle(pNAE->stPI.hThread))
		{
			return GetLastError();
		}
		if (!VirtualProtect(
			pNAE,
			sizeof(ParamsNoAccessDuringExec),
			PAGE_READWRITE,
			&dwOldProtect
		))
		{
			return GetLastError();
		}
		if (!VirtualProtect(
			pRO->pCmdLine,
			pRO->dwLen,
			PAGE_READWRITE,
			&dwOldProtect
		))
		{
			return GetLastError();
		}
	}
	if (!CloseHandle(pRO->hJobObject))
	{
		return GetLastError();
	}
	if (!RemoveVectoredExceptionHandler(pRO->hExceptionHandler))
	{
		return 0xD;
	}
	return 0;
}