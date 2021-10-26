#pragma once
#include <malloc.h>
#include <stdio.h>

// warning C4316 처리용..
/// DirectX 변수의 정의 부분에서 __declspec(align(16)) 으로 16 바이트 정렬을 사용한다고 명시되어 있어
/// 자주 사용되는 데이터를 특정 프로세서 캐시라인 크기로 정렬하여 한꺼번에 계산하여 CPU 캐시 처리 성능을 높혀준다고 한다..
/// 위의 경우는 변수를 스텍에 올리면 상관이 없는데 힙 할당시에 문제가 발생하므로 처리가 필요하다..
/// 
/// - 재정의 하지 않고 사용시에 문제점 -
///  -> CPU에서 SSE 명령어 연산을 처리할때 정렬되는 데이터를 안전하게 보장 받을 수 없다..
///  -> 즉, 안전성을 위해 new, delete 를 재정의하여 사용하자..

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