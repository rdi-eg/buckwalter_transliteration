#include "buckwalter_transliteration.h"

namespace RDI
{

extern const std::map<wchar_t, char> arabic_to_buckwalter = {
	{HAMZA           ,	'\''},
	{ALEF_MADDA      ,	'|'	},
	{ALEF_HAMZA_ABOVE,	'>'	},
	{WAW_HAMZA_ABOVE ,	'&'	},
	{ALEF_HAMZA_BELOW,	'<'	},
	{YEH_HAMZA_ABOVE ,	'}'	},
	{ALEF_NO_HAMZA   ,	'A'	},
	{BEH             ,	'b'	},
	{TEH_MARBOOTA    ,	'p'	},
	{TEH             ,	't'	},
	{THEH            ,	'v'	},
	{JEEM            ,	'j'	},
	{HAH             ,	'H'	},
	{KHAH            ,	'x'	},
	{DAL             ,	'd'	},
	{THAL            ,	'*'	},
	{REH             ,	'r'	},
	{ZAIN            ,	'z'	},
	{SEEN            ,	's'	},
	{SHEEN           ,	'$'	},
	{SAD             ,	'S'	},
	{DAD             ,	'D'	},
	{TAH             ,	'T'	},
	{ZAH             ,	'Z'	},
	{AIN             ,	'E'	},
	{GHAIN           ,	'g'	},
	{FEH             ,	'f'	},
	{QAF             ,	'q'	},
	{KAF             ,	'k'	},
	{LAM             ,	'l'	},
	{MEEM            ,	'm'	},
	{NOON            ,	'n'	},
	{HEH             ,	'h'	},
	{WAW             ,	'w'	},
	{YEH_NO_DOTS     ,	'Y'	},
	{YEH_WITH_DOTS   ,	'y'	},
	{TATWEEL         ,	'_'	},
	{TANWEEN_FATHA   ,	'F'	},
	{TANWEEN_DAMA    ,	'N'	},
	{TANWEEN_KASRA   ,	'K'	},
	{FATHA           ,	'a'	},
	{DAMA            ,	'u'	},
	{KASRA           ,	'i'	},
	{SHADA           ,	'~'	},
	{SKOON           ,	'o'	}
 };

extern const std::map<char, wchar_t> buckwalter_to_arabic = {
	{	'\'',	HAMZA           },
	{	'|'	,	ALEF_MADDA      },
	{	'>'	,	ALEF_HAMZA_ABOVE},
	{	'&'	,	WAW_HAMZA_ABOVE },
	{	'<'	,	ALEF_HAMZA_BELOW},
	{	'}'	,	YEH_HAMZA_ABOVE },
	{	'A'	,	ALEF_NO_HAMZA   },
	{	'b'	,	BEH             },
	{	'p'	,	TEH_MARBOOTA    },
	{	't'	,	TEH             },
	{	'v'	,	THEH            },
	{	'j'	,	JEEM            },
	{	'H'	,	HAH             },
	{	'x'	,	KHAH            },
	{	'd'	,	DAL             },
	{	'*'	,	THAL            },
	{	'r'	,	REH             },
	{	'z'	,	ZAIN            },
	{	's'	,	SEEN            },
	{	'$'	,	SHEEN           },
	{	'S'	,	SAD             },
	{	'D'	,	DAD             },
	{	'T'	,	TAH             },
	{	'Z'	,	ZAH             },
	{	'E'	,	AIN             },
	{	'g'	,	GHAIN           },
	{	'f'	,	FEH             },
	{	'q'	,	QAF             },
	{	'k'	,	KAF             },
	{	'l'	,	LAM             },
	{	'm'	,	MEEM            },
	{	'n'	,	NOON            },
	{	'h'	,	HEH             },
	{	'w'	,	WAW             },
	{	'Y'	,	YEH_NO_DOTS     },
	{	'y'	,	YEH_WITH_DOTS   },
	{	'_'	,	TATWEEL         },
	{	'F'	,	TANWEEN_FATHA   },
	{	'N'	,	TANWEEN_DAMA    },
	{	'K'	,	TANWEEN_KASRA   },
	{	'a'	,	FATHA           },
	{	'u'	,	DAMA            },
	{	'i'	,	KASRA           },
	{	'~'	,	SHADA           },
	{	'o'	,	SKOON           }
 };

extern const std::vector<char> char_spaces = {
	' ' ,
	'\f',
	'\n',
	'\r',
	'\t',
	'\v'
};

extern const std::vector<wchar_t> wspaces = {
	L' ' ,
	L'\f',
	L'\n',
	L'\r',
	L'\t',
	L'\v'
};

extern const std::map<char, wchar_t> space_to_wspace = {
	{	' ',	L' '   },
	{	'\f',	L'\f'  },
	{	'\n',	L'\n'  },
	{	'\r',	L'\r'  },
	{	'\t',	L'\t'  },
	{	'\v',	L'\v'  }
};

extern const std::map<wchar_t, char> wspace_to_space = {
	{	L' ',	' '   },
	{	L'\f',	'\f'  },
	{	L'\n',	'\n'  },
	{	L'\r',	'\r'  },
	{	L'\t',	'\t'  },
	{	L'\v',	'\v'  }
};

}
