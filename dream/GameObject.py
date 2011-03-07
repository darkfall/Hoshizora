#!/usr/bin/python
import wx
import pickle

		
class EditorObject(object):
	def __init__(self):
		self._attributes = {}
		self._additionalUI = {}
		self._name = ''
		self.UIActive = False
	
	def createUI(self,container):# Must implement this
		if self.UIActive:
			return
		box = wx.BoxSizer(wx.VERTICAL)
		
		namefield = wx.TextCtrl(container, value = str(self.editorName))
		self._additionalUI[wx.StaticText(container, -1, 'Object Name' , wx.Point(10, 20),style=wx.ALIGN_CENTER_VERTICAL)] = 'objName'
		self._attributes[namefield] = 'editorName'
		
		for field in self._attributes:
			container.Bind(wx.EVT_TEXT, self.onChange, field)
		box.Add(namefield,1, wx.RIGHT, 20)
		container.SetSizer(box)
		self.container = container
		self.UIActive = True
		
	def refresh(self):
		for field in self._attributes:
			field.SetValue(str(getattr(self,self._attributes[field])))
			
	def onChange(self,event):
		field = event.GetEventObject()
		try:
			if field in self._attributes:
				setattr(self,self._attributes[field],event.GetString())
				
		except ValueError:
			field.SetValue(str(getattr(self,self._attributes[field])))
	
	def destroyUI(self):# Must implement this
		if not self.UIActive:
			return
		del self.container
		for field in self._attributes:
			field.Destroy()
			del field
		for field in self._additionalUI:
			field.Destroy()
			del field
		self._attributes={}
		self.UIActive = False
	
	def get_editorName(self):
		'''Editor Name'''
		return self._name
	
	def set_editorName(self,value):
		#print 'set'
		self._name = str(value)
		
	editorName = property(get_editorName,set_editorName)


class GameObject(EditorObject):
	def __init__(self):
		self._x,self._y,self._w,self._h = .0,.0,.0,.0
		super(GameObject,self).__init__()
		
	def createUI(self,container):# Must implement this
		if self.UIActive:
			return
		
		self._attributes[wx.TextCtrl(container, value = str(self.objectX))] = 'objectX'
		self._attributes[wx.TextCtrl(container, value = str(self.objectY),size = (140,-1),style=wx.ALIGN_CENTER_VERTICAL)] = 'objectY'
		self._attributes[wx.TextCtrl(container, value = str(self.objectW),size = (140,-1),style=wx.ALIGN_CENTER_VERTICAL)] = 'objectW'
		self._attributes[wx.TextCtrl(container, value = str(self.objectH),size = (140,-1),style=wx.ALIGN_CENTER_VERTICAL)] = 'objectH'
		self._additionalUI[wx.StaticText(container, -1, 'ObjectX' , wx.Point(10, 50))] = 'ObjectX'
		self._additionalUI[wx.StaticText(container, -1, 'ObjectY' , wx.Point(10, 80))] = 'ObjectY'
		self._additionalUI[wx.StaticText(container, -1, 'ObjectW' , wx.Point(10, 110))] = 'ObjectW'
		self._additionalUI[wx.StaticText(container, -1, 'ObjectH' , wx.Point(10, 140))] = 'ObjectH'
		super(GameObject,self).createUI(container)
	@property	
	def objectX(self):
		'''Object X cood'''
		return self._x
	@objectX.setter
	def objectX(self,value):
		self._x = float(value)
	@property
	def objectY(self):
		'''Object Y cood'''
		return self._y
	@objectY.setter
	def objectY(self,value):
		self._y = float(value)
		
	@property
	def objectW(self):
		return self._w
	@objectW.setter
	def objectW(self,value):
		self._w = float(value)
	
	@property
	def objectH(self):
		return self._h
	@objectH.setter
	def objectH(self,value):
		self._h = float(value)
		
	def getDefinition(self):
		definitions = {}
		for field in self._attributes:
			key = self._attributes[field]
			definitions[key] = getattr(self,key)
		return definitions
		
	def setDefinitions(self,**definitions):
		for key in definitions:
			value = definitions[key]
			setattr(self,key,value)



if __name__ == '__main__':
	app = wx.App(False)
	frame = wx.Frame(None)
	panel = ExamplePanel(frame)
	obj = GameObject()
	obj.createUI(panel)
	frame.Show()
	app.MainLoop()