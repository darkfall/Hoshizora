#!/usr/bin/python

import wx,os
from wx.lib.masked import NumCtrl
class EditorObject(object):
	def getDefinition():
		return
	def applyDefinition():
		return
		

class RectangleGameObjectSelection(object):
	def createUI(self,frame,sizer):
		self.sizer = sizer
		self.frame = frame
		self.grid = wx.GridSizer(30,4,5,5)
		self.title = wx.StaticText(self.frame, -1, 'RECTANGLE')
		self.save = wx.Button(self.frame, -1, 'Save Changes')
		self.namefield = wx.TextCtrl(self.frame, -1)
		self.frame.Bind(wx.EVT_BUTTON,self.saveData,self.save)
		self.delete = wx.Button(self.frame, -1, 'Delete All')
		self.sizer.Add(self.delete)
		frame.Bind(wx.EVT_BUTTON, self.deleteall, self.delete)
		self.atts = {}
		self.atts['xl']=(wx.StaticText(self.frame, -1, 'X'))
		self.atts['x']=(NumCtrl(self.frame, -1,integerWidth = 5,fractionWidth = 2))
		
		self.atts['yl']=(wx.StaticText(self.frame, -1, 'Y'))
		self.atts['y']=(NumCtrl(self.frame, -1,integerWidth = 5,fractionWidth = 2))
		self.atts['sxl']=(wx.StaticText(self.frame, -1, 'X Scale'))
		self.atts['sx']=(NumCtrl(self.frame, -1,integerWidth = 5,fractionWidth = 2))
		self.atts['syl']=(wx.StaticText(self.frame, -1, 'Y Scale'))
		self.atts['sy']=(NumCtrl(self.frame, -1,integerWidth = 5,fractionWidth = 2))
		
		self.atts['wl']=(wx.StaticText(self.frame, -1, 'Width'))
		self.atts['w']=(NumCtrl(self.frame, -1,integerWidth = 5,fractionWidth = 2))
		self.atts['hl']=(wx.StaticText(self.frame, -1, 'Height'))
		self.atts['h']=(NumCtrl(self.frame, -1,integerWidth = 5,fractionWidth = 2))
		self.atts['rotl']=(wx.StaticText(self.frame, -1, 'Rotation'))
		self.atts['rot']=(NumCtrl(self.frame, -1,integerWidth = 5,fractionWidth = 2))
		self.atts['actorl']=(wx.StaticText(self.frame, -1, 'Actor'))
		self.atts['actor']=(wx.TextCtrl(self.frame, -1))
		self.grid.Add(self.atts['rotl'])
		self.grid.Add(self.atts['rot'])
		self.grid.Add(self.atts['xl'])
		self.grid.Add(self.atts['x'])
		self.grid.Add(self.atts['yl'])
		self.grid.Add(self.atts['y'])
		self.grid.Add(self.atts['sxl'])
		self.grid.Add(self.atts['sx'])
		self.grid.Add(self.atts['syl'])
		self.grid.Add(self.atts['sy'])

		self.grid.Add(self.atts['wl'])
		self.grid.Add(self.atts['w'])
		self.grid.Add(self.atts['hl'])
		self.grid.Add(self.atts['h'])

		self.grid.Add(self.atts['actorl'])
		self.grid.Add(self.atts['actor'])

		self.sizer.Add(self.title)
		self.sizer.Add(self.namefield)
		self.sizer.Add(self.grid)
		self.sizer.Add(self.save)
		#print self.atts
		self.loadData()

	def destroyUI(self):
		for k,u in self.atts:
			u.Destroy()
		self.save.Destroy()
		self.title.Destroy()
		self.grid.Clear()
		self.sizer.Clear()
		#self.grid.Destroy()

	def __init__(self,obj):
		self.obj = obj
	def loadData(self):
		self.atts['x'].SetValue(self.obj.x)
		self.atts['y'].SetValue(self.obj.y)
		self.atts['sx'].SetValue(self.obj.sx)
		self.atts['sy'].SetValue(self.obj.sy)
		self.atts['rot'].SetValue(self.obj.rot)
		
		self.atts['w'].SetValue(self.obj.w)
		self.atts['h'].SetValue(self.obj.h)
		self.atts['actor'].SetValue(self.obj.actor)
		self.namefield.SetValue(self.obj.name)
	def saveData(self,e = None):
		self.obj.name = self.namefield.GetValue()
		self.obj.x = self.atts['x'].GetValue()
		self.obj.y = self.atts['y'].GetValue()
		self.obj.sx = self.atts['sx'].GetValue()
		self.obj.sy = self.atts['sy'].GetValue()
		self.obj.rot = self.atts['rot'].GetValue()
		
		self.obj.w = self.atts['w'].GetValue()
		self.obj.h = self.atts['h'].GetValue()
		self.obj.actor = self.atts['actor'].GetValue()
		if hasattr(self,'f'):
			self.f()
	
	def deleteall(self,e):
		getScene().removeObject(self.obj)
		
		
