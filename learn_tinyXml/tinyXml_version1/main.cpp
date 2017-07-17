#include "tinystr.h"
#include "tinyxml.h"

#include <stdlib.h>

// load the named file and dump its structure to STDOUT
void dump_to_stdout(const char* pFilename)
{
	TiXmlDocument doc(pFilename);
	bool loadOkay = doc.LoadFile();
	if (loadOkay)
	{
		printf("\n%s:\n", pFilename);
		//dump_to_stdout( &doc ); // define later
	}
	else
	{
		printf("Failed to load file \"%s\"\n", pFilename);
	}
}


void build_simple_doc()
{
	// make xml: <?xml..><Hello>World</Hello>
	TiXmlDocument doc;
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
	TiXmlElement* element = new TiXmlElement("Hello");
	TiXmlText* text = new TiXmlText("World");

	element->LinkEndChild(text);
	doc.LinkEndChild(decl);
	doc.LinkEndChild(element);

	doc.SaveFile("madeByHand.xml");
}


void write_simple_doc2()
{
	// same as write simple doc1 but add each node
	// as early as possible into the tree
	TiXmlDocument doc;
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);

	TiXmlElement* element = new TiXmlElement("Hello");
	doc.LinkEndChild(element);

	TiXmlText* text = new TiXmlText("World");
	element->LinkEndChild(text);

	doc.SaveFile("madeByHand2.xml");
}


int dump_attribs_to_stdout(TiXmlElement* pElement, unsigned int indent)
{
	if (!pElement) return 0;

	TiXmlAttribute* pAttrib = pElement->FirstAttribute();
	int i = 0;
	int ival;
	double dval;
	//const char* pIndent = 
}


void write_app_setting_doc()
{
	TiXmlDocument doc;
	TiXmlElement* msg;
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);

	TiXmlElement* root = new TiXmlElement("MyAPP");
	doc.LinkEndChild(root);
	
	TiXmlComment* comment = new TiXmlComment();
	comment->SetValue("Settings for MyAPP");
	root->LinkEndChild(comment);

	TiXmlElement* msgs = new TiXmlElement("Messages");
	root->LinkEndChild(msgs);

	msg = new TiXmlElement("Welcome");
	msg->LinkEndChild(new TiXmlText("Welcome to MyAPP"));
	msgs->LinkEndChild(msg);

	msg = new TiXmlElement("Farewell");
	msg->LinkEndChild(new TiXmlText("Thank you for using MyAPP"));
	msgs->LinkEndChild(msg);

	TiXmlElement* windows = new TiXmlElement("Windows");
	root->LinkEndChild(windows);

	TiXmlElement* window;
	window = new TiXmlElement("Window");
	windows->LinkEndChild(window);
	window->SetAttribute("name", "MainFrame");
	window->SetAttribute("x", 5);
	window->SetAttribute("y", 5);
	window->SetAttribute("w", 400);
	window->SetAttribute("h", 250);

	TiXmlElement* connections = new TiXmlElement("Connections");
	root->LinkEndChild(connections);

	TiXmlElement* cxn = new TiXmlElement("Connection");
	connections->LinkEndChild(cxn);
	//root->LinkEndChild(cxn);
	cxn->SetAttribute("ip", "192.168.0.1");
	cxn->SetDoubleAttribute("timeout", 123.456);

	doc.SaveFile("appsettings.xml");
}



int main(int argc, char** argv)
{
	//dump_to_stdout("example.xml");
	//build_simple_doc();
	//write_simple_doc2();
	write_app_setting_doc();

	return 0;
}