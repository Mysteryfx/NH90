#ifndef	__NOSERIALIZATIONFLAG_H_
#define	__NOSERIALIZATIONFLAG_H_

#include "ExpImpSpec.h" 

namespace ResourceKeeperFlags
{

	class USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR NoSerializationFlag
	{
	protected:
		bool no_Serialization;
	public:
		NoSerializationFlag();
		virtual ~NoSerializationFlag(){};

		const bool noSerialization() const{return no_Serialization;}
	};

	USERINTERFACE_EXPORT_IMPORT_SPECIFICATOR const bool noSerialization();
};

#endif	//	__NOSERIALIZATIONFLAG_H__
