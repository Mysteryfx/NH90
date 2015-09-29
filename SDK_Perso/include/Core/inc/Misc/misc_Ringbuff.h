#ifndef __RINGBUFF_H
#define __RINGBUFF_H

#include <time.h>
#include <stdarg.h>
#include <windows.h>
#include "_config.h"

typedef unsigned short ushort;
typedef unsigned char uchar;
typedef unsigned long ULONG;
//typedef bool BOOL;

typedef struct tag_TUNIHeader
{
  uchar type;
  uchar number;
  ushort len;

} TUNIHeader;

//=================================================================
// базовый класс кольцевого буфера
//	(работа с кольцевым массивом байт)
class ED_CORE_EXTERN CBaseRingBuffer
{
public:
	CBaseRingBuffer();
	virtual ~CBaseRingBuffer();

	// оператор присваивания
	CBaseRingBuffer& operator=( const CBaseRingBuffer& from );
	
	// размер заголовка
	virtual ULONG GetHeaderSize() { return sizeof(RingHeader); }

	// Инициализация
	//	нового буфера (с выделением памяти)
	virtual BOOL InitNew( ULONG uSize );
	//	нового буфера на указанном куске памяти
	virtual BOOL Init( char* pBuffer, ULONG uSize );
	//  использование уже созданного и инициализированного буфера
	virtual void Open( char* pBuffer );

	// безпроверочное добавление байтового блока
	void PutBlock( LPCTSTR pBlockBuffer,		// указатель на блок памяти, помещаемой в кольцо
		           ULONG uBlockBufferSize,	// размер буфера 
		           ULONG uBlockSize			// размер блока
				 );
	// безпроверочное извлечение байтового блока
	void GetBlock( LPTSTR pBlockBuffer, 
				   ULONG uBlockBufferSize,
				   ULONG uBlockSize,
				   ULONG* puReadOffset,
				   ULONG* puReadLap
				 );

	// заголовок
	struct RingHeader
	{
		ULONG uSize;		   // общая длинна буфера
		
		// базовые смещения
		ULONG uDataOffset;	   // смещение на начало данных
		ULONG uDataSize;	   // размер данных
		ULONG uDataTailOffset; // смещение за конец данных

		// смещения для операций записи
		ULONG uWriteOffset;		// смещение на первый свободный байт
		ULONG uWriteLap;		// текущий круг uWriteOffset
		
		// текущая позиция стирания
		ULONG uEraseOffset;
		ULONG uEraseLap;
		
	};

protected:

	////////////////////////////////////////////////////////
	// инициализировать заголовок
	virtual void InitHeader( ULONG uSize );
	
	// проверка того, что позицию чтения переехала позиция записи
	BOOL IsReadOverwritten();
	
	// буфер полностью свободен
	BOOL IsFree();
	// определение объема свободного места
	ULONG GetFreeSize();

	// определение объема занятого места (относительно позиции чтения)
	ULONG GetBusySize();

	// попытаться найти место подновую запись путем выкидывания самых старых
	// (использует функцию DiscardNextRecord() )
	virtual BOOL TrySearchMemory( ULONG uDemandSize );

	// выкинуть очередную запись 
	// (эта функция должна быть реализована в порожденных классах !)
	virtual BOOL DiscardNextRecord();

	// корректировка позиции чтения
	void CorrectReadPosition( BOOL *pbOverwrite );

	////////////////////////////////////////////////////////
	// указатель на буффер
	char* pBuffer;
	RingHeader* pHdr;
	// признак локального выделения памяти (в CBaseRingBuffer)
	BOOL bBufferAllocated;

	// текущая позиция чтения
	ULONG	uReadOffset;
	ULONG	uReadLap;

};

//===========================================================
// кольцевой буфер записей переменной длинны
// ВНИМАНИЕ! Функции PushXXX и PopXXX не использовать
class ED_CORE_EXTERN CRingBuffer : public CBaseRingBuffer
{
public:
	CRingBuffer();
	virtual ~CRingBuffer();

	// оператор присваивания
	CRingBuffer& operator=( const CRingBuffer& from );
	
	// размер заголовка
	virtual ULONG GetHeaderSize() { return sizeof(RecordRingHeader); }

	// Инициализация
	//  использование уже созданного и инициализированного буфера
	virtual void Open( char* pBuffer );

	// Информационные функции
	//	текущее число записей
	inline int GetTotalCount()	{ return pRecHdr->iTotalCount; }
	//	счетчик добавленных записей
	inline int GetAddCount()	{ return pRecHdr->iAddCount; }
	//	счетчик удаленных записей
	inline int GetDelCount()	{ return pRecHdr->iDelCount; }

