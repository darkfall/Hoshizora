#!/usr/bin/python
import wx
from GameObject import *



class InspectorPanel(wx.Panel):
	def __init__(self, parent):
		wx.Panel.__init__(self, parent)
		self.clear = wx.Button(self, pos = (20,250), label = 'Clear')
		self.Bind(wx.EVT_BUTTON, self.onClear,self.clear)
		self.load = wx.Button(self, pos = (140,250), label = 'Load')
		self.Bind(wx.EVT_BUTTON, self.onLoad,self.load)
		
	def onClear(self,event):
		obj.destroyUI()
		
	def onLoad(self,event):
		obj.createUI(self)


class MainEditor(wx.Panel):
	def __init__(self,parent):
		wx.Panel.__init__(self, parent, size = (800,600))
		wx.WindowDC(self)
	def onUpdate(self,event):
		pass
		
	def onDraw(self,event):
		pass
		
if __name__ == '__main__':
	app = wx.App(False) # initiate app
	'''Editor Frame setting up'''
	editorframe = wx.Frame(None,size = (800,600),title = 'EditorScene')
	editorPanel = MainEditor(editorframe)
	editorframe.Show()
	
	'''Editor Inspector setting up'''
	inspectorFrame = wx.Frame(None,size = (400,300),title = 'Inspector')
	inspectorPanel = InspectorPanel(inspectorFrame)
	inspectorFrame.Show()
	'''Random Stuff'''
	
	obj = GameObject()
	
	obj.createUI(inspectorPanel)
	
	obj.setDefinitions(ObjectX=10,objectY=20,objectW=30,objectH=40,editorName='hahahaha')
	obj.refresh()
	'''Start application'''
	app.MainLoop()