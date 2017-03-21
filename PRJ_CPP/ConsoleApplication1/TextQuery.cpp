#include "TextQuery.h"


TextQuery::TextQuery()
{
}


TextQuery::~TextQuery()
{
}

TextQuery::TextQuery(std::ifstream &)
{
}

QueryResult TextQuery::query(const std::string &) const
{
	return QueryResult();
}