class PolygonGameObjectSelection(object):
	def createUI(self,frame,sizer):
		self.sizer = sizer
		self.frame = frame
		self.grid = wx.GridSizer(20,4,5,5)
		self.title = wx.StaticText(self.frame, -1, 'POLYGON')
		self.save = wx.Button(self.frame, -1, 'Save Changes')
		self.frame.Bind(wx.EVT_BUTTON,self.saveData,self.save)
		self.namefield = wx.TextCtrl(self.frame, -1 ,'')
		self.sizer.Add(self.namefield)
		self.delete = wx.Button(self.frame, -1, 'Delete All')
		self.sizer.Add(self.delete)
		frame.Bind(wx.EVT_BUTTON, self.deleteall, self.delete)
		self.UIs = []
		for i in range(len(self.obj.vertex)):
			self.UIs.append([])
			ui = self.UIs[i]

			ui.append(wx.StaticText(self.frame, -1, 'X Shift'))
			ui.append(NumCtrl(self.frame, -1,integerWidth = 5,fractionWidth = 2))
			ui.append(wx.StaticText(self.frame, -1, 'Y Shift'))
			ui.append(NumCtrl(self.frame, -1,integerWidth = 5,fractionWidth = 2))
			
		for u in self.UIs:
			for v in u:
				self.grid.Add(v)
		self.atts = {}
		self.atts['actorl']=(wx.StaticText(self.frame, -1, 'Actor'))
		self.atts['actor']=(wx.TextCtrl(self.frame, -1))
		self.atts['xl']=(wx.StaticText(self.frame, -1, 'X'))
		self.atts['x']=(NumCtrl(self.frame, -1,integerWidth = 5,fractionWidth = 2))

		self.atts['yl']=(wx.StaticText(self.frame, -1, 'Y'))
		self.atts['y']=(NumCtrl(self.frame, -1,integerWidth = 5,fractionWidth = 2))
		self.atts['sxl']=(wx.StaticText(self.frame, -1, 'X Scale'))
		self.atts['sx']=(NumCtrl(self.frame, -1,integerWidth = 5,fractionWidth = 2))
		self.atts['syl']=(wx.StaticText(self.frame, -1, 'Y Scale'))
		self.atts['sy']=(NumCtrl(self.frame, -1,integerWidth = 5,fractionWidth = 2))
		self.atts['rotl']=(wx.StaticText(self.frame, -1, 'Rotation'))
		self.atts['rot']=(NumCtrl(self.frame, -1,integerWidth = 5,fractionWidth = 2))
		self.grid.Add(self.atts['xl'])
		self.grid.Add(self.atts['x'])
		self.grid.Add(self.atts['yl'])
		self.grid.Add(self.atts['y'])
		self.grid.Add(self.atts['sxl'])
		self.grid.Add(self.atts['sx'])
		self.grid.Add(self.atts['syl'])
		self.grid.Add(self.atts['sy'])
		self.grid.Add(self.atts['rotl'])
		self.grid.Add(self.atts['rot'])
		self.grid.Add(self.atts['actorl'])
		self.grid.Add(self.atts['actor'])

		self.sizer.Add(self.title)
		self.sizer.Add(self.grid)
		self.sizer.Add(self.save)
		self.loadData()

	def destroyUI(self):
		for u in self.UIs:
			for v in u:
				v.Destroy()
		for k,u in self.atts:
			u.Destroy()
		self.save.Destroy()
		self.title.Destroy()
		self.grid.Clear()
		self.sizer.Clear()
		#self.grid.Destroy()

	def __init__(self,obj):
		self.obj = obj
	def loadData(self):
		for i in range(len(self.obj.vertex)):
			self.UIs[i][1].SetValue(self.obj.vertex[i][0])
			self.UIs[i][3].SetValue(self.obj.vertex[i][1])
		self.atts['x'].SetValue(self.obj.x)
		self.atts['y'].SetValue(self.obj.y)
		self.atts['sx'].SetValue(self.obj.sx)
		self.atts['sy'].SetValue(self.obj.sy)
		self.atts['rot'].SetValue(self.obj.rot)
		self.atts['actor'].SetValue(self.obj.actor)
		self.namefield.SetValue(self.obj.name)
	def saveData(self,e = None):

		for i in range(len(self.obj.vertex)):
			self.obj.setVertex(i,self.UIs[i][1].GetValue(),self.UIs[i][3].GetValue())
		self.obj.x = self.atts['x'].GetValue()
		self.obj.y = self.atts['y'].GetValue()
		self.obj.sx = self.atts['sx'].GetValue()
		self.obj.sy = self.atts['sy'].GetValue()
		self.obj.rot = self.atts['rot'].GetValue()
		self.obj.actor = self.atts['actor'].GetValue()
		self.obj.name = self.namefield.GetValue()
		if hasattr(self,'f'):
			self.f()

	def deleteall(self,e):
		getScene().removeObject(self.obj)

