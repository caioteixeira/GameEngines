#include "ITPEnginePCH.h"
// Implements custom assert handler
#if _WIN32
#include <cstdlib>
#include <wchar.h>
bool DbgAssertFunction(bool expr, const wchar_t* expr_string, const wchar_t* desc, int line_num,
					   const wchar_t* file_name)
{
	bool bShouldHalt = !expr;
	if (bShouldHalt)
	{
		static wchar_t szBuffer[1024];
		_snwprintf_s(szBuffer, 1024, _TRUNCATE, 
					L"Assertion Failed!\n\nDescription: %s\nExpression: %s\nFile: %s\nLine: %d\n\nPress Retry to debug.",
					desc, expr_string, file_name, line_num);
		int msgbox = MessageBox(NULL, szBuffer, L"Assert", MB_ICONERROR | MB_ABORTRETRYIGNORE | MB_DEFBUTTON2);
		switch (msgbox)
		{
			case IDABORT:
				std::exit(1);
				break;
			case IDRETRY:
				// Do nothing which will cause it to break into the debugger
				break;
			case IDIGNORE:
				bShouldHalt = false; // If we're ignoring the assert, the show must go on!
				break;
		}
	}

	return bShouldHalt;
}
#else // not _WIN32
#include <cstdlib>
#include <CoreFoundation/CoreFoundation.h>
bool DbgAssertFunction(bool expr, const char* expr_string, const char* desc, int line_num,
					   const char* file_name)
{
	bool bShouldHalt = !expr;
	if (bShouldHalt)
	{
		const char* header = "Assert";
		//convert the strings from char* to CFStringRef
		CFStringRef header_ref      = CFStringCreateWithCString( NULL, header, (int)strlen(header) );
		CFStringRef message_ref  = CFStringCreateWithFormat( NULL, NULL,
															CFSTR("Assertion Failed!\n\nDescription: %s\nExpression: %s\nFile: %s\nLine: %d\n\nPress Retry to debug."),  desc, expr_string, file_name, line_num );
		
		CFOptionFlags result;  //result code from the message box
		
		//launch the message box
		CFUserNotificationDisplayAlert(
									   0, // no timeout
									   kCFUserNotificationNoteAlertLevel, //change it depending message_type flags ( MB_ICONASTERISK.... etc.)
									   NULL, //icon url, use default, you can change it depending message_type flags
									   NULL, //not used
									   NULL, //localization of strings
									   header_ref, //header text
									   message_ref, //message text
									   CFSTR("Ignore"), //default "ok" text in button
									   CFSTR("Abort"), //alternate button title
									   CFSTR("Retry"), //other button title, null--> no other button
									   &result //response flags
									   );
		
		//Clean up the strings
		CFRelease( header_ref );
		CFRelease( message_ref );
		
		if( result == kCFUserNotificationAlternateResponse )
		{
			std::exit(1);
		}
		else if (result == kCFUserNotificationDefaultResponse)
		{
			// Ignore this assertion
			bShouldHalt = false;
		}
	}
	//Convert the result
	return bShouldHalt;
}
#endif
