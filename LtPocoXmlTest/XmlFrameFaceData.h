#pragma once
#include "string"
#include "vector"
#include "map"
#include "Poco/DOM/Node.h"
#include "Poco/Util/JSONConfiguration.h"


#ifdef  LTPOCOXMLTEST_EXPORTS
#define LT_POCO_XML_API  __declspec(dllexport)
#else
#define LT_POCO_XML_API
#endif


// [1/26/2018 Lit]: 读写xml描述文件
class LT_POCO_XML_API XmlFrameFaceData
{
public:
	XmlFrameFaceData(void);
	~XmlFrameFaceData(void);

	static XmlFrameFaceData& instance();

public:
	int LoadByXmlPath(const std::string &sXmlPath);

	// 通过视频帧id和人脸id获取人脸数据
	int GetOneFaceDataByFrameIdAndFaceId(int iFrameId, int iFaceId, std::string &sJson);

	// 通过视频帧id和人脸id获取人脸数据
	int GetAllFaceDataByFrameId(int iFrameId, std::string &sJson);

private:
	// 人脸检测信息
	struct StFaceDetectInfo
	{
		std::string m_sPosX;
		std::string m_sPosY;
		std::string m_sWidth;
		std::string m_sHeight;
		std::string m_sLEyeX;
		std::string m_sLEyeY;
		std::string m_sREyeX;
		std::string m_sREyeY;
		std::string m_sMouthX;
		std::string m_sMouthY;
	};

	// 人脸属性信息
	struct StFaceAttribute
	{
		std::string m_sGender;
		std::string m_sAge;
		std::string m_sSkinCol;
		std::string m_sExpression;
		std::string m_sYaw;
		std::string m_sPitch;
		std::string m_sRoll;
	};

	// 人脸质量信息
	struct StFaceQuality
	{
		std::string m_sTotalScore;
		std::string m_sClarity;
		std::string m_sBrightness;
		std::string m_sMouthOpening;
		std::string m_sLEyeOpening;
		std::string m_sREyeOpening;
		std::string m_sGlassProb;
		std::string m_sSunglassProb;
		std::string m_sMouthMask;
	};

	// 所有xml中数据
	struct StFrameFaceData
	{
		StFaceDetectInfo m_stDetInfo;
		StFaceAttribute  m_stAttribute;
		StFaceQuality    m_stQuality;
	};

	typedef std::vector<StFrameFaceData> VecOneFrameFace;
	typedef std::map<int, VecOneFrameFace> MapAllFrameData;    // 键为帧序号，值为这一帧的所有人脸数据


	// 通过tag获取值
	std::string GetValByTag(Poco::XML::Node *node, const std::string &sTag);

	// 获取人脸检测信息
	int GetFaceDetInfo(Poco::XML::Node *node, StFaceDetectInfo &stDetInfo);

	// 获取属性信息
	int GetFaceAttriInfo(Poco::XML::Node *node, StFaceAttribute &stAttriInfo);

	// 获取质量信息
	int GetFaceQualityInfo(Poco::XML::Node *node, StFaceQuality &stQualityInfo);

	bool FormatOneFaceJson(const StFrameFaceData &stOneData, const std::string &sFrameId, int iFaceId, Poco::Util::JSONConfiguration &jsonData);
	

private:

	MapAllFrameData m_vecXmlData;
	std::string m_sVideoName;
};

