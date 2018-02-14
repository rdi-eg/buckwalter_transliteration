#include "rdi_buckwalter_conversions.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <omp.h>
#include "rdi_buckwalter_transliteration.h"
#include <clocale>
using namespace std;

namespace RDI
{

namespace RDIInternal
{

template <typename T>
bool within_vector(T element, const vector<T> v)
{
	auto iterator = std::find (begin(v), end(v), element);

	if (iterator != end(v))
		return true;

	return false;
}

char convert_wspace_to_space(const wchar_t space)
{
	assert ( within_vector(space, wspaces) );

	return wspace_to_space.at(space);
}

wchar_t convert_space_to_wspace(const char space)
{
	assert ( within_vector(space, char_spaces) );

	return space_to_wspace.at(space);
}


/// the index is passed by reference because we need to move it to after the unknown characters.
string handle_unknown_char(const wstring &input, size_t &index)
{
	assert(index < input.size());
	string output;

	if (index > 0 && !iswspace(input[index - 1]))
		output += " ";

	output += "<UNK>";

	for(; index < input.size(); index++)
	{
		if(iswspace(input[index]) || within_vector(input[index], arabic_letters_without_tashkeel))
		{
			break;
		}
	}

	if (!iswspace(input[index]) && index == input.size() - 1)
		output += " ";

	index--;

	return output;
}

wstring internal_convert_buckwalter_to_arabic(string buckwlater, bool tashkeel)
{
	std::setlocale(LC_ALL, "en_US.UTF8"); // needed by the isspace and iswspace functions
	wstring arabic;
	map<int, wstring> index_arabic;

#pragma omp parallel
{
	wstring private_arabic;
	int last_index = 0;
	#pragma omp for
	for (size_t i = 0; i < buckwlater.size(); i++)
	{
		if (isspace(buckwlater[i]))
		{
			private_arabic += convert_space_to_wspace(buckwlater[i]);
		}
		else if (within_vector(buckwlater[i],
							   tashkeel ? buckwalter_letters_with_tashkeel :
										  buckwalter_letters_without_tashkeel))
		{
			private_arabic += buckwalter_to_arabic.at(buckwlater[i]);
		}

		last_index = i;
	}

	#pragma omp critical
	{
		if(private_arabic != L"") // if this condition does not exist some kind of race condition happens
								  // We don't know why it works this way but it works...
		{
			index_arabic[last_index] = private_arabic;
		}
	}
}

	for (auto element : index_arabic)
		arabic += element.second;

	return arabic;
}

} // namespace RDIInternal

using namespace RDIInternal;

wstring convert_buckwalter_to_arabic(string buckwlater)
{
	return internal_convert_buckwalter_to_arabic(buckwlater, true);
}

wstring convert_buckwalter_to_arabic_no_tashkeel(string buckwlater)
{
	return internal_convert_buckwalter_to_arabic(buckwlater, false);
}

string convert_buckwalter_to_buckwalter_without_tashkeel(string buckwalter)
{
	std::setlocale(LC_ALL, "en_US.UTF8"); //needed by the isspace and iswspace functions
	string output;

	for (size_t i = 0; i < buckwalter.size(); i++)
	{
		if (within_vector(buckwalter[i], buckwalter_letters_without_tashkeel))
		{
			output += buckwalter[i];
		}
	}

	return output;
}

wstring convert_arabic_to_arabic_without_tashkeel(wstring arabic_with_tashkeel)
{
	std::setlocale(LC_ALL, "en_US.UTF8"); //needed by the isspace and iswspace functions
	wstring output;
	map<int, wstring> index_arabic;
#pragma omp parallel
{
	wstring private_output;
	int last_index = 0;
	#pragma omp for
	for (size_t i = 0; i < arabic_with_tashkeel.size(); i++)
	{
		if (iswspace(arabic_with_tashkeel[i]) ||
				within_vector(arabic_with_tashkeel[i], arabic_letters_without_tashkeel))
		{
			private_output += arabic_with_tashkeel[i];
		}
		last_index = i;
	}

	#pragma omp critical
	{
		if(private_output != L"") // if this condition does not exist some kind of race condition happens
								  // We don't know why it works this way but it works...
		{
			index_arabic[last_index] = private_output;
		}
	}
}
	for (auto element : index_arabic)
		output += element.second;

	return output;
}

string convert_arabic_to_buckwalter_remove_unknown(wstring arabic)
{
	std::setlocale(LC_ALL, "en_US.UTF8"); //needed by the isspace and iswspace functions
	string buckwalter;

	for (size_t i = 0; i < arabic.size(); i++)
	{
		if (iswspace(arabic[i]))
		{
			buckwalter += convert_wspace_to_space(arabic[i]);
		}
		else if (within_vector(arabic[i], arabic_letters_with_tashkeel))
		{
			buckwalter += arabic_to_buckwalter.at(arabic[i]);
		}
	}

	return buckwalter;
}

string convert_arabic_to_buckwalter(wstring arabic)
{
	std::setlocale(LC_ALL, "en_US.UTF8"); //needed by the isspace and iswspace functions
	string buckwalter;
	int sz = arabic.size();
	for (size_t i = 0; i < sz; i++)
	{
		if (iswspace(arabic[i]))
		{
			buckwalter += convert_wspace_to_space(arabic[i]);
		}
		else if (within_vector(arabic[i], arabic_letters_with_tashkeel))
		{
			buckwalter += arabic_to_buckwalter.at(arabic[i]);
		}
		else
		{
			buckwalter += handle_unknown_char(arabic, i);
		}
	}

	return buckwalter;
}

} // namespace RDI
