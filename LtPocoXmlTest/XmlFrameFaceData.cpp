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
#include "sstream"


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

bool XmlFrameFaceData::FormatOneFaceJson(const StFrameFaceData &stOneData, const std::string &sFrameId, int iFaceId, Poco::Util::JSONConfiguration &jsonData)
{
	try
	{
		Poco::Util::JSONConfiguration jsPara;
		jsPara.setInt("track_id", -1);   

		//	<!-- 人脸检测信息，坐标均以整数表示，包含人脸位置坐标，左右眼中心坐标，嘴中心坐标 -->
		//<FaceDetectInfo>
		jsPara.setString("pos_x", stOneData.m_stDetInfo.m_sPosX);
		jsPara.setString("pos_y", stOneData.m_stDetInfo.m_sPosY);
		jsPara.setString("face_height", stOneData.m_stDetInfo.m_sHeight);
		jsPara.setString("face_width", stOneData.m_stDetInfo.m_sWidth);
		jsPara.setString("left_eye_x", stOneData.m_stDetInfo.m_sLEyeX);
		jsPara.setString("left_eye_y", stOneData.m_stDetInfo.m_sLEyeY);
		jsPara.setString("right_eye_x", stOneData.m_stDetInfo.m_sREyeX);
		jsPara.setString("right_eye_y", stOneData.m_stDetInfo.m_sREyeY);
		jsPara.setString("mouthX", stOneData.m_stDetInfo.m_sMouthX);
		jsPara.setString("mouthY", stOneData.m_stDetInfo.m_sMouthY);

		//	<!-- 人脸属性信息 -->
		//<FaceAttribut>
		jsPara.setString("sex", stOneData.m_stAttribute.m_sGender);
		jsPara.setString("age", stOneData.m_stAttribute.m_sAge);
		jsPara.setString("skin_color", stOneData.m_stAttribute.m_sSkinCol);
		//表情暂未实现
		jsPara.setString("expression", "smile");
		//<!--人脸角度，yaw深度旋转角，pitch上下旋转角，roll面内旋转角-->
		//<Position>
		jsPara.setString("pitch", stOneData.m_stAttribute.m_sPitch);  //上下旋转角
		jsPara.setString("yaw", stOneData.m_stAttribute.m_sYaw);    //深度旋转角
		jsPara.setString("roll", stOneData.m_stAttribute.m_sRoll);   //面内旋转角

		//<!-- 人脸质量信息 -->
		//<FaceQuality>
		jsPara.setString("quality_score", stOneData.m_stQuality.m_sTotalScore);
		//人脸清晰度
		jsPara.setString("fuzzy_rate", stOneData.m_stQuality.m_sClarity);
		//人脸亮度
		jsPara.setString("brightness_rate", stOneData.m_stQuality.m_sBrightness);
		//张嘴分数
		jsPara.setString("mouthscore", stOneData.m_stQuality.m_sMouthOpening);
		//左眼睁开分数
		jsPara.setString("lefteyescore", stOneData.m_stQuality.m_sLEyeOpening);
		//右眼睁开分数
		jsPara.setString("righteyescore", stOneData.m_stQuality.m_sREyeOpening);
		//是否戴眼镜
		jsPara.setString("blackglasses", stOneData.m_stQuality.m_sGlassProb);
		//是否戴墨镜
		jsPara.setString("sunglasses", stOneData.m_stQuality.m_sSunglassProb);
		//是否戴口罩
		jsPara.setString("maskscore", stOneData.m_stQuality.m_sMouthMask);

		jsPara.setString("background_color", "white");
		jsPara.setString("exposure_rate", "0.5");
		jsPara.setString("eye_color", "black");
		jsPara.setInt("eye_distance", (atoi(stOneData.m_stDetInfo.m_sREyeX.c_str()) - atoi(stOneData.m_stDetInfo.m_sLEyeX.c_str())));
		if (atof(stOneData.m_stQuality.m_sLEyeOpening.c_str()) >= 0.5 || atof(stOneData.m_stQuality.m_sREyeOpening.c_str()) >= 0.5)
		{
			jsPara.setString("eye_state", "open");
		}
		else
		{
			jsPara.setString("eye_state", "close");
		}	

		// 写入face_url	
		/*std::string url = getLocalIP();
		std::string facePath = VA_getFacePath(sFrameId, m_sVideoName);
		facePath += Poco::format("%s_%d.jpg", sFrameId, iFaceId);
		facePath = facePath.substr(1, facePath.length() - 1);
		url += facePath;
		std::string sFramepath = VA_getFramePath(sFrameId, m_sVideoName);
		sFramepath += sFrameId + ".jpg";*/

		std::string url = "192.168.60.44:8123/data/2018_1M23_15H18M45S/video_snap/1_4.jpg";
		std::string sFramepath = "./data/2018_1M23_15H18M45S/video_pic/1.jpg";
		jsPara.setString("face_path", "");
		jsPara.setString("face_url", url);
		jsPara.setString("frame_path", sFramepath);

		jsPara.setString("head_state", "rise");
		std::string jewelry;
		if(atof(stOneData.m_stQuality.m_sGlassProb.c_str()) >= 0.5 )
		{
			jewelry.append("blackglasses,");
		}
		if(atof(stOneData.m_stQuality.m_sSunglassProb.c_str()) >= 0.5 )
		{
			jewelry.append("sunglasses,");
		}
		if(atof(stOneData.m_stQuality.m_sMouthMask.c_str()) < 0.44)
		{
			jewelry.append("mask");
		}

		jsPara.setString("jewelry", jewelry);

		std::string mouthstate;
		if(atof(stOneData.m_stQuality.m_sMouthOpening.c_str()) >= 0.5)
		{
			jsPara.setString("mouth_state", "open");
		}
		else
		{
			jsPara.setString("mouth_state", "close");
		}	
		jsPara.setString("nation", "han");

		std::ostringstream ostr;
		jsPara.save(ostr);

		jsonData.setInt(Poco::format("snap[%d].snap_id", iFaceId), iFaceId);
		jsonData.setString(Poco::format("snap[%d].data", iFaceId), ostr.str());
	}
	catch (Poco::Exception &e)
	{
		return false;
	}

	return true;
}

