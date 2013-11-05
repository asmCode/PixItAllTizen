#include "StrUtils.h"

void StrUtils::Split(const std::string &src,
					 const std::string &separator,
					 std::vector<std::string> &strParts)
{
	strParts.clear();
	
	std::string _src = src;
	int offset = 0;
	
	while (offset != -1)
	{
		offset = _src.find_first_of(separator);
		if (offset != -1)
		{
			strParts.push_back(_src.substr(0, offset));
			_src = _src.substr(offset + separator.length());
		}
	}
}