	// Операционные функции
	// ВНИМАНИЕ! До окончания операции Push (PushStop) НЕЛЬЗЯ
	//			 пользоваться функциями извлечения Pop. И наоборот!
	//	добавить запись
	//		начать добавление
	BOOL PushStart( ULONG uRecordSize );
	//		добавить часть данных
	BOOL PushData( LPCTSTR pRecordPart, ULONG uRecordPartSize );
	//		закончить добавление записи
	BOOL PushStop();

	//	извлечь запись
	//		начать извлечение
	//		возвращает длинну данных записи
	ULONG PopStart( BOOL* pbOverwrite = NULL	// признак "разрыва" потока сообщений 
												// (была пропущена чать информации по причине переполнения)
				  );	
	//		извлечь кусок записи
	//		возвращает длинну считанных данных записи
	BOOL PopData( LPCTSTR pRecordPartBuffer, 
				  ULONG pRecordPartBufferSize,
				  ULONG uRecordPartSize );
	//		закончить извлечение записи
	void PopStop();

	// заголовок
	struct RecordRingHeader : public CBaseRingBuffer::RingHeader
	{
		// статистика по записям
		int iTotalCount;	// текущее число записей
		int iAddCount;		// суммарное число добавленных записей
		int iDelCount;		// суммарное число удаленных записей
	};

protected:

	/////////////////////////////////////////////////////////////
	// инициализировать заголовок
	virtual void InitHeader( ULONG uSize );
	// выкинуть следующую запись
	virtual BOOL DiscardNextRecord();

	/////////////////////////////////////////////////////////////
	// указатель на заголовок
	RecordRingHeader* pRecHdr;
	
	// оставшаяся длинна текущей записи
	ULONG	uRecordRestSize;
};


//==============================================================
// класс - кольцевой буффер пакетов

// ВНИМАНИЕ! Функции PushXXX и PopXXX не использовать
class ED_CORE_EXTERN CUNIRingBuffer : public CBaseRingBuffer
{
public:
	CUNIRingBuffer();
	virtual ~CUNIRingBuffer();

	// оператор присваивания
	CUNIRingBuffer& operator=( const CUNIRingBuffer& from );
	
	// Функция добавления сообщения
	// Если второй части пакета нет -> 
	//	( pAddData = NULL ) && ( uAddLen == 0 )
	BOOL Put( const TUNIHeader* pPacket, // пакет
			  const LPCTSTR pAddData,		 // вторая чать пакета
			  ushort uAddLen			 // длинна второй части пакета
			);

	// Функция выборки сообщения
	BOOL Get( LPCTSTR pBuffer,			 // буффер под пакет
			  ULONG uBufferSize,		 // размер буфера
			  BOOL* pbBreakFlag			 // признак разрыва потока пакетов
			);

	// Получить заголовок следующего пакета
	BOOL ReadHeader( TUNIHeader* pHeader,
					 BOOL* pbBreakFlag
				   );
protected:
	//////////////////////////////////////////////
	// выкинуть следующую запись
	virtual BOOL DiscardNextRecord();
};

//==============================================================
// класс - кольцевой буффер сообщений на базе MMF'a
#define DEFAULT_RINGSIZE 20000
class ED_CORE_EXTERN CRingMessageBuffer : public CBaseRingBuffer
{
public:
	CRingMessageBuffer();
	virtual ~CRingMessageBuffer();

	// оператор присваивания
	CRingMessageBuffer& operator=( const CRingMessageBuffer& from );
	
	static CRingMessageBuffer* OpenNew( const LPCTSTR name, ULONG uSize=0, LPSECURITY_ATTRIBUTES pSA=NULL);

	// Открытие/создание буфера в MMF 
	BOOL Open( 
		const LPCTSTR name, 
		ULONG uSize = 0, 
		LPSECURITY_ATTRIBUTES pSA=NULL );
	// закрытие буфера в MMF с удалением
	BOOL Close( BOOL bDelete=TRUE );

	// Функция добавления сообщения
	BOOL AddMessage( 
		const LPCTSTR buffer, 
		ULONG buflen=(ULONG)-1);
	// Функция добавления форматированного сообщения
	BOOL __cdecl AddFormatMessage( 
		LPCTSTR buffer, 
		...);
	// Функция выборки параметров сообщения
	BOOL GetMessage( 
		ULONG &hSource, 
		ULONG &hType, 
		ULONG &hSubtype,
		time_t &time,
		LPCTSTR buffer,
		ULONG buflen );

	struct MessageHeader
	{
		ULONG buflen;
		ULONG hSource; 
		ULONG hType;
		ULONG hSubtype;
		time_t time;
	};

protected:
	//////////////////////////////////////////////
	// деск. MMF
	HANDLE	hFileMapping;
	// деск. Mutex
	HANDLE	hMutex;
	
	//////////////////////////////////////////////
	// выкинуть следующую запись
	virtual BOOL DiscardNextRecord();
};

#endif
