"""
class PythonScriptLoader(object):
"""

#Search:
	## AniImage
	def LoadElementAniImage(self, window, value, parentWindow):
		[...]
		
		if value.has_key("width") and value.has_key("height"):
			window.SetSize(value["width"], value["height"])
			
#Add below:
		if True == value.has_key("x_scale") and True == value.has_key("y_scale"):
			window.SetScale(float(value["x_scale"]), float(value["y_scale"]))
