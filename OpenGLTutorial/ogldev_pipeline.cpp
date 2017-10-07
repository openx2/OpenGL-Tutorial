#include "stdafx.h"
#include "ogldev_pipeline.h"

const Matrix4f&  Pipeline::getWPTrans()
{
	getWorldTrans();
	getProjTrans();

	m_WPtransformation = m_ProjTransformation * m_Wtransformation;  //���������а�����ڵ�����λ�ã�Ȼ�����ͶӰ
	return m_WPtransformation;
}

const Matrix4f&  Pipeline::getVPTrans()
{
	getViewTrans();
	getProjTrans();

	m_VPtransformation = m_ProjTransformation * m_Vtransformation; //�Ȱں����λ�ã�Ȼ��ͶӰ
	return m_VPtransformation;
}

const Matrix4f&  Pipeline::getWVTrans()
{
	getWorldTrans();
	getViewTrans();

	m_WVtransformation = m_Vtransformation * m_Wtransformation; //�ȵ��������������е�λ�ã�Ȼ��������λ��
	return m_WVtransformation;
}

const Matrix4f& Pipeline::getWVPTrans()
{
	getWorldTrans();
	getVPTrans();

	m_WVPtransformation = m_VPtransformation * m_Wtransformation; //�ȵ��������������е�λ�ã�Ȼ��������λ�ã�������͸��ͶӰ
	return m_WVPtransformation;
}

const Matrix4f&  Pipeline::getWorldTrans()
{
	Matrix4f scaleTrans, rotateTrans, translationTrans;

	scaleTrans.InitScaleTransform(m_scale);
	rotateTrans.InitRotateTransform(m_rotateInfo);
	translationTrans.InitTranslationTransform(m_worldPos);
	m_Wtransformation = translationTrans * rotateTrans * scaleTrans; //���󸴺ϵ�˳����Ӳ����ģ���Ϊ��ƽ������ת�ᵼ�¶���ƽ��
	return m_Wtransformation;
}

const Matrix4f&  Pipeline::getViewTrans()
{
	Matrix4f cameraTranslationTrans, cameraRotationTrans;

	cameraTranslationTrans.InitTranslationTransform(-m_camera.pos.x, -m_camera.pos.y, -m_camera.pos.z);
	cameraRotationTrans.InitCameraTransform(m_camera.target, m_camera.up);
	m_Vtransformation = cameraRotationTrans * cameraTranslationTrans; //���󸴺ϵ�˳����Ӳ����ģ��Ƚ����ƽ�Ƶ�ԭ�㣬Ȼ������ת��z��������
	return m_Vtransformation;
}

const Matrix4f&  Pipeline::getProjTrans()
{
	m_ProjTransformation.InitPersProjTransform(m_persProjInfo);
	return m_ProjTransformation;
}