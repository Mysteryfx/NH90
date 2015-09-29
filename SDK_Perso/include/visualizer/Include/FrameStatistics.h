#pragma once
#include "ed/vector.h"
#include <functional>

//����� ���������� �� ��������, �������� �������� �� ������ ��������� timeDelta
class FrameStatistics
{
	typedef std::function<float()> GetterFunc;

	struct Item
	{
		ed::string	name;
		ed::vector<float> statistics;
		float		min, max;
		GetterFunc	getValue;
		Item(): min(99999999.f), max(-99999999.f) {}
	};

	long long startTime;
	long long lastTime;
	double time;//�������� ����� � ������� ������ ����� ����������
	double curTime;//������� ����� ���������
	double timeDelta;//����� ���������
	ed::vector<Item> items;//������� ��������
	ed::vector<unsigned> framesPerInterval;//���������� ������ � ������ ���������

public:
			FrameStatistics();
	void	setStartTime();
	//���������� ��� ����� � ���������� ����������, �� ��������� 1 �������
	void	setTimeDelta(double delta);

	//����������� ������������� ��������
	void	addItem(const char *name, GetterFunc getter);
	//���������� ED_watch
	void	addWatch(const char *name);

	//�������� ��������� ����� � ��������� �� ���������, ��� ������������� ������� ������� ��������� � ������ �����
	//������� ���������� ���� ��� �� ����.
	void	gatherFrame();

	void	clear();

	void	saveAsCSV(const char *filename);
};