//Search:
	HEADER_CG_QUEST_CONFIRM					 = 31,
	
//Add below:
	HEADER_CG_REQUEST_EVENT_QUEST				= 32,

	
//Search:
	HEADER_CG_LOGIN5_OPENID						= 116,

//Add below:
#ifdef ENABLE_EVENT_MANAGER
	HEADER_CG_REQUEST_EVENT_DATA				= 117,
#endif


//Search:
	HEADER_GC_AUTH_SUCCESS_OPENID				= 154,
	
	
//Add below:
#ifdef ENABLE_EVENT_MANAGER
	HEADER_GC_EVENT_INFO						= 155,
	HEADER_GC_EVENT_RELOAD						= 157,
#endif


//Search:
	QUEST_INPUT_STRING_MAX_NUM = 64,
	
//Add below:
	QUEST_NAME_MAX_NUM = 64,

	
//Add:
#ifdef ENABLE_EVENT_MANAGER
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