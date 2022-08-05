//Add:
#ifdef __EVENT_MANAGER__
#include "event_manager.h"
#endif


//Search:
int main(int argc, char **argv)
{
	[...]
	DSManager dsManager;

#ifdef __AUCTION__
	AuctionManager auctionManager;
#endif

//Add below:
#ifdef __EVENT_MANAGER__
	CEventManager event_manager;
#endif