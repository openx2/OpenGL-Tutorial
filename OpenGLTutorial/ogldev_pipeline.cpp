#include "stdafx.h"
#include "ogldev_pipeline.h"

const Matrix4f * Pipeline::getTrans()
{
	Matrix4f scaleTrans, rotateTrans, translationTrans, persProjTrans;

	scaleTrans.InitScaleTransform(m_scale);
	rotateTrans.InitRotateTransform(m_rotateInfo);
	translationTrans.InitTranslationTransform(m_worldPos);
	persProjTrans.InitPersProjTransform(m_persProjInfo);
	m_transformation = persProjTrans * translationTrans * rotateTrans * scaleTrans; //矩阵复合的顺序是硬编码的，因为先平移再旋转会导致二次平移
	return &m_transformation;
}
