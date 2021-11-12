#pragma once

/// <summary>
/// �������� Ÿ���� �Ѱ��� �Լ��� ��ȯ�ϱ� ���� ���� Class
/// �ش� Class�� Type�� �ݿ��Ͽ� ���� Class Operator�� ���� �������� Ÿ������ ��ȯ
/// </summary>

class GraphicResourceManager;
class BasicRenderTarget;
class ComputeRenderTarget;

class OriginalRenderTarget
{
public:
	GraphicResourceManager* pThis;
	eRenderTarget state;

public:
	operator BasicRenderTarget* ();
	operator ComputeRenderTarget* ();
};