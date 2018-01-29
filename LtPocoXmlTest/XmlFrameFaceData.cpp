#include "XmlFrameFaceData.h"
#include "Poco/SingletonHolder.h"
#include <Poco/AutoPtr.h>
#include <Poco/Util/XMLConfiguration.h>
#include "Poco/DOM/NodeIterator.h"
#include "Poco/DOM/NodeFilter.h"
#include "Poco/DOM/DOMParser.h"
#include "Poco/DOM/Document.h"
#include "Poco/DOM/NodeList.h"
#include "Poco/DOM/NamedNodeMap.h"
#include "iostream"


XmlFrameFaceData::XmlFrameFaceData(void)
{
}


XmlFrameFaceData::~XmlFrameFaceData(void)
{
}

XmlFrameFaceData& XmlFrameFaceData::instance()
{
	static Poco::SingletonHolder<XmlFrameFaceData>sh;
	return *sh.get();
}

int XmlFrameFaceData::GetFrameDataByFrameIdAndFaceId(int iFrameId, int iFaceId, std::string &sJson)
{
	return 0;
}

std::string XmlFrameFaceData::GetValByTag(Poco::XML::Node *node, const std::string &sTag)
{
	if (!node)
	{
		return "";
	}

	try
	{
		Poco::XML::Node *tempNo = node->getNodeByPath(sTag);
		if (!tempNo)
		{
			return "";
		}
	//	std::string NoFace = tempNo->nodeName();
	//	std::string NoFaceVal = tempNo->nodeValue();

		Poco::XML::NodeList *oneNode = tempNo->childNodes();
		if (oneNode->length() < 1)
		{
			return "";
		}

		return oneNode->item(0)->nodeValue();
	}
	catch (Poco::Exception &e)
	{
		printf(e.displayText().c_str());
		printf("\n");
		return "";
	}
}

// 获取人脸检测信息
int XmlFrameFaceData::GetFaceDetInfo(Poco::XML::Node *node, StFaceDetectInfo &stDetInfo)
{
	if (!node)
	{
		return -1;
	}

	try
	{
		Poco::XML::Node *nodeDet = node->getNodeByPath("FaceDetectInfo");
		if (!nodeDet)
		{
			return -2;
		}

		stDetInfo.m_sPosX = GetValByTag(nodeDet, "PosX");
		stDetInfo.m_sPosY = GetValByTag(nodeDet, "PosY");
		stDetInfo.m_sWidth = GetValByTag(nodeDet, "Width");
		stDetInfo.m_sHeight = GetValByTag(nodeDet, "Height");
		stDetInfo.m_sLEyeX = GetValByTag(nodeDet, "LEyeX");
		stDetInfo.m_sLEyeY = GetValByTag(nodeDet, "LEyeY");
		stDetInfo.m_sREyeX = GetValByTag(nodeDet, "REyeX");
		stDetInfo.m_sREyeY = GetValByTag(nodeDet, "REyeY");
		stDetInfo.m_sMouthX = GetValByTag(nodeDet, "MouthX");
		stDetInfo.m_sMouthY = GetValByTag(nodeDet, "MouthY");

	}
	catch (Poco::Exception &e)
	{
		printf(e.displayText().c_str());
		printf("\n");
		return -3;
	}

	return 0;
}

// 获取属性信息
int XmlFrameFaceData::GetFaceAttriInfo(Poco::XML::Node *node, StFaceAttribute &stAttriInfo)
{
	if (!node)
	{
		return -1;
	}

	try
	{
		Poco::XML::Node *nodeAttri = node->getNodeByPath("FaceAttribut");
		if (!nodeAttri)
		{
			return -2;
		}

		stAttriInfo.m_sGender = GetValByTag(nodeAttri, "Gender");
		stAttriInfo.m_sAge = GetValByTag(nodeAttri, "Age");
		stAttriInfo.m_sSkinCol = GetValByTag(nodeAttri, "SkinCol");
		stAttriInfo.m_sExpression = GetValByTag(nodeAttri, "Expression");

		// 获取Position
		Poco::XML::Node *nodePos = nodeAttri->getNodeByPath("Position");
		stAttriInfo.m_sYaw = GetValByTag(nodePos, "yaw");
		stAttriInfo.m_sPitch = GetValByTag(nodePos, "pitch");
		stAttriInfo.m_sRoll = GetValByTag(nodePos, "roll");

	}
	catch (Poco::Exception &e)
	{
		printf(e.displayText().c_str());
		printf("\n");
		return -3;
	}

	return 0;
}

