
#include <iostream>
#include "../LtPocoXmlTest/XmlFrameFaceData.h"


int main()
{
	int iTest = XmlFrameFaceData::instance().LoadByXmlPath("./811.xml");
	std::cout << "Ret: " << iTest << std::endl;
#ifdef WIN32
	system("pause");
#endif // WIN32

	return 0;
}


