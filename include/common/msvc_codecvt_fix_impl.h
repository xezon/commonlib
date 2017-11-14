
// Apparently Microsoft forgot to define a symbol for codecvt.
// Works with /MT only
// _MSC_VER 1900 = VS 2015
// _MSC_VER 1911 = VS 2017

#include <locale>

#if (!_DLL) && (_MSC_VER >= 1900) && (_MSC_VER <= 1911)
std::locale::id std::codecvt<char16_t, char, _Mbstatet>::id;
#endif
