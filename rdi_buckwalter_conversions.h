#ifndef BUCKWALTER_CONVERSIONS_H
#define BUCKWALTER_CONVERSIONS_H

#include <string>
#include <vector>

namespace RDI
{

extern "C++"
{
	/// Converts Arabic to Buckwalter
	/// Any non-arab characters will be ignored completely and the resulting buckwalter string will not contain them
	std::string convert_arabic_to_buckwalter_remove_unknown(std::wstring arabic);

	/// Converts Arabic to Buckwalter
	/// replace any non-arab characters with "<UNK>"
	std::string convert_arabic_to_buckwalter(std::wstring arabic);

	/// Converts Arabic to Buckwalter
	/// replace each non-arab characters with "<UNK>" with a list of the opposite countinuous non-arabic chars
	/// return a list of any non-arab characters
	std::string convert_arabic_to_buckwalter(std::wstring arabic,std::vector<std::wstring>& unkown_chars);

	/// Converts Buckwalter to Arabic
	/// If given any letters that are not defined in the buckwalter specification they
	/// will be erased from the resulting wstring
	std::wstring convert_buckwalter_to_arabic(std::string buckwlater);

	/// Converts Buckwalter to Arabic
	/// recover the unkowns charchaters using it's orginal symbol list
	/// If given any letters that are not defined in the buckwalter specification they
	/// will be erased from the resulting wstring

	std::wstring convert_buckwalter_to_arabic(std::string buckwlater,const std::vector<std::wstring>& orignal_unknowns);

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
