#ifndef _CELLTimestamp_hpp_
#define _CELLTimestamp_hpp_
#include <chrono>

class CELLTIMEStamp
{
public:
	CELLTIMEStamp() 
	{
		Update();
	}
	~CELLTIMEStamp() {}
	void Update()
	{
		m_begin = std::chrono::high_resolution_clock::now();
	}
	// ��ȡ��ǰ΢��
	long long getElapsedTimeInMicroSec()
	{
		return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - m_begin).count();
	}
	// ��ȡ��ǰ����
	double getElapsedTimeInMilliSec()
	{
		return this->getElapsedTimeInMicroSec() * 0.001;
	}
	//��ȡ��ǰ��
	double getElapsedSecond()
	{
		return getElapsedTimeInMicroSec() * 0.000001;
	}
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_begin;
};
#endif // !_CELLTimestamp_hpp_