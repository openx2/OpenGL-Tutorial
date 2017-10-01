#include "stdafx.h"
#include "ogldev_pipeline.h"

const Matrix4f * Pipeline::getTrans()
{
	Matrix4f scaleTrans, rotateTrans, translationTrans;

	scaleTrans.InitScaleTransform(m_scale);
	rotateTrans.InitRotateTransform(m_rotateInfo);
	translationTrans.InitTranslationTransform(m_worldPos);
	m_transformation = translationTrans*rotateTrans*scaleTrans; //���󸴺ϵ�˳����Ӳ����ģ���Ϊ��ƽ������ת�ᵼ�¶���ƽ��
	return &m_transformation;
}
