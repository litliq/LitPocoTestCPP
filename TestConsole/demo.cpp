
#include <iostream>
#include "../LtPocoXmlTest/XmlFrameFaceData.h"


int main()
{
	int iTest = XmlFrameFaceData::instance().LoadByXmlPath("./811.xml");
	std::cout << "Ret: " << iTest << std::endl << std::endl;

/*
	std::string sOneFaceData;
	iTest = XmlFrameFaceData::instance().GetOneFaceDataByFrameIdAndFaceId(1, 0, sOneFaceData);
	std::cout << "GetOneFaceDataByFrameIdAndFaceId: " << iTest << std::endl;
	std::cout << sOneFaceData << std::endl << std::endl;*/

	std::string sAllFaceData;
	iTest = XmlFrameFaceData::instance().GetAllFaceDataByFrameId(1, sAllFaceData);
	std::cout << "GetAllFaceDataByFrameId: " << iTest << std::endl;
	std::cout << sAllFaceData << std::endl << std::endl;

#ifdef WIN32
	system("pause");
#endif // WIN32

	return 0;
}


