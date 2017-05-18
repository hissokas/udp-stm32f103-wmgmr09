/******************************
΢Ƕ���ӹ����� ��Ȩ����
STM32 WIFI ������
������΢Ƕ���ӡ���ͨҼ��WIFI�����塱
�������¼ http://yuikin.taobao.com
Դ����ʹ��Э�飺��������ṩ���ͻ���ѧϰ�����߲ο�WIFIԭ��֮�ã��㲻�����㷢���Ĳ�Ʒ����
������Դ����������ļ����߲���Ƭ�Σ������ܳ��ۻ���ʹ�ñ��������ṩ��Դ����������ͬ���Ͳ�Ʒ��
���磺�����塢ѧϰ�壬Ӫ���Խ�ѧ�����ȵȣ�������Ϊ��Ȩ���������ұ���׷����ط������ε�Ȩ����
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