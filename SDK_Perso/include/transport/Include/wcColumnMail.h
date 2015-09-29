#ifndef WO_COLUMN_MAIL_H
#define WO_COLUMN_MAIL_H


#include "WorldMail.h"
#include <osg/Vec2d>

#include "CarsEnums.h"
#include "lTerraBasic.h"

struct ColumnStatus;
struct PathOptions;
enum wcCoalitionName;

namespace roads{struct Road;}
namespace roads3{struct Road;}
class wcColumn;
class wcRoute;
struct PathQueryParams;

class wcColumnMail 
{
public:
	enum ColumnOrder
	{
		ContinueMovement = 1,
		StopMovement,
		ClearPath,
		SetFormation,
		SetROE,
		SetAlarmState
	};

	typedef GenMessage<wColumnMsgID+1, Mail::CreateMsg, ObjectID, NM_ROAD::Road*, wcCoalitionName, bool, unsigned int, ed::string, ColumnStatus> MsgCreate;
	typedef GenMessage<wColumnMsgID+2, Mail::StatusMsg, ObjectID, ColumnStatus> MsgStatus;
	typedef GenMessage<wColumnMsgID+3, Mail::Reliable, ObjectID, PathQueryParams*> MsgPathUpdate;
	typedef GenMessage<wColumnMsgID+4, Mail::Reliable, ObjectID, bool> MsgAssignTargetsUpdate;

	typedef GenMessage<wColumnMsgID+6, Mail::Reliable,  ObjectID, double> MsgOrderSpeed;
	typedef GenMessage<wColumnMsgID+7, Mail::Reliable,  ObjectID, PathOptions> MsgOrderPath;
	typedef GenMessage<wColumnMsgID+8, Mail::Reliable,  ObjectID, PathOptions> MsgOrderStraightPath;
	typedef GenMessage<wColumnMsgID+9, Mail::Reliable,  ObjectID, ColumnOrder, int> MsgOrder;
	typedef GenMessage<wColumnMsgID+10, Mail::Reliable,  ObjectID, ObjectID> MsgOrderOpenFire;
	typedef GenMessage<wColumnMsgID+11, Mail::Reliable,  ObjectID, ObjectID> MsgOrderStopFire;
	typedef GenMessage<wColumnMsgID+12, Mail::Reliable,  ObjectID, bool> MsgOrderEnableFire;

	typedef GenMessage<wColumnMsgID+13, Mail::System, ObjectID, bool, wcRoute*, int> MsgAsyncSetPath;
	typedef GenMessage<wColumnMsgID+14, Mail::System, ObjectID, wcRoute*, double, ObjectID, double> MsgAsyncSetBypass;

	typedef GenMessage<wColumnMsgID+15, Mail::DestroyMsg, ObjectID> MsgDestroy;
    typedef GenMessage<wColumnMsgID+16, Mail::Reliable, ObjectID, bool> MsgHideGroup;

	wcColumnMail();
	~wcColumnMail();

	static void sendCreate(const wcColumn* host, Mail::Message& msg);
	static void sendDestroy(const wcColumn* host);
    static void sendHideGroup(const wcColumn* host, bool b);
	static void sendStatus(const wcColumn* host, const ColumnStatus& status, int dest);
	static void sendPathUpdate(const wcColumn* host, PathQueryParams* pp);
	static void sendAssignTargetsUpdate(const wcColumn* host);

	static void sendOrderFormation(const wcColumn* host, CarFormationType type);
	static void sendOrderSpeed(const wcColumn* host, double speed);
	static void sendOrderPath(const wcColumn* host, const PathOptions& );
	static void sendOrderStraightPath(const wcColumn* host, const PathOptions&);
	static void sendOrder(const wcColumn* host, ColumnOrder order, int orderParam);
	static void sendOrderOpenFire(const wcColumn* host, ObjectID targetId);
	static void sendOrderStopFire(const wcColumn* host, ObjectID targetId);
	static void sendOrderEnableFire(const wcColumn* host, bool b);
};


namespace Mail 
{
	Stream& operator >> (Stream& stream, wcColumnMail::ColumnOrder& data);
	Stream& operator >> (Stream& stream, CarFormationType& data);
	Stream& operator >> (Stream& stream, ColumnStatus& data);
	Stream& operator >> (Stream& stream, PathOptions& data);
	Stream& operator >> (Stream& stream, NM_ROAD::Road*& data);
	Stream& operator >> (Stream& stream, PathQueryParams*& data);
}

#endif /* WO_COLUMN_MAIL_H */
