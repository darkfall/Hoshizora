#!/usr/bin/python

import wx,os
from wx.lib.masked import NumCtrl
class EditorObject(object):
	def getDefinition():
		return
	def applyDefinition():
		return
		
class Selection(object):
	def createUI(self,frame,sizer):
		self.sizer = sizer
		self.frame = frame
		self.text = wx.StaticText(self.frame, -1, 'Nothing has been Selected')
		self.sizer.Add(self.text)
	def destroyUI(self):
		self.sizer.Clear()
		self.text.Destroy()
		
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
		wx.Frame.__init__(self, parent, title = 'Inspector (Empty)', size = (300,500))
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
		try:
			if self._sel == sel:
				return
			self._sel.destroyUI()
			self._elementSizer.Clear()
		except:
			pass
		
		self._sel = sel
		sel.createUI(self,self._elementSizer)
		self.Layout()
		
	def deselect(self):
		self.applySelection(self._defaultSelection)
		
		
	def debugButton(self,e):
		if self._sel== self._defaultSelection:
			self.applySelection(SceneSelection(None))
		else:
			self.deselect()
		#self._defaultSelection.destroyUI()
		
		

if __name__=='__main__':
	app = wx.App(False)
	inspector = InspectorPanel(None)
	inspector.Show()
	app.MainLoop()