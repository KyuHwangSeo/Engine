#include <vector>
#include "BoneWeights.h"

BoneWeights::BoneWeights()
	:sumWeight(0.0f)
{

}

BoneWeights::~BoneWeights()
{
	m_BoneWeights.clear();
}

void BoneWeights::AddBoneWeight(int boneIndex, float boneWeight)
{
	if (boneWeight <= 0.0f)
		return;

	sumWeight += boneWeight;

	m_BoneWeights.push_back(new Weight(boneIndex, boneWeight));
}

void BoneWeights::AddBoneWeight(Weight* boneWeightPair)
{
	AddBoneWeight(boneWeightPair->m_BoneNumber, boneWeightPair->m_BoneWeight);
}

void BoneWeights::AddBoneWeights(const BoneWeights& boneWeights)
{
	for (size_t i = 0; i < boneWeights.m_BoneWeights.size(); ++i)
	{
		AddBoneWeight(boneWeights.m_BoneWeights[i]);
	}
}

void BoneWeights::Validate()
{
	sumWeight = 0.0f;
	int i = 0;

	std::vector<Weight*>::iterator it = m_BoneWeights.begin();
	while (it != m_BoneWeights.end())
	{
		sumWeight += (*it)->m_BoneWeight;
		++it;
	}
}

void BoneWeights::Normalize()
{
	Validate();

	float fScale = 1.0f / sumWeight;

	std::vector<Weight*>::iterator it = m_BoneWeights.begin();
	while (it != m_BoneWeights.end())
	{
		(*it)->m_BoneWeight *= fScale;
		++it;
	}
}
