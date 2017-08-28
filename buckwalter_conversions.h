#ifndef BUCKWALTER_CONVERSIONS_H
#define BUCKWALTER_CONVERSIONS_H

#include <string>

namespace RDI
{

extern "C++"
{
	enum ReplacePolicy
	{
		Delete,
		UNK
	};

	/// Converts Arabic to Buckwalter
	/// replace_policy will either replace any non-arab characters with "<UNK>" without the quotations,
	/// or it will be ignored completely and the resulting buckwalter string will not contain them
	std::string convert_arabic_to_buckwalter(std::wstring arabic, ReplacePolicy replace_policy = ReplacePolicy::Delete);

	/// Converts Buckwalter to Arabic
	/// If given any letters that are not defined in the buckwalter specification they
	/// will be erased from the resulting wstring
	std::wstring convert_buckwalter_to_arabic(std::string buckwlater);

	/// Same as above but it does not any letters that are not defined
	/// in the buckwalter specification (except for the tashkeel)
	/// will be erased from the result
	std::wstring convert_buckwalter_to_arabic_no_tashkeel(std::string buckwlater);
}

}
#endif // BUCKWALTER_CONVERSIONS_H