// 获取质量信息
int XmlFrameFaceData::GetFaceQualityInfo(Poco::XML::Node *node, StFaceQuality &stQualityInfo)
{
	if (!node)
	{
		return -1;
	}

	try
	{
		Poco::XML::Node *nodeQua = node->getNodeByPath("FaceQuality");
		if (!nodeQua)
		{
			return -2;
		}

		stQualityInfo.m_sTotalScore = GetValByTag(nodeQua, "TotalScore");
		stQualityInfo.m_sClarity = GetValByTag(nodeQua, "Clarity");
		stQualityInfo.m_sBrightness = GetValByTag(nodeQua, "TotalScore");
		stQualityInfo.m_sMouthOpening = GetValByTag(nodeQua, "MouthOpening");
		stQualityInfo.m_sLEyeOpening = GetValByTag(nodeQua, "LEyeOpening");
		stQualityInfo.m_sREyeOpening = GetValByTag(nodeQua, "REyeOpening");
		stQualityInfo.m_sGlassProb = GetValByTag(nodeQua, "GlassProb");
		stQualityInfo.m_sSunglassProb = GetValByTag(nodeQua, "SunglassProb");
		stQualityInfo.m_sMouthMask = GetValByTag(nodeQua, "MouthMask");

	}
	catch (Poco::Exception &e)
	{
		printf(e.displayText().c_str());
		printf("\n");
		return -3;
	}

	return 0;
}

// 通过路径读取xml文件
int XmlFrameFaceData::LoadByXmlPath(const std::string &sXmlPath)
{
	m_vecXmlData.clear();
	int iFaceNum = 0;

	try
	{
		Poco::XML::InputSource srcFile(sXmlPath);
		Poco::XML::DOMParser parser;           //创建一个分析者
		Poco::AutoPtr<Poco::XML::Document> pDoc = parser.parse(&srcFile);
		if (!pDoc)
		{
			return -1;
		}
		Poco::XML::Element* xmlRoot = pDoc->documentElement();
		if (!xmlRoot)
		{
			return -2;
		}

		Poco::XML::Element* xmlFaces = xmlRoot->getChildElement("Faces");
		if (!xmlFaces)
		{
			return -3;
		}
		Poco::XML::NodeList *faceNode = xmlFaces->childNodes();
		if (!faceNode)
		{
			return -4;
		}
		int iLength = faceNode->length();
		int iRet = 0;
		for (int i = 0; i < iLength; i++)
		{
			std::string sFaceTag = faceNode->item(i)->nodeName();
			std::string sFaceTagName = faceNode->item(i)->nodeValue();
			if (faceNode->item(i)->nodeType() != Poco::XML::Node::ELEMENT_NODE)
			{
				continue;
			}
			
			std::string sFrameId = GetValByTag(faceNode->item(i), "FrameNo");
			if (sFrameId.empty())
			{
				continue;
			}

			iFaceNum++;

			StFrameFaceData dataAll;
			iRet = GetFaceDetInfo(faceNode->item(i), dataAll.m_stDetInfo);
			iRet = GetFaceAttriInfo(faceNode->item(i), dataAll.m_stAttribute);
			iRet = GetFaceQualityInfo(faceNode->item(i), dataAll.m_stQuality);

			m_vecXmlData[atoi(sFrameId.c_str())].push_back(dataAll);
		}
	}
	catch (Poco::Exception &e)
	{
		printf(e.displayText().c_str());
		printf("\n");
		return -10;
	}

	return iFaceNum;
}