class Selection(object):
	def createUI(self,frame,sizer):
		self.sizer = sizer
		self.frame = frame
		self.text = wx.StaticText(self.frame, -1, 'Nothing has been Selected')
		self.sizer.Add(self.text)
	def destroyUI(self):
		self.sizer.Clear()
		self.text.Destroy()
	def loadData(self):
		pass
		
class MultipleSelection(object):
	def createUI(self,frame,sizer):
		self.sizer = sizer
		self.frame = frame
		self.title = wx.StaticText(self.frame, -1, 'Multiple Selection')
		self.delete = wx.Button(self.frame, -1, 'Delete All')
		self.sizer.Add(self.title)
		self.sizer.Add(self.delete)
		frame.Bind(wx.EVT_BUTTON, self.deleteall, self.delete)
		self.loadData()
		
	def destroyUI(self):
		self.title.Destroy()
		self.delete.Destroy()
		
	def deleteall(self,e):
		for o in self.selection:
			getScene().removeObject(o)
	def loadData(self):
		pass
	def saveData(self):
		pass
	def __init__(self,selection):
		self.selection = selection
		
class SceneSelection(object):
	def createUI(self,frame,sizer):
		self.sizer = sizer
		self.frame = frame
		self.grid = wx.GridSizer(6,2,5,5)
		self.title = wx.StaticText(self.frame, -1, '')

		self.inputTextOne = NumCtrl(self.frame, -1,integerWidth = 5,fractionWidth = 5)
		self.inputTextTwo = NumCtrl(self.frame, -1,integerWidth = 5,fractionWidth = 5)
		self.inputTextThree = NumCtrl(self.frame, -1,integerWidth = 5,fractionWidth = 5)
		self.inputTextFour = NumCtrl(self.frame, -1,integerWidth = 5,fractionWidth = 5)
		self.inputTextFive = NumCtrl(self.frame, -1,integerWidth = 5,fractionWidth = 5)
		self.inputTextSix = NumCtrl(self.frame, -1,integerWidth = 5,fractionWidth = 5)

		self.labelOne = wx.StaticText(self.frame, -1, 'Width')
		self.labelTwo = wx.StaticText(self.frame, -1, 'Height')
		self.labelThree = wx.StaticText(self.frame, -1, 'StartX')
		self.labelFour = wx.StaticText(self.frame, -1, 'StartY')
		self.labelFive = wx.StaticText(self.frame, -1, 'Gravity Y')
		self.labelSix = wx.StaticText(self.frame, -1, 'Gravity Y')

		self.grid.Add(self.labelOne, 0 ,wx.EXPAND)
		self.grid.Add(self.inputTextOne, 0 ,wx.ALIGN_RIGHT)
		self.grid.Add(self.labelTwo, 0 ,wx.EXPAND)
		self.grid.Add(self.inputTextTwo, 0 ,wx.ALIGN_RIGHT)
		self.grid.Add(self.labelThree, 0 ,wx.EXPAND)
		self.grid.Add(self.inputTextThree, 0 ,wx.ALIGN_RIGHT)
		self.grid.Add(self.labelFour, 0 ,wx.EXPAND)
		self.grid.Add(self.inputTextFour, 0 ,wx.ALIGN_RIGHT)
		self.grid.Add(self.labelFive, 0 ,wx.EXPAND)
		self.grid.Add(self.inputTextFive, 0 ,wx.ALIGN_RIGHT)
		self.grid.Add(self.labelSix, 0 ,wx.EXPAND)
		self.grid.Add(self.inputTextSix, 0 ,wx.ALIGN_RIGHT)


		self.sizer.Add(self.title)
		self.sizer.Add(self.grid)
		self.loadData()

	def destroyUI(self):
		self.labelOne.Destroy()
		self.labelTwo.Destroy()
		self.labelThree.Destroy()
		self.labelFour.Destroy()
		self.labelFive.Destroy()
		self.labelSix.Destroy()
		self.inputTextOne.Destroy()
		self.inputTextTwo.Destroy()
		self.inputTextThree.Destroy()
		self.inputTextFour.Destroy()
		self.inputTextFive.Destroy()
		self.inputTextSix.Destroy()
		#self.grid.Destroy()

	def __init__(self,scene):
		self.scene = scene
	def loadData(self):
		pass
	def saveData(self):
		pass
