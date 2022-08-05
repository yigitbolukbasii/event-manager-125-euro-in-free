//Add:
#include "service.h"


//Search:
	HEADER_GD_REQUEST_CHANNELSTATUS	= 140,
	
	
//Add below:
#ifdef __EVENT_MANAGER__
	HEADER_GD_UPDATE_EVENT_STATUS	= 141,
	HEADER_GD_EVENT_NOTIFICATION	= 142,
#endif


//Search:
	HEADER_DG_RESPOND_CHANNELSTATUS		= 181,

//Add below:
#ifdef __EVENT_MANAGER__
	HEADER_DG_UPDATE_EVENT_STATUS	= 182,
	HEADER_DG_EVENT_NOTIFICATION	= 183,
#endif


//Add:
#ifdef __EVENT_MANAGER__
typedef struct SEventTable
{
	DWORD	dwID;
	char	szType[64];
	long	startTime;
	long	endTime;
	int		iValue0;
	int		iValue1;
	bool	bCompleted;
} TEventTable;
#endif