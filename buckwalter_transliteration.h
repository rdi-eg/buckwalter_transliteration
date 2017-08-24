#ifndef BUCKWALTER_TRANSLITERATION_H
#define BUCKWALTER_TRANSLITERATION_H

#include <ar_letter_definitions.h>
#include <map>
#include <vector>

namespace RDI
{

extern const std::map<wchar_t, char> arabic_to_buckwalter;
extern const std::map<char, wchar_t> buckwalter_to_arabic;
extern const std::vector<char> buckwalter_letters_with_tashkeel;
extern const std::vector<char> buckwalter_letters_without_tashkeel;
extern const std::vector<char> char_spaces;
extern const std::vector<wchar_t> wspaces;
extern const std::map<char, wchar_t> space_to_wspace;
extern const std::map<wchar_t, char> wspace_to_space;

}

// HAMZA             -> 	'\''
// ALEF_MADDA        -> 	'|'
// ALEF_HAMZA_ABOVE  -> 	'>'
// WAW_HAMZA_ABOVE   -> 	'&'
// ALEF_HAMZA_BELOW  -> 	'<'
// YEH_HAMZA_ABOVE   -> 	'}'
// ALEF_NO_HAMZA     -> 	'A'
// BEH               -> 	'b'
// TEH_MARBOOTA      -> 	'p'
// TEH               -> 	't'
// THEH              -> 	'v'
// JEEM              -> 	'j'
// HAH               -> 	'H'
// KHAH              -> 	'x'
// DAL               -> 	'd'
// THAL              -> 	'*'
// REH               -> 	'r'
// ZAIN              -> 	'z'
// SEEN              -> 	's'
// SHEEN             -> 	'$'
// SAD               -> 	'S'
// DAD               -> 	'D'
// TAH               -> 	'T'
// ZAH               -> 	'Z'
// AIN               -> 	'E'
// GHAIN             -> 	'g'
// FEH               -> 	'f'
// QAF               -> 	'q'
// KAF               -> 	'k'
// LAM               -> 	'l'
// MEEM              -> 	'm'
// NOON              -> 	'n'
// HEH               -> 	'h'
// WAW               -> 	'w'
// YEH_NO_DOTS       -> 	'Y'
// YEH_WITH_DOTS     -> 	'y'
// TATWEEL           -> 	'_'
// TANWEEN_FATHA     -> 	'F'
// TANWEEN_DAMA      -> 	'N'
// TANWEEN_KASRA     -> 	'K'
// FATHA             -> 	'a'
// DAMA              -> 	'u'
// KASRA             -> 	'i'
// SHADA             -> 	'~'
// SKOON             -> 	'o'

#endif // BUCKWALTER_TRANSLITERATION_H
