#pragma once

#include "FMSpace/AirFlow.h"

//"���������" ����������� ��������� ������ ����:
//	- ������� ���������� 
//	- ����������
//	- ������ ��������
//	- �������
//	- �����

//	���������� � ������� ������� ��������� ������:
//	���������� - � ���� ��������� "������� ������ = f(������.�������,������.������ �������)",
//	������� - ����������� "������.������ ������� = f(������� ����������)" � ������ "��� = f(����.����.����., ����.������.�������)"
//  � ������������ ��� ���������� �������� ������� (���������� �� ������ �������� ���������� ������� �� ������)
//	����������� (����������������) ���������


namespace EagleFM {

struct AirFlow;

class  IJetElement //����� ���������
{
public:
	IJetElement() { AirFlowExt_.Temperature = 288.0; }
	virtual	~IJetElement() {}

	void	setExternAirFlow(const AirFlow& air_flow) { AirFlowExt_ = air_flow; }
	void	resetExternAirFlow() { 
		AirFlowExt_.Mass = 0.0; 
		AirFlowExt_.Temperature = 288.0; 
		AirFlowExt_.Pressure = PressureMCA_SL;	
	}

protected:

	AirFlow AirFlowExt_; //� ������ �������� ����� �������� �������� ������
};

}