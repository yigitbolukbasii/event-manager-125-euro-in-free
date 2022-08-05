"""
class MiniMap(ui.ScriptWindow):
"""

# Search: 
	def __Initialize(self):
		[...]
		self.serverInfo = None
		
#Add below:
		if app.ENABLE_EVENT_MANAGER:
			self.InGameEventButton = 0
			self.interface = None
			
			
#Search:
	def __LoadWindow(self):
		[...]
			self.serverInfo = self.GetChild("ServerInfo")
			
#Add below:
			if app.ENABLE_EVENT_MANAGER:
				self.InGameEventButton = self.GetChild("InGameEventButton")
				self.InGameEventButton.SetEvent(ui.__mem_func__(self.ToggleInGameEvent))
				
				
				
#Add:
	if app.ENABLE_EVENT_MANAGER:
		def BindInterfaceClass(self, interface):
			self.interface = interface
			
		def ToggleInGameEvent(self):
			self.interface.ToggleInGameEvent()
				

