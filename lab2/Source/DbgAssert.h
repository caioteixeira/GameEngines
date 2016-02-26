// Defines a custom assertion macro
#pragma once

#ifdef _DEBUG
extern bool DbgAssertFunction(bool expr, wchar_t* expr_string, wchar_t* desc, int line_num, wchar_t* file_name);

// These macros convert __FILE__ from char* to wchar_t*
#define DBG_WIDEN2(x) L##x
#define DBG_WIDEN(x) DBG_WIDEN2(x)
#define __WFILE__ DBG_WIDEN(__FILE__)

#define DbgAssert(expr, description) {if (DbgAssertFunction((expr), L#expr, L##description, __LINE__, __WFILE__)) {_asm{int 3}}}
#else
#define DbgAssert(expr, description)
#endif // _DEBUG
