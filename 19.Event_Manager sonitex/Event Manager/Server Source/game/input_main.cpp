//Add:
#ifdef __EVENT_MANAGER__
#include "event_manager.h"
#endif


//Add:
void CInputMain::RequestEventQuest(LPCHARACTER ch, const void* c_pData)
{
	TPacketCGRequestEventQuest* p = (TPacketCGRequestEventQuest*)c_pData;

	if (ch && ch->GetDesc())
		quest::CQuestManager::instance().RequestEventQuest(p->szName, ch->GetPlayerID());
}

#ifdef __EVENT_MANAGER__
void CInputMain::RequestEventData(LPCHARACTER ch, const char* c_pData)
{
	if (ch && ch->GetDesc())
	{
		const TPacketCGRequestEventData* p = reinterpret_cast<const TPacketCGRequestEventData*>(c_pData);
		CEventManager::Instance().SendEventInfo(ch, p->bMonth);
	}
}
#endif


//Search:
		case HEADER_CG_QUEST_CONFIRM:
			QuestConfirm(ch, c_pData);
			break;

//Add below:
		case HEADER_CG_REQUEST_EVENT_QUEST:
			RequestEventQuest(ch, c_pData);
			break;
			
			
//Search:
		case HEADER_CG_DRAGON_SOUL_REFINE:
			{
				TPacketCGDragonSoulRefine* p = reinterpret_cast <TPacketCGDragonSoulRefine*>((void*)c_pData);
				switch(p->bSubType)
				{
				case DS_SUB_HEADER_CLOSE:
					ch->DragonSoul_RefineWindow_Close();
					break;
				case DS_SUB_HEADER_DO_REFINE_GRADE:
					{
						DSManager::instance().DoRefineGrade(ch, p->ItemGrid);
					}
					break;
				case DS_SUB_HEADER_DO_REFINE_STEP:
					{
						DSManager::instance().DoRefineStep(ch, p->ItemGrid);
					}
					break;
				case DS_SUB_HEADER_DO_REFINE_STRENGTH:
					{
						DSManager::instance().DoRefineStrength(ch, p->ItemGrid);
					}
					break;
				}
			}

			break;
			
//Add below:
#ifdef __EVENT_MANAGER__
		case HEADER_CG_REQUEST_EVENT_DATA:
			RequestEventData(ch, c_pData);
			break;
#endif