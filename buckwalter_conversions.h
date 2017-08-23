#ifndef BUCKWALTER_CONVERSIONS_H
#define BUCKWALTER_CONVERSIONS_H

#include <string>

namespace RDI
{

extern "C++"
{
	enum ReplaceNonArabic
	{
		Delete,
		UNK
	};

	/// Converts Arabic to Buckwalter
	/// replace_policy will either replace any non-arab characters with "<UNK>" without the quotations,
	/// or it will be ignored completely and the resulting buckwalter string will not contain them
	std::string convert_arabic_to_buckwalter(std::wstring arabic, ReplaceNonArabic replace_policy);

	/// Converts Buckwalter to Arabic
	/// If given any letters that are not defined in the buckwalter specification they will be erased
	/// from the resulting wstring
	std::wstring convert_buckwalter_to_arabic(std::string buckwlater);
}

}
#endif // BUCKWALTER_CONVERSIONS_H
