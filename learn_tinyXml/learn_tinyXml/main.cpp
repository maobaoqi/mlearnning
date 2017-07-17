#include "tinyxml2.h"

using namespace tinyxml2;

void build_simple_doc()
{
	XMLDocument doc;
	XMLDeclaration* decl = new XMLDeclaration("1.0", "", "");
}


int main(int argc, char** argv)
{
	XMLDocument doc;
	doc.LoadFile("demo.xml");

	return 0;
}