#ifndef COMOPT_ERROR_H
#define COMOPT_ERROR_H

#include <iostream>
#include <sstream>

#ifndef COMPOPT_FUNC_NAME
// not using visual studio
#ifndef _MSC_VER
#define COMPOPT_FUNC_NAME __PRETTY_FUNCTION__
#else
// for Visual Studio 
#define COMPOPT_FUNC_NAME __FUNCSIG__
#endif
#endif

#define COMPOPT_LOCATION \
   "\n ... in function: " << COMPOPT_FUNC_NAME << \
   "\n ... at line " << __LINE__ << " of file: " << __FILE__ << '\n'

#define COMOPT_WARNING(msg)                                             \
      std::ostringstream comoptMsgStream;                               \
      comoptMsgStream << msg << COMPOPT_FUNC_NAME;                      \
	  const char* inputString = comoptMsgStream.str().c_str();	        \
	  std::cerr << "\n\n" << inputString << "\n";                       

// A separate buffer will be used for the warning and errors
#define COMOPT_ERROR(msg)  \
COMOPT_WARNING(msg);       \
std::abort();

#define COMOPT_ASSERT(x, msg, warning)                          \
   if (!(x))                                                    \
   {                                                            \
		if (!(warning))                                         \
		{                                                       \
			COMOPT_ERROR("Assertion failed with error: ("       \
				<< #x << ") is false: " << msg);                \
		}                                                       \
		else 										            \
		{                                                       \
			COMOPT_WARNING("Assertion failed with warning: ("   \
				<< #x << ") is false: " << msg);                \
		}                                                       \
   }