#ifndef BUCKWALTER_CONVERSIONS_H
#define BUCKWALTER_CONVERSIONS_H

#include <string>
#include <vector>

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
	/// Any non-arab characters will be ignored completely and the resulting buckwalter string will not contain them
	std::string convert_arabic_to_buckwalter(std::wstring arabic);

	/// Converts Arabic to Buckwalter
	/// replace any non-arab characters with "<UNK>",
	/// and returns the unknowns in the second argument in the same order as they were placed in the original string
	std::string convert_arabic_to_buckwalter(std::wstring arabic, std::vector<std::wstring> &unknowns);

	/// Converts Buckwalter to Arabic
	/// If given any letters that are not defined in the buckwalter specification they
	/// will be erased from the resulting wstring
	std::wstring convert_buckwalter_to_arabic(std::string buckwlater);

	/// Same as above but it does not any letters that are not defined
	/// in the buckwalter specification (except for the tashkeel)
	/// will be erased from the result
	std::wstring convert_buckwalter_to_arabic_no_tashkeel(std::string buckwlater);

	/// Any unknown character will be discarded
	std::string convert_buckwalter_to_buckwalter_without_tashkeel(std::string buckwalter);

	/// Any unknown character will be discarded
	std::wstring convert_arabic_to_arabic_without_tashkeel(std::wstring arabic_with_tashkeel);
}

}
#endif // BUCKWALTER_CONVERSIONS_H