class InspectorPanel(wx.Frame):
	def __init__(self,parent):
		wx.Frame.__init__(self, parent, title = 'Inspector (Empty)', size = (500,500))
		self._sizer = wx.BoxSizer(wx.VERTICAL)
		self.SetSizer(self._sizer)
		self._defaultSelection = Selection()
		
		self._debugButton = wx.Button(self,-1,'Hit me')
		self._elementSizer = wx.BoxSizer(wx.VERTICAL)
		self._sizer.Add(self._debugButton)
		self._sizer.Add(self._elementSizer)
		self.Bind(wx.EVT_BUTTON,self.debugButton,self._debugButton)
		self.deselect()
		
	def applySelection(self,sel):
		#try:
		#	if self._sel == sel:
		#		return
		for c in self.GetChildren():
			c.Destroy()
		#self. = InspectorPan
		self._elementSizer.Clear()
		#except:	pass
		
		self._sel = sel
		sel.createUI(self,self._elementSizer)
		self.Layout()
		
	def deselect(self):
		self.applySelection(self._defaultSelection)
		
		
	def debugButton(self,e):
		if self._sel== self._defaultSelection:
			self.applySelection(MultipleSelection(None))
		else:
			self.deselect()
			
	def update(self):
		self._sel.loadData()
		

if __name__=='__main__':
	app = wx.App(False)
	inspector = InspectorPanel(None)
	inspector.Show()
	app.MainLoop()