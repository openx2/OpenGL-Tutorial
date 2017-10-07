#include "stdafx.h"
#include "ogldev_pipeline.h"

const Matrix4f&  Pipeline::getWPTrans()
{
	getWorldTrans();
	getProjTrans();

	m_WPtransformation = m_ProjTransformation * m_Wtransformation;  //先在世界中把物体摆到合适位置，然后进行投影
	return m_WPtransformation;
}

const Matrix4f&  Pipeline::getVPTrans()
{
	getViewTrans();
	getProjTrans();

	m_VPtransformation = m_ProjTransformation * m_Vtransformation; //先摆好相机位置，然后投影
	return m_VPtransformation;
}

const Matrix4f&  Pipeline::getWVTrans()
{
	getWorldTrans();
	getViewTrans();

	m_WVtransformation = m_Vtransformation * m_Wtransformation; //先调好物体在世界中的位置，然后调整相机位置
	return m_WVtransformation;
}

const Matrix4f& Pipeline::getWVPTrans()
{
	getWorldTrans();
	getVPTrans();

	m_WVPtransformation = m_VPtransformation * m_Wtransformation; //先调好物体在世界中的位置，然后调整相机位置，最后进行透视投影
	return m_WVPtransformation;
}

const Matrix4f&  Pipeline::getWorldTrans()
{
	Matrix4f scaleTrans, rotateTrans, translationTrans;

	scaleTrans.InitScaleTransform(m_scale);
	rotateTrans.InitRotateTransform(m_rotateInfo);
	translationTrans.InitTranslationTransform(m_worldPos);
	m_Wtransformation = translationTrans * rotateTrans * scaleTrans; //矩阵复合的顺序是硬编码的，因为先平移再旋转会导致二次平移
	return m_Wtransformation;
}

const Matrix4f&  Pipeline::getViewTrans()
{
	Matrix4f cameraTranslationTrans, cameraRotationTrans;

	cameraTranslationTrans.InitTranslationTransform(-m_camera.pos.x, -m_camera.pos.y, -m_camera.pos.z);
	cameraRotationTrans.InitCameraTransform(m_camera.target, m_camera.up);
	m_Vtransformation = cameraRotationTrans * cameraTranslationTrans; //矩阵复合的顺序是硬编码的，先将相机平移到原点，然后将它旋转向z轴正方向
	return m_Vtransformation;
}

const Matrix4f&  Pipeline::getProjTrans()
{
	m_ProjTransformation.InitPersProjTransform(m_persProjInfo);
	return m_ProjTransformation;
}