int XmlFrameFaceData::GetOneFaceDataByFrameIdAndFaceId(int iFrameId, int iFaceId, std::string &sJson)
{
	MapAllFrameData::iterator itData = m_vecXmlData.find(iFrameId);
	if (itData == m_vecXmlData.end())
	{
		return -1;
	}

	if (iFaceId >= itData->second.size() || iFaceId < 0)
	{
		return -2;
	}

	Poco::Util::JSONConfiguration jsonConf;
	if (!FormatOneFaceJson(itData->second[iFaceId], Poco::format("%d", iFrameId), iFaceId, jsonConf))
	{
		sJson = "";
		return -3;
	}

	std::ostringstream ostr;
	jsonConf.save(ostr);
	sJson = ostr.str();
	return 0;
}

int XmlFrameFaceData::GetAllFaceDataByFrameId(int iFrameId, std::string &sJson)
{
	MapAllFrameData::iterator itData = m_vecXmlData.find(iFrameId);
	if (itData == m_vecXmlData.end())
	{
		return -1;
	}

	std::string sFrameId = Poco::format("%d", iFrameId);
	Poco::Util::JSONConfiguration jsonConf;
	VecOneFrameFace::iterator itOne = itData->second.begin();
	int iFaceId = 0;
	for (; itOne != itData->second.end(); itOne++)
	{
		FormatOneFaceJson(*itOne, sFrameId, iFaceId, jsonConf);
		iFaceId++;
	}

	std::ostringstream ostr;
	jsonConf.save(ostr);
	sJson = ostr.str();
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