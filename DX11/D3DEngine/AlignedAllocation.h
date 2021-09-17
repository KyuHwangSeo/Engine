#pragma once
#include <malloc.h>
#include <stdio.h>

// warning C4316 ó����..
/// DirectX ������ ���� �κп��� __declspec(align(16)) ���� 16 ����Ʈ ������ ����Ѵٰ� ��õǾ� �־�
/// ���� ���Ǵ� �����͸� Ư�� ���μ��� ĳ�ö��� ũ��� �����Ͽ� �Ѳ����� ����Ͽ� CPU ĳ�� ó�� ������ �����شٰ� �Ѵ�..
/// ���� ���� ������ ���ؿ� �ø��� ����� ���µ� �� �Ҵ�ÿ� ������ �߻��ϹǷ� ó���� �ʿ��ϴ�..
/// 
/// - ������ ���� �ʰ� ���ÿ� ������ -
///  -> CPU���� SSE ��ɾ� ������ ó���Ҷ� ���ĵǴ� �����͸� �����ϰ� ���� ���� �� ����..
///  -> ��, �������� ���� new, delete �� �������Ͽ� �������..

template<size_t T>

class AlignedAllocation
{
public:
	static void* operator new(size_t size)
	{
		return _aligned_malloc(size, T);
	}

	static void operator delete(void* memory)
	{
		_aligned_free(memory);
	}
};