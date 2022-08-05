//Search:
	HEADER_CG_QUEST_CONFIRM			= 31,
	
//Add below:
	HEADER_CG_REQUEST_EVENT_QUEST		= 32,
	
	
//Search:
	HEADER_CG_LOGIN5_OPENID			= 116,

//Add below:
#ifdef __EVENT_MANAGER__
	HEADER_CG_REQUEST_EVENT_DATA		= 117,
#endif


//Search:
	HEADER_GC_AUTH_SUCCESS_OPENID	= 154,

//Add below:
#ifdef __EVENT_MANAGER__
	HEADER_GC_EVENT_INFO			= 155,
	HEADER_GC_EVENT_RELOAD			= 156,
#endif


//Search:	
	HEADER_GG_CHECK_AWAKENESS		= 29,

//Add below:
#ifdef __EVENT_MANAGER__
	HEADER_GG_EVENT_RELOAD			= 30,
	HEADER_GG_EVENT					= 31,
#endif


//Add:
#ifdef __EVENT_MANAGER__
typedef struct SPacketGGReloadEvent
{
	BYTE bHeader;
} TPacketGGReloadEvent;

typedef struct SPacketGGEvent
{
	BYTE			bHeader;
	TEventTable		table;
	bool			bState;
} TPacketGGEvent;

typedef struct SPacketCGRequestEventData
{
	BYTE bHeader;
	BYTE bMonth;
} TPacketCGRequestEventData;

typedef struct SPacketGCEventInfo
{
	BYTE	bHeader;
	int		iSize;
} TPacketGCEventInfo;

typedef struct SPacketGCEventReload
{
	BYTE	bHeader;
} TPacketGCEventReload;

typedef struct SPacketEventData
{
	DWORD	dwID;
	BYTE	bType;
	long	startTime;
	long	endTime;
	int		iValue0;
	int		iValue1;
	bool	bCompleted;
} TPacketEventData;
#endif

typedef struct command_request_event_quest
{
	BYTE		bHeader;
	char		szName[QUEST_NAME_MAX_NUM + 1];
} TPacketCGRequestEventQuest;