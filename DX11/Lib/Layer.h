#pragma once
#include <map>

///
/// 2021/08/12 12:56
/// SeoKyuHwang
///
/// Layer Class
///
/// - 오브젝트 간 충돌 등 Layer를 나누어 처리하기 위한 클래스
/// - LayerList는 전역으로 사용한다

class Layer
{
public:
	Layer(int key = 0, string name = "Defalt") : m_LayerKey(key), m_LayerName(name) {}
	~Layer() = default;

public:
	int m_LayerKey;
	string m_LayerName;

	//bool operator==(const Layer& layer)
	//{
	//	return (m_LayerKey == layer.m_LayerKey) ? true : false;
	//}
};

class LayerList
{
public:
	static void Initialize();
	static void Release();

	static void AddLayer(Layer* newLayer);
	static void AddLayer(string layerName);
	static Layer* GetLayer(int layerKey = 0);
	static void RemoveLayer(int layerKey);

private:
	static std::vector<Layer*> g_LayerList;
};
