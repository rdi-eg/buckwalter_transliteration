#include "buckwalter_conversions.h"

#define DO_NOT_DEFINE_THE_ARRAYS

#include <iostream>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <omp.h>
#include "buckwalter_transliteration.h"

using namespace std;
using namespace RDI;

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
string handle_unknown_char(const wstring &input, size_t &index, ReplacePolicy replace_policy)
{
	if (replace_policy == ReplacePolicy::Delete)
		return "";

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

string RDI::convert_arabic_to_buckwalter(wstring arabic, ReplacePolicy replace_policy)
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
		else
		{
			buckwalter += handle_unknown_char(arabic, i, replace_policy);
		}
	}

	return buckwalter;
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

wstring RDI::convert_buckwalter_to_arabic(string buckwlater)
{
	return internal_convert_buckwalter_to_arabic(buckwlater, true);
}

wstring RDI::convert_buckwalter_to_arabic_no_tashkeel(string buckwlater)
{
	return internal_convert_buckwalter_to_arabic(buckwlater, false);
}
