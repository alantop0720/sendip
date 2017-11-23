// test2.cpp : 定义控制台应用程序的入口点。

#include "stdafx.h"

#include "Smtp.h"
#include <iostream>
#include <atltime.h>
#include <Wininet.h>
#include <cstringt.h>


using namespace std;

char temp[500] = "\0";

int sendip(char *ip)
{
	CSmtp smtp(
		25,								/*smtp端口*/
		"smtp.126.com",					/*smtp服务器地址*/
		"zhangzhm@126.com",	        /*源邮箱地址*/
		"password",					/*邮箱密码*/
		"30796007@qq.com",	    /*目的邮箱地址*/
		ip,					/*主题*/
		"ip"		/*邮件正文*/
	);

	// 添加附件时注意,\一定要写成\\，因为转义字符的缘故
	// Addattachment:添加附件  CSmtp::DeleteAttachment函数删除附件  

	/*
	string filePath;
	filePath = "timg.jpg";
	smtp.AddAttachment(filePath);*/

	int err;
	if ((err = smtp.SendEmail_Ex()) != 0)
	{
		if (err == 1)
			cout << "错误1: 由于网络不畅通，发送失败!" << endl;
		if (err == 2)
			cout << "错误2: 用户名错误,请核对!" << endl;
		if (err == 3)
			cout << "错误3: 用户密码错误，请核对!" << endl;
		if (err == 4)
			cout << "错误4: 请检查附件目录是否正确，以及文件是否存在!" << endl;
	}

	//system("pause");  //VS2010 调试 console 程序比较重要
	return 0;
}


void GetIP()
{

	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(2, 2);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		return;
	}

	/* Confirm that the WinSock DLL supports 2.2.*/
	/* Note that if the DLL supports versions greater    */
	/* than 2.2 in addition to 2.2, it will still return */
	/* 2.2 in wVersion since that is the version we      */
	/* requested.                                        */

	if (LOBYTE(wsaData.wVersion) != 2 ||
		HIBYTE(wsaData.wVersion) != 2) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		WSACleanup();
		return;
	}

	/* The WinSock DLL is acceptable. Proceed. */



	char   name[255];
	gethostname(name, 255);

	CTime time = CTime::GetCurrentTime();

	CString strTime = time.Format(_T("%Y-%m-%d %H:%M:%S")); //Format("%I:%M:%S %p, %A, %B %d, %Y")

	char *charpoint = strTime.GetBuffer(strTime.GetLength());

	char   Url[100] =_T( "http://123cha.com");
	HINTERNET hSession = InternetOpen(_T(" RookIE/1.0 "), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hSession != NULL)
	{
		HINTERNET handle2 = InternetOpenUrl(hSession, Url, NULL, 0, INTERNET_FLAG_DONT_CACHE, 0);
		if (handle2 != NULL)
		{
			//printf(" %s\n ", Url);
			byte  Temp[2048];
			ULONG Number = 1;

			//测试写入文件时，使用。
			//FILE  * stream;
			char *end = NULL;

			//if ((stream = fopen("d:\\ip.html", " wb ")) != NULL) 
			// 上面代码测试时使用，配合下面的fwrite 和 fclose 
			{
				while (Number  >   0)
				{
					char ip[100] = "\0";
					InternetReadFile(handle2, Temp, 2048 - 1, &Number);

					//搜索匹配字符串 解析出IP地址 搜索前面"ip/?q=" 到">"之间的字符串
					char *p = strstr((char*)Temp, "ip/?q=");
					if (NULL != p)
					{
						end = (char *)memchr(p, '>', 50);

						memcpy(ip, p + 6, end - p - 6);
						//此处是更新时间，
						//strcat(ip, " update by ");
						//strcat(ip, charpoint);

						strcat(name, " = ");
						strcat(name, ip);

						//SetWindowText(name);
						if (strcmp(temp, name) == 0)
							printf("ip相同，不发送邮件\n");
						else
						{
							strcpy(temp, name);
							sendip(name);
							printf("邮件已发送。 发送内容：%s\n", name);
						}


						//获取ip地址后，就清除资源，返回。
						InternetCloseHandle(handle2);
						handle2 = NULL;

						InternetCloseHandle(hSession);
						hSession = NULL;

						WSACleanup();

						return;


					}
					//fwrite(Temp, sizeof(char), Number, stream);
				}
				//fclose(stream);
			}

			InternetCloseHandle(handle2);
			handle2 = NULL;
		}
		InternetCloseHandle(hSession);
		hSession = NULL;
	}
	printf("InternetOpen函数执行失败,请检查网络\n");

	WSACleanup();


}

void main()
{
	while (true)
	{
		GetIP();
		Sleep(1000 * 10); //10秒发送一次
	}

}


