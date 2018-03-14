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

	output +=  "!";// ((! ==<UNK>))

	for(; index < input.size(); index++)
	{
		if(iswspace(input[index]) || within_vector(input[index], _arabic_letters_without_tashkeel))
		{
			break;
		}
	}

	if (!iswspace(input[index]) && index == input.size() - 1)
		output += " ";

	index--;

	return output;
}
/*
 *return one unkown to backwalter "!" and save the opposite countinuous non-arabic chars
*/
string handle_unknown_char(const wstring &input, size_t &index,wstring& continuous_unkowns)
{
	assert(index < input.size());
	string output;
	continuous_unkowns = L"";
	output +=  "!";// ((! ==<UNK>))
	for(; index < input.size(); index++)
	{
		if(iswspace(input[index]) || within_vector(input[index], _arabic_letters_with_tashkeel))
		{
			break;
		}
		else
		{
			continuous_unkowns+=input[index];
		}
	}
	index--;
	return output;
}
string handle_spaces(const wstring &input, size_t &index,wstring& continuous_spaces)
{
	assert(index < input.size());
	string output;
	continuous_spaces = L"";
	output +=  " ";// ((! ==<UNK>))
	for(; index < input.size(); index++)
	{
		if(!iswspace(input[index]))
		{
			break;
		}
		else
		{
			continuous_spaces+=input[index];
		}
	}
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
			else if(buckwlater[i]=='!')// '!' == <UNK>
			{
				private_arabic += L"<UNK>";
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

wstring internal_convert_buckwalter_to_arabic(string buckwlater, bool tashkeel,const std::vector<wstring>& orignal_unknowns,
											  const std::vector<wstring>& orignal_wspaces)
{
	std::setlocale(LC_ALL, "en_US.UTF8"); // needed by the isspace and iswspace functions
	wstring arabic;
	int unk_index = 0;
	int wspace_index = 0;
	for (size_t i = 0; i < buckwlater.size(); i++)
	{
		if (isspace(buckwlater[i]))
		{
			arabic += orignal_wspaces[wspace_index++];
		}
		else if (within_vector(buckwlater[i],
							   tashkeel ? buckwalter_letters_with_tashkeel :
							   buckwalter_letters_without_tashkeel))
		{
			arabic += buckwalter_to_arabic.at(buckwlater[i]);
		}
		else if(buckwlater[i]=='!')// '!' == <UNK>
		{
			arabic += orignal_unknowns[unk_index++];
		}
	}
	return arabic;
}

} // namespace RDIInternal

using namespace RDIInternal;

wstring convert_buckwalter_to_arabic(string buckwlater)
{
	return internal_convert_buckwalter_to_arabic(buckwlater, true);
}
wstring convert_buckwalter_to_arabic(string buckwlater,const std::vector<wstring>&orignal_unknowns,const std::vector<wstring>& orignal_wspaces)
{
	return internal_convert_buckwalter_to_arabic(buckwlater, true,orignal_unknowns,orignal_wspaces);
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
		if (iswspace(buckwalter[i]))
		{
			output += buckwalter[i];
		}
		else if (within_vector(buckwalter[i], buckwalter_letters_without_tashkeel))
		{
			output += buckwalter[i];
		}
		else if(buckwalter[i]=='!') // '!' == <UNK>
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
					within_vector(arabic_with_tashkeel[i], _arabic_letters_without_tashkeel))
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
		else if (within_vector(arabic[i], _arabic_letters_with_tashkeel))
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

	for (size_t i = 0; i < arabic.size(); i++)
	{
		if (iswspace(arabic[i]))
		{
			buckwalter += convert_wspace_to_space(arabic[i]);
		}
		else if (within_vector(arabic[i], _arabic_letters_with_tashkeel))
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

bool is_all_tashekeel_or_empty(wstring &input)
{
	if(input.empty()) return true;
	for(size_t index = 0 ; index < input.size(); index++)
	{
		if(!within_vector(input[index], _arabic_tashkeel))
		{
			return false;
		}
	}
	return true;
}

string handle_tashkeel(const wstring &input, size_t &index)
{
	assert(index < input.size());
	string continuous_tshkeel;

	for(; index < input.size(); index++)
	{
		if( within_vector(input[index], _arabic_tashkeel))
		{
			continuous_tshkeel+= arabic_to_buckwalter.at(input[index]);
		}
		else
		{
			break;
		}
	}
	index--;

	return continuous_tshkeel;
}
/*
 *return backwlater and for each unkown  "!" it saves a list of the opposite countinuous non-arabic chars
*/
string convert_arabic_to_buckwalter_no_tahkeel(wstring arabic,std::vector<wstring>& unkown_chars ,
											   std::vector<wstring>& spaces , std::vector<string>& orignal_letter_formarion)
{
	std::setlocale(LC_ALL, "en_US.UTF8"); //needed by the isspace and iswspace functions
	string buckwalter;
	size_t sz = arabic.size();
	bool my_previous_is_aletter = false;
	for (size_t i = 0; i < sz; i++)
	{
		if (iswspace(arabic[i]))
		{
			if(my_previous_is_aletter)
			{
				orignal_letter_formarion.push_back("");//adding tashkeel for the previous letter
				my_previous_is_aletter = false;
			}
			wstring continuous_spaces;
			buckwalter += handle_spaces(arabic,i,continuous_spaces);
			spaces.push_back(continuous_spaces);
			orignal_letter_formarion.push_back("");
		}

		else if (within_vector(arabic[i], _arabic_letters_without_tashkeel))
		{

			buckwalter += arabic_to_buckwalter.at(arabic[i]);
			if(my_previous_is_aletter)
			{
				orignal_letter_formarion.push_back("");
			}
			if(i==sz-1)
			{
				orignal_letter_formarion.push_back(""); // handel if last element with no tashkeel
			}
			my_previous_is_aletter = true;
		}
		else  if (within_vector(arabic[i], _arabic_tashkeel))
		{
			if(my_previous_is_aletter)
			{
				string letter_tashkeel = handle_tashkeel(arabic,i);
				orignal_letter_formarion.push_back(letter_tashkeel);
				my_previous_is_aletter = false;
			}
		}
		else
		{
			if(my_previous_is_aletter)
			{
				orignal_letter_formarion.push_back("");
				my_previous_is_aletter = false;
			}
			wstring continuous_unkowns;
			buckwalter +=handle_unknown_char(arabic, i,continuous_unkowns);
			unkown_chars.push_back(continuous_unkowns);
			orignal_letter_formarion.push_back("");
		}
	}

	return buckwalter;
}
} // namespace RDI
