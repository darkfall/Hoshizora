##-------------  panels.py
import wx

class PanelOne(wx.Panel):
	def __init__(self,parent):
		print """panel One Constructor"""
		wx.Panel.__init__(self, parent, -1)
		self.parent = parent
		self.label = wx.StaticText(self, -1, "panel one")
		self.switch_panel = wx.Button(self, -1, "Switch to panel two",
		(50,50))
		self.Bind(wx.EVT_BUTTON, self.__OnButton, self.switch_panel)
		self.__do_layout()

	def __do_layout(self):
		self.grid_sizer = wx.GridSizer(5, 5, 4, 4)
		self.grid_sizer.Add(self.label, 0, wx.FIXED_MINSIZE, 0)
		self.grid_sizer.Add(self.switch_panel, 0, wx.FIXED_MINSIZE, 0)

		self.SetAutoLayout(True)
		self.SetSizer(self.grid_sizer)

		self.grid_sizer.Fit(self)
		self.grid_sizer.SetSizeHints(self)

	def __OnButton(self,event):
		print "OnButton"
		self.parent.loadNewPanel(self)

class PanelTwo(wx.Panel):
	def __init__(self,parent):
		print """panel Two Constructor"""
		wx.Panel.__init__(self, parent, -1)
		self.parent = parent
		self.label = wx.StaticText(self, -1, "panel two")
		self.switch_panel = wx.Button(self, -1, "Switch to panel one",
		(50,50))
		self.Bind(wx.EVT_BUTTON, self.__OnButton, self.switch_panel)
		self.__do_layout()

	def __do_layout(self):
		self.grid_sizer = wx.GridSizer(5, 5, 4, 4)
		self.grid_sizer.Add(self.label, 0, wx.FIXED_MINSIZE, 0)
		self.grid_sizer.Add(self.switch_panel, 0, wx.FIXED_MINSIZE, 0)

		self.SetAutoLayout(True)
		self.SetSizer(self.grid_sizer)

		self.grid_sizer.Fit(self)
		self.grid_sizer.SetSizeHints(self)

	def __OnButton(self,event):
		print "OnButton"
		self.parent.loadNewPanel(self)
		
active_frame = 1
class MyFrame(wx.Frame):
	def __init__(self, *args,**kwds):
		kwds['style'] = wx.DEFAULT_FRAME_STYLE
		wx.Frame.__init__(self,*args,**kwds)
		self.panel = PanelOne(self)
		self.__set_properties()
		self.__do_layout()
	def __set_properties(self):
		self.SetTitle("frame_1")

	def __do_layout(self):
		self.sizer_1 = wx.BoxSizer(wx.VERTICAL)
		self.sizer_1.Add(self.panel, 1, wx.EXPAND, 0)
		self.SetAutoLayout(True)
		self.SetSizer(self.sizer_1)
		self.sizer_1.Fit(self)
		self.sizer_1.SetSizeHints(self)
		self.Layout()

	def loadNewPanel(self,invoker):
		if isinstance(invoker,PanelOne):
			print "loading panel two"
			self.panel = PanelTwo(self)
		else:
			print "loading panel one"
			self.panel = PanelOne(self)
		self.sizer_1.Fit(self)
		self.sizer_1.SetSizeHints(self)

class MyApp(wx.App):
	def OnInit(self):
		wx.InitAllImageHandlers()
		frame = MyFrame(None, -1, "This is a wx.Frame",pos=(0,0),size=(640,480),style = wx.DEFAULT_FRAME_STYLE)
		self.SetTopWindow(frame)
		frame.Show()
		return 1

if __name__ == "__main__":
	app = MyApp(0)
	app.MainLoop()