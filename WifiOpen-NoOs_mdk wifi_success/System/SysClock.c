/******************************
微嵌电子工作室 版权所有
STM32 WIFI 开发板
适用于微嵌电子“灵通壹号WIFI开发板”
详情请登录 http://yuikin.taobao.com
源代码使用协议：本代码仅提供给客户做学习、或者参考WIFI原理之用，你不能在你发布的产品里面
包含本源代码的完整文件或者部分片段），不能出售或者使用本工作室提供的源代码制作相同类型产品，
例如：开发板、学习板，营利性教学讲座等等，否则视为侵权，本工作室保留追究相关法律责任的权利。
*******************************/

#include "Project.h"

static SYSTEMTIME SysClock;
static volatile DWORD dwDelayMsCounter = 0;

#ifdef _WIN32
HANDLE hSysClockThread = INVALID_HANDLE_VALUE;
HANDLE hSysClockExitEvent = INVALID_HANDLE_VALUE;
#endif

void SysClockInit(void)
{
	TRACE("+SysClockInit()\r\n");

	ZeroMemory(&SysClock, sizeof(SYSTEMTIME));
	SysClockConfiguration();

	TRACE("-SysClockInit()\r\n");
}

#ifdef _WIN32
static DWORD SysClockThread(PVOID pThreadArg)
{
	TRACE("+SysClockThread()\r\n");

	hSysClockExitEvent = CreateEvent(NULL,FALSE,FALSE,NULL);

	if(!hSysClockExitEvent) {
		TRACE("SysClockThread: Create Object Failure\r\n");
		return -1;
	}

	for(;;) {
		// Delay 1ms
		WaitForSingleObject(hSysClockExitEvent,1);
		// Call system clock call-back function
		SysTickCallBack();
	}

	CloseHandle(hSysClockExitEvent);

	TRACE("-SysClockThread()\r\n");

	return 0;
}
#endif

void SysClockConfiguration(void)
{
	TRACE("+SysClockConfiguration()\r\n");

#ifdef _WIN32
	//hSysClockThread = CreateThread(NULL,0,SysClockThread,NULL,0,NULL);
#else
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	SysTick_Config(90000);
#endif

	TRACE("-SysClockConfiguration()\r\n");
}

void SysTickCallBack(void)
{
	// System clock
	if (1) {
		SysClock.wMilliseconds ++;
		if (SysClock.wMilliseconds == 1000) {
			SysClock.wMilliseconds = 0;
			SysClock.wSecond ++;
			//TRACE("CurrTime: %02d:%02d:%02d\r\n", SysClock.wHour, SysClock.wMinute, SysClock.wSecond);
			if (SysClock.wSecond == 60) {
				SysClock.wSecond = 0;
				SysClock.wMinute ++;
				if (SysClock.wMinute == 60) {
					SysClock.wMinute = 0;
					SysClock.wHour ++;
					if (SysClock.wHour == 24)
						SysClock.wHour = 0;
				}
			}
		}
	}
	// For Delay(ms)
	if (dwDelayMsCounter)
		dwDelayMsCounter --;
}

void Sleep(DWORD ms)
{
	dwDelayMsCounter = ms;

	while(dwDelayMsCounter);
}

void SysGetTime (PSYSTEMTIME pTime)
{
	CopyMemory(pTime, &SysClock, sizeof(SYSTEMTIME));
}