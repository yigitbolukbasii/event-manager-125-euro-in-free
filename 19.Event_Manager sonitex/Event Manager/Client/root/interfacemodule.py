if app.ENABLE_EVENT_MANAGER:
	import uiEvent
	
"""
class Interface(object):
"""

#Search:
	def __init__(self):
		[...]
		self.wndGuildBuilding = None
		
#Add below:
		if app.ENABLE_EVENT_MANAGER:
			self.wndEventOverview = None

#Search:
	def __MakeWindows(self):
		[...]
		wndMiniMap = uiMiniMap.MiniMap()
		
#Add below:
		if app.ENABLE_EVENT_MANAGER:
			wndMiniMap.BindInterfaceClass(self)
			
			
#Seach:
		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			self.wndDragonSoul.SetDragonSoulRefineWindow(self.wndDragonSoulRefine)
			self.wndDragonSoulRefine.SetInventoryWindows(self.wndInventory, self.wndDragonSoul)
			self.wndInventory.SetDragonSoulRefineWindow(self.wndDragonSoulRefine)
			
#Add below:
		if app.ENABLE_EVENT_MANAGER:
			self.wndEventOverview = uiEvent.EventOverview()
			
			
#Search:
	def Close(self):
		[...]
		if self.wndGameButton:
			self.wndGameButton.Destroy()
			
#Add below:
		if app.ENABLE_EVENT_MANAGER:
			if self.wndEventOverview:
				self.wndEventOverview.Hide()
				self.wndEventOverview.Destroy()
				
				
#Search:
		del self.wndItemSelect
		
#Add below:
		if app.ENABLE_EVENT_MANAGER:
			del self.wndEventOverview
			
			
		
#Add:
	if app.ENABLE_EVENT_MANAGER:
		def ToggleInGameEvent(self):
			if False == player.IsObserverMode():
				if False == self.wndEventOverview.IsShow():
					self.wndEventOverview.Open()
				else:
					self.wndEventOverview.Close()
					
		def OpenInGameEvent(self):
			if self.wndEventOverview:
				if not self.wndEventOverview.IsShow():
					self.wndEventOverview.Open()
					
		def CloseInGameEvent(self):
			if self.wndEventOverview:
				if self.wndEventOverview.IsShow():
					self.wndEventOverview.Close()