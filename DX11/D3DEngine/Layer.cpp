#include "DeleteDefine.h"
#include "Object.h"
#include "Layer.h"

std::vector<Layer*> LayerList::g_LayerList;

void LayerList::Initialize()
{
	g_LayerList.push_back(new Layer());
}

void LayerList::Release()
{
	for (Layer* layer : g_LayerList)
	{
		SAFE_DELETE(layer);
	}
}

void LayerList::AddLayer(Layer* newLayer)
{
	newLayer->m_LayerKey = g_LayerList.size();
	g_LayerList.push_back(newLayer);
}

void LayerList::AddLayer(string layerName)
{
	Layer* newLayer = new Layer();
	newLayer->m_LayerKey = g_LayerList.size();
	newLayer->m_LayerName = layerName;
	g_LayerList.push_back(newLayer);
}

Layer* LayerList::GetLayer(int layerKey)
{
	return g_LayerList[layerKey];
}

void LayerList::RemoveLayer(int layerKey)
{
	// 해당 레이어 오브젝트 레이어 초기화 추가해야함..
	vector<Layer*>::iterator itor = g_LayerList.begin();

	unsigned int m_Index = 0;
	for (; itor != g_LayerList.end(); itor++)
	{
		if ((*itor)->m_LayerKey == layerKey)
		{
			g_LayerList.erase(itor++);
			break;
		}

		m_Index++;
	}

	for (;m_Index < g_LayerList.size(); m_Index++)
	{
		g_LayerList[m_Index]->m_LayerKey = m_Index;
	}
}