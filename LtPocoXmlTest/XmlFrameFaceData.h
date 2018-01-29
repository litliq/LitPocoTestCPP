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


// [1/26/2018 Lit]: ��дxml�����ļ�
class LT_POCO_XML_API XmlFrameFaceData
{
public:
	XmlFrameFaceData(void);
	~XmlFrameFaceData(void);

	static XmlFrameFaceData& instance();

public:
	int LoadByXmlPath(const std::string &sXmlPath);

	// ͨ����Ƶ֡id������id��ȡ��������
	int GetOneFaceDataByFrameIdAndFaceId(int iFrameId, int iFaceId, std::string &sJson);

	// ͨ����Ƶ֡id������id��ȡ��������
	int GetAllFaceDataByFrameId(int iFrameId, std::string &sJson);

private:
	// ���������Ϣ
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

	// ����������Ϣ
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

	// ����������Ϣ
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

	// ����xml������
	struct StFrameFaceData
	{
		StFaceDetectInfo m_stDetInfo;
		StFaceAttribute  m_stAttribute;
		StFaceQuality    m_stQuality;
	};

	typedef std::vector<StFrameFaceData> VecOneFrameFace;
	typedef std::map<int, VecOneFrameFace> MapAllFrameData;    // ��Ϊ֡��ţ�ֵΪ��һ֡��������������


	// ͨ��tag��ȡֵ
	std::string GetValByTag(Poco::XML::Node *node, const std::string &sTag);

	// ��ȡ���������Ϣ
	int GetFaceDetInfo(Poco::XML::Node *node, StFaceDetectInfo &stDetInfo);

	// ��ȡ������Ϣ
	int GetFaceAttriInfo(Poco::XML::Node *node, StFaceAttribute &stAttriInfo);

	// ��ȡ������Ϣ
	int GetFaceQualityInfo(Poco::XML::Node *node, StFaceQuality &stQualityInfo);

	bool FormatOneFaceJson(const StFrameFaceData &stOneData, const std::string &sFrameId, int iFaceId, Poco::Util::JSONConfiguration &jsonData);
	

private:

	MapAllFrameData m_vecXmlData;
	std::string m_sVideoName;
};

