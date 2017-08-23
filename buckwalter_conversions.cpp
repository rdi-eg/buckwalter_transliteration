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

string handle_unknown_char(const wstring &input, size_t index, ReplaceNonArabic replace_policy)
{
	if (replace_policy == ReplaceNonArabic::Delete)
		return "";

	assert(index < input.size());
	string output;

	if (index > 0 && !iswspace(input[index - 1]))
		output += " ";

	output += "<UNK>";

	if(index + 1 < input.size() && !iswspace(input[index + 1]))
		output += " ";

	return output;
}

string RDI::convert_arabic_to_buckwalter(wstring arabic, ReplaceNonArabic replace_policy)
{
	std::setlocale(LC_ALL, "en_US.UTF8"); //needed by the isspace and iswspace functions
	string buckwalter;
	map<int, string> index_buckwalter;

#pragma omp parallel
{
	string private_buckwalter;
	int last_index = 0;
	#pragma omp for
	for (size_t i = 0; i < arabic.size(); i++)
	{
		if (iswspace(arabic[i]))
			private_buckwalter += convert_wspace_to_space(arabic[i]);
		else if (within_vector(arabic[i], arabic_letters_with_tashkeel))
			private_buckwalter += arabic_to_buckwalter.at(arabic[i]);
		else if (iswspace(arabic[i+1]) || within_vector(arabic[i+1], arabic_letters_with_tashkeel))
			private_buckwalter += handle_unknown_char(arabic, i, replace_policy);
		last_index = i;
	}

	#pragma omp critical
	index_buckwalter[last_index - 1] = private_buckwalter;
}

	for (auto element : index_buckwalter)
		buckwalter += element.second;

	return buckwalter;
}

wstring RDI::convert_buckwalter_to_arabic(string buckwlater)
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
			private_arabic += convert_space_to_wspace(buckwlater[i]);
		else if (within_vector(arabic[i], arabic_letters_with_tashkeel))
			private_arabic += arabic_to_buckwalter.at(buckwlater[i]);

		last_index = i;
	}

	#pragma omp critical
	index_arabic[last_index - 1] = private_arabic;
}

	for (auto element : index_arabic)
		arabic += element.second;

	return arabic;
}
