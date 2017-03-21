#pragma once
#include <vector>
#include <memory>
#include <map>
#include <set>

class QueryResult;

class TextQuery
{
public:
	TextQuery();
	~TextQuery();

	using line_no = std::vector<std::string>::size_type;
	TextQuery(std::ifstream&);
	QueryResult query(const std::string&) const;

private:
	std::shared_ptr<std::vector<std::string>> file;
	std::map<std::string, std::shared_ptr<std::set<line_no>>> wm;
};

class QueryResult
{

};

