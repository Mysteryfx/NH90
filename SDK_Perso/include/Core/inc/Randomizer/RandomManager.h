#ifndef __Common_Random_Manager_h__
#define __Common_Random_Manager_h__
#include "_config.h"

// Менеджер псевдослучайных последовательностей
// Пример использования
// globalUniformRandom = globalRandomManager()->createUniform(78954602, 2000, 20, 6.0f);
class RandomManager
{
public:
	typedef ed::vector<float> sequence_t;
	typedef const ed::vector<float> const_sequence_t;

	ED_CORE_EXTERN RandomManager();
	ED_CORE_EXTERN virtual ~RandomManager();		// Уничтожает все последовательности
	ED_CORE_EXTERN void remove(const_sequence_t *seq);	// Уничтожение заданной последовательности
	ED_CORE_EXTERN void add(const_sequence_t *seq);		// Добавление готовой последовательности в список
	// Создание равномерной на (0, 1) последовательности заданного размера и
	// качества (хи-квадрат) с включением в список
	ED_CORE_EXTERN const_sequence_t *createUniform(
		unsigned int seed,			// Начальное значение для стандартного генератора rand() 
		unsigned int size,			// Количество чисел в последовательности
		unsigned int intervals,		// Количество доверительных интервалов (степеней свободы + 1)
		float criteria);			// Максимально допустимое значение хи-квадрат статистики
	// Создает стандартную нормальную последовательность на основе заданной равномерной
	ED_CORE_EXTERN const_sequence_t* createNormal(const_sequence_t *seq);
	// Создает последовательность с заданным законом распределения f на основе заданной равномерной
	ED_CORE_EXTERN const_sequence_t* createCustom(const_sequence_t *seq, float (*f)(float));

	// reference sequences
	const_sequence_t* refUniformRandom() const { return _ref_uniform; }
	const_sequence_t* refNormalRandom() const { return _ref_normal; }

private:
	ed::list<const_sequence_t*> seqlist;		// Список последовательностей
	const_sequence_t* _ref_uniform;
	const_sequence_t* _ref_normal;
};

extern ED_CORE_EXTERN RandomManager* globalRandomManager();

#endif /* __Common_Random_Manager_h__ */
