//Search:
	Set(HEADER_CG_QUEST_CONFIRM, sizeof(TPacketCGQuestConfirm), "QuestConfirm", true);
	
//Add below:
	Set(HEADER_CG_REQUEST_EVENT_QUEST, sizeof(TPacketCGRequestEventQuest), "RequestEventQuest", true);
	
	
//Search:
	Set(HEADER_CG_DRAGON_SOUL_REFINE, sizeof(TPacketCGDragonSoulRefine), "DragonSoulRefine", false);
	Set(HEADER_CG_STATE_CHECKER, sizeof(BYTE), "ServerStateCheck", false);
	
//Add below:
#ifdef __EVENT_MANAGER__
	Set(HEADER_CG_REQUEST_EVENT_DATA, sizeof(TPacketCGRequestEventData), "EventRequest", true);
#endif


//Search:
	Set(HEADER_GG_CHECK_AWAKENESS,		sizeof(TPacketGGCheckAwakeness),	"CheckAwakeness",		false);

//Add below:
#ifdef __EVENT_MANAGER__
	Set(HEADER_GG_EVENT_RELOAD,				sizeof(TPacketGGReloadEvent),		"Event",	false);
	Set(HEADER_GG_EVENT						sizeof(TPacketGGEvent),				"Event",	false);
#endif