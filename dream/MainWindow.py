from taste import *
import pickle
import os,wx
from wx.lib.masked import NumCtrl
class EditorObject(object):
	def getDefinition():
		return
	def applyDefinition():
		return

keynum = 0
def getUnusedkey(n = 0):
	global keynum
	keynum = keynum+1
	return 'obj'+str(keynum)

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
		self.atts['ml']=wx.StaticText(self.frame, -1, 'Mass')
		self.atts['m']=NumCtrl(self.frame, -1, integerWidth = 5,fractionWidth = 2)
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
		self.grid.Add(self.atts['ml'])
		self.grid.Add(self.atts['m'])
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
		self.atts['m'].SetValue(self.obj.m)
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
		self.obj.m = self.atts['m'].GetValue()
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
		self.atts['ml']=wx.StaticText(self.frame, -1, 'Mass')
		self.atts['m']=NumCtrl(self.frame, -1, integerWidth = 5,fractionWidth = 2)
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
		self.grid.Add(self.atts['ml'])
		self.grid.Add(self.atts['m'])
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
		self.atts['m'].SetValue(self.obj.m)
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
		self.obj.m = self.atts['m'].GetValue()
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


class BallGameObjectSelection(object):
	def createUI(self,frame,sizer):
		self.sizer = sizer
		self.frame = frame
		self.grid = wx.GridSizer(20,4,5,5)
		self.title = wx.StaticText(self.frame, -1, 'BALL')
		self.save = wx.Button(self.frame, -1, 'Save Changes')
		self.frame.Bind(wx.EVT_BUTTON,self.saveData,self.save)
		self.namefield = wx.TextCtrl(self.frame, -1 ,'')
		self.sizer.Add(self.namefield)
		self.delete = wx.Button(self.frame, -1, 'Delete All')
		self.sizer.Add(self.delete)
		frame.Bind(wx.EVT_BUTTON, self.deleteall, self.delete)
		self.atts = {}
		self.atts['actorl']=(wx.StaticText(self.frame, -1, 'Actor'))
		self.atts['actor']=(wx.TextCtrl(self.frame, -1))
		self.atts['xl']=(wx.StaticText(self.frame, -1, 'X'))
		self.atts['x']=(NumCtrl(self.frame, -1,integerWidth = 5,fractionWidth = 2))
		self.atts['yl']=(wx.StaticText(self.frame, -1, 'Y'))
		self.atts['y']=(NumCtrl(self.frame, -1,integerWidth = 5,fractionWidth = 2))
		self.atts['rotl']=(wx.StaticText(self.frame, -1, 'Rotation'))
		self.atts['rot']=(NumCtrl(self.frame, -1,integerWidth = 5,fractionWidth = 2))
		self.atts['radl']=(wx.StaticText(self.frame, -1, 'Radius'))
		self.atts['rad']=(NumCtrl(self.frame, -1,integerWidth = 5,fractionWidth = 2))
		self.atts['ml']=wx.StaticText(self.frame, -1, 'Mass')
		self.atts['m']=NumCtrl(self.frame, -1, integerWidth = 5,fractionWidth = 2)
		self.grid.Add(self.atts['xl'])
		self.grid.Add(self.atts['x'])
		self.grid.Add(self.atts['yl'])
		self.grid.Add(self.atts['y'])
		self.grid.Add(self.atts['rotl'])
		self.grid.Add(self.atts['rot'])
		self.grid.Add(self.atts['radl'])
		self.grid.Add(self.atts['rad'])
		self.grid.Add(self.atts['actorl'])
		self.grid.Add(self.atts['actor'])
		self.grid.Add(self.atts['ml'])
		self.grid.Add(self.atts['m'])
		self.sizer.Add(self.title)
		self.sizer.Add(self.grid)
		self.sizer.Add(self.save)
		self.loadData()
	def destroyUI(self):
		for k,u in self.atts:
			u.Destroy()
		self.save.Destroy()
		self.title.Destroy()
		self.grid.Clear()
		self.sizer.Clear()
	def __init__(self,obj):
		self.obj = obj
	def loadData(self):
		self.atts['x'].SetValue(self.obj.x)
		self.atts['y'].SetValue(self.obj.y)
		self.atts['rad'].SetValue(self.obj.rad)
		self.atts['rot'].SetValue(self.obj.rot)
		self.atts['m'].SetValue(self.obj.m)
		self.atts['actor'].SetValue(self.obj.actor)
		self.namefield.SetValue(self.obj.name)
	def saveData(self,e = None):
		self.obj.x = self.atts['x'].GetValue()
		self.obj.y = self.atts['y'].GetValue()
		self.obj.sx = self.atts['rad'].GetValue()
		self.obj.rot = self.atts['rot'].GetValue()
		self.obj.actor = self.atts['actor'].GetValue()
		self.obj.m = self.atts['m'].GetValue()
		self.obj.name = self.namefield.GetValue()
		if hasattr(self,'f'):
			self.f()
	def deleteall(self,e):
		getScene().removeObject(self.obj)

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



Colour = wx.Colour(255,255,255,255)
SelectionColour = wx.Colour(125,125,255,255)

defers = {
'topleft':[2,3],
'topright':[0,3],
'botright':[0,1],
'botleft':[2,1]
}
dindex = {
'topleft':[1,1,0,0],
'topright':[0,1,1,0],
'botright':[0,0,1,1],
'botleft':[1,0,0,1]
}
def getScene():
	return _scene


def getInspector():
	return _inspector



def dotTranscood(v,cosr,sinr,sx,sy,dx,dy):
	x,y = v
	nx = (x*cosr - y*sinr)*sx+dx
	ny = (x*sinr + y*cosr)*sy+dy
	return nx,ny
	

class MouseTool(object):
	def mouseDown(self,key,x,y):
		pass
	def mouseUp(self,key,x,y):
		pass
	def mouseMove(self,x,y):
		pass
	def draw(self,dc,shift):
		pass

class TransformationButton(object):
	pass
	
class DotButton(TransformationButton):
	def draw(self,dc,x,y,shift):
		dc.SetBrush(TransformButtonBrush)
		dc.SetPen(TransformButtonPen)
		dc.DrawCircle(x+shift[0],y+shift[1],5)
dotButton = DotButton()

class SelectionTool(MouseTool):
	def __init__(self):
		self.lMouseDown = False
		self.status = 'default'
		self.selection = []
		self.dotButton = DotButton()
		self.rot = 0
		self.drot = 0
		self.buttons={}
		self.centre = (0,0)
	def DeleteSelected(self):
		for o in self.selection:
			getScene().removeObject(o)

	def getTransformationButton(self,x,y):
		for i,v in self.buttons.iteritems():
			if (x-v[0])**2+(y-v[1])**2 < 16:
				return i
		return 'selection'
	def nudgeCentre(self,x,y):
		ox,oy = self.centre
		self.centre = (x+ox,y+oy)
	def mouseDown(self,key,x,y):
		if key == 'l':
			self.oriXs,self.oriYs = x,y
			if self.lMouseDown: # in case mouse moved outside of the window
				return
			self.rectW = 0
			self.rectH = 0
			self.lMouseDown = True
			self.oriX,self.oriY = x,y #Record original coodinates.
			if len(self.selection) == 0:
				self.status = 'selection'
			else:
				self.status = self.getTransformationButton(x,y)
			if self.status == 'selection':
				l = getScene().selectObject(min(x,self.oriX),min(y,self.oriY),max(abs(self.oriX-x),1),max(abs(self.oriY-y),1))
				if len(l)>0 and l[0] in self.selection:
					self.status = 'drag'
					
			if self.status in self.buttons:
				self.oriBoundingBox = self.boundingBox[:]
	def createBoundingBox(self):
		x1,y1,x2,y2=999999,999999,-1,-1
		for v in self.selection:	
			v.selecting = True
			x1 = min(min(v.getVertex(),key=lambda v:v[0])[0],x1)
			y1 = min(min(v.getVertex(),key=lambda v:v[1])[1],y1)
			x2 = max(max(v.getVertex(),key=lambda v:v[0])[0],x2)
			y2 = max(max(v.getVertex(),key=lambda v:v[1])[1],y2)
		self.boundingBox=[x1,y1,x2,y2]
	def mouseUp(self,key,x,y):
		if key == 'r':
			# We assume right key is cancel selection
			pass
		if key == 'l':
			if not self.lMouseDown:
				return
			self.lMouseDown = False
			
			if self.status == 'selection':
				for v in self.selection:
					v.selecting = False
				self.selection = getScene().selectObject(min(x,self.oriX),min(y,self.oriY),max(abs(self.oriX-x),1),max(abs(self.oriY-y),1))
				self.centre=(0,0)
				self.rot = 0
				self.createBoundingBox()
				self.updateButtonCord()
				if len(self.selection)>1:
					getInspector().applySelection(MultipleSelection(self.selection))
					
				elif len(self.selection)==0:
					getInspector().deselect()
				else:
					getInspector().applySelection(self.selection[0].getSelection(self.updateFunc))
					
			elif self.status == 'drag':
				pass
			elif self.status == 'rotation':
				self.createBoundingBox()
				self.updateButtonCord()
	def updateFunc(self):
		self.createBoundingBox();self.updateButtonCord()
				
	def mouseMove(self,x,y):
		
		if not self.lMouseDown:
			return
		if self.status=='selection':
			self.rectX = min(self.oriX,x)
			self.rectY = min(self.oriY,y)
			self.rectW = abs(self.oriX-x)
			self.rectH = abs(self.oriY-y)
			for v in self.selection:
				v.selecting = False
			self.selection = getScene().selectObject(min(x,self.oriX),min(y,self.oriY),max(abs(self.oriX-x),1),max(abs(self.oriY-y),1))
			for v in self.selection:
				v.selecting = True
		elif isinstance(self.status, TransformationButton):
			self.status.pushchange(self.selection,x,y)
		xn,yn = x-self.oriXs,y-self.oriYs
		self.oriXs,self.oriYs = x,y
		if self.status == 'drag':
			for a in self.selection:
				a.x = a.x+xn
				a.y = a.y+yn
			self.boundingBox[0] = self.boundingBox[0]+xn
			self.boundingBox[1] = self.boundingBox[1]+yn
			self.boundingBox[2] = self.boundingBox[2]+xn
			self.boundingBox[3] = self.boundingBox[3]+yn
			self.updateButtonCord()
		elif self.status in dindex:
			oribound = self.boundingBox[:]
			x1,y1,x2,y2 = oribound
			nudge = (xn,yn,xn,yn)
			if self.status in dindex:
				self.boundingBox = map(lambda orix,newy,n:orix+newy*n, self.boundingBox,dindex[self.status],nudge)
			nx1,ny1,nx2,ny2 = self.boundingBox
			dx,dy = oribound[defers[self.status][0]],oribound[defers[self.status][1]]
			sx,sy = (nx2-nx1)/(x2-x1),(ny2-ny1)/(y2-y1)
			self.updateButtonCord()
			for v in self.selection:
				orix,oriy = v.x,v.y
				orix = orix - dx
				oriy = oriy - dy
				orix = orix*sx
				oriy = oriy*sy
				orix = orix + dx
				oriy = oriy + dy
				v.x,v.y = orix,oriy
				v.sx = v.sx*sx
				v.sy = v.sy*sy
		elif self.status == 'centre':
			self.nudgeCentre(xn,yn)
			self.updateButtonCord()
		elif self.status == 'rotation':
			x1,y1,x2,y2 = self.boundingBox
			dx,dy = (x1+x2)/2,(y1+y2)/2
			nrot = math.atan2(y-dy-self.centre[1],x-dx-self.centre[0])
			self.rot,self.drot = nrot,nrot-self.rot
			self.updateButtonCord()
			cosr,sinr=math.cos(self.drot),math.sin(self.drot)
			for v in self.selection:
				x,y = v.x-dx-self.centre[0],v.y-dy-self.centre[1]
				v.x = (x*cosr - y*sinr)+dx+self.centre[0]
				v.y = (x*sinr + y*cosr)+dy+self.centre[1]
				v.rot = v.rot+self.drot
		getInspector().update()
	
	def updateButtonCord(self):
			x1,y1,x2,y2 = self.boundingBox
			dx,dy = (x1+x2)/2,(y1+y2)/2
			sinr = math.sin(self.rot)
			cosr = math.cos(self.rot)
			orix = 50*cosr
			oriy = 50*sinr
			orix = orix + self.centre[0] + dx
			oriy = oriy + self.centre[1] + dy
			self.buttons['centre'] = (dx+self.centre[0],dy+self.centre[1]) #centre
			self.buttons['topleft'] = (x1,y1) #topleft
			self.buttons['topright'] = (x2,y1) #topright
			self.buttons['botleft'] = (x1,y2) #botleft
			self.buttons['botright'] = (x2,y2) #botright
			self.buttons['topcentre'] = ((x1+x2)/2,y1) #topcentre
			self.buttons['botcentre'] = ((x1+x2)/2,y2) #botcentre
			self.buttons['centreleft'] = (x1,(y1+y2)/2) #centreleft
			self.buttons['centreright'] = (x2,(y1+y2)/2) #centreright
			self.buttons['rotation'] = (orix,oriy)
			
			
	def draw(self,dc,shift):
		if self.lMouseDown:
			if self.status=='selection' and self.rectW*self.rectH>0:
				dc.SetBrush(wx.Brush(wx.Colour(0,0,0,0)))
				dc.DrawRectangle(self.rectX+shift[0],self.rectY+shift[1],self.rectW,self.rectH)
		if len(self.selection)>0:
			try:
				x1,y1,x2,y2 = self.boundingBox
				for i,v in self.buttons.iteritems():
					self.dotButton.draw(dc,v[0],v[1],shift)
			except: pass

class RectangleTool(MouseTool):
	def __init__(self):
		self.status = 'default'
		self.startcood = (0,0)
		self.endcood = (0,0)
	def mouseDown(self,key,x,y):
		#print key
		if key == 'l':
			if self.status == 'default':
				self.startcood = (x,y)
				self.endcood = (x,y)
				self.status = 'build'
	def mouseMove(self,x,y):
		if self.status == 'build':
			self.endcood = (x,y)
	def mouseUp(self,key,x,y):
		if self.status == 'build':
			if self.startcood != self.endcood and abs(self.endcood[0]-self.startcood[0])>1 and abs(self.endcood[1]-self.startcood[1])>1:
				getScene().addObject(rectangleGameObject(getUnusedkey(len(getScene().objects)),'None',abs(self.endcood[0]+self.startcood[0])/2,abs(self.endcood[1]+self.startcood[1])/2,abs(self.endcood[0]-self.startcood[0]),abs(self.endcood[1]-self.startcood[1])))
		#if self.status == ''
		self.status = 'default'
	def draw(self,dc,shift):
		if self.status=='build':
			dc.SetBrush(wx.Brush(wx.Colour(0,0,0,0)))
			dc.DrawRectangle(min(self.endcood[0],self.startcood[0])+shift[0],min(self.endcood[1],self.startcood[1])+shift[1],abs(self.endcood[0]-self.startcood[0]),abs(self.endcood[1]-self.startcood[1]))

class PolygonTool(MouseTool):
	def __init__(self):
		self.status = 'default' # default, build
		self.cords = [(0,0)]
	def testCord(self,x,y):
		return True
		#if len(self.cords)<3:return True
		#print map(lambda n:cross(self.cords[n],self.cords[n-1],(x,y)),range(len(self.cords)))
		#return all(map(lambda n:cross(self.cords[n],self.cords[n-1],(x,y))!=0,range(len(self.cords))))
	def mouseDown(self,key,x,y):
		if key == 'l':
			if self.status == 'default':
				self.cords = []
				self.lastcord = (x,y)
				self.status = 'build'
	def mouseMove(self,x,y):
		if self.status == 'build' or self.status == 'centre':self.lastcord = (x,y)
	def mouseUp(self,key,x,y):
		if self.status == 'build':
			
			if len(self.cords)>0 and sum(map(lambda x,y:abs(x-y),self.cords[0],(x,y)))<25 and len(self.cords) >= 3:
				self.status = 'centre'
			elif self.testCord(x,y):
				self.cords.append((x,y))
		elif self.status == 'centre':
			self.status = 'default'
			getScene().addObject(polygonGameObject(getUnusedkey(len(getScene().objects)),'None',self.lastcord[0],self.lastcord[1],map(lambda x:[x[0]-self.lastcord[0],x[1]-self.lastcord[1]],self.cords)))
	def draw(self,dc,shift):
		if self.status == 'centre':
			dotButton.draw(dc,self.lastcord[0],self.lastcord[1],shift)
			if len(self.cords)>2:
				dc.DrawLines(map(lambda x:wx.Point(*x),self.cords+[self.cords[0]]),*shift)
			
		elif self.status == 'build':
			if len(self.cords)>1:
				dc.DrawLines(map(lambda x:wx.Point(*x),self.cords),*shift)
			if len(self.cords)!=0:
				dc.DrawLines((wx.Point(*self.cords[-1]),wx.Point(*self.lastcord)),*shift)

class BallTool(MouseTool):
	def __init__(self):
		self.status = 'default'
	def mouseDown(self,key,x,y):
		if key == 'l':
			if self.status == 'default':
				self.startcord = (x,y)
				self.endcord = (x,y)
				self.r = 0
				self.status = 'build'
	def mouseMove(self,x,y):
		if self.status == 'build':
			self.endcord = (x,y)
			self.r = ((x-self.startcord[0])**2+(y-self.startcord[1])**2)**0.5
	def mouseUp(self,key,x,y):
		if self.status == 'build':
			self.status = 'default'
			getScene().addObject(ballGameObject(getUnusedkey(len(getScene().objects)),'None',self.startcord[0],self.startcord[1],self.r))
	def draw(self,dc,shift):
		if self.status == 'build':
			dc.SetBrush(wx.Brush(wx.Colour(0,0,0,0)))
			#dc.SetPen(RectPen)
			#dc.SetBrush(RectPen)
			dc.DrawCircle(self.startcord[0]+shift[0],self.startcord[1]+shift[1],self.r)

class Scene(object):
	def __init__(self):
		self.objects = []
		self.name = 'testScene'
		self.path = "game/testscene.lua"
		self.w,self.h=3000,600
		self.shift=[0,0]
	def addObject(self,obj):
		self.objects.append(obj)
	def removeObject(self,obj):
		self.objects.remove(obj)
	def selectObject(self,x,y,w=1,h=1):
		result = []
		for v in self.objects:
			if intercept(v.getVertex(),[[x,y],[x+w,y],[x+w,y+h],[x,y+h]]):
				result.append(v)
		#print result
		return result
	def draw(self,dc,shift):
		for v in self.objects:
			v.draw(dc,shift)
	def loadDefinition(self):
		try:
			fpy = file(self.name,'r')
			self.objects = pickle.load(fpy)
			fpy.close()
		except:pass
	def writeDefinition(self):
		flua = file(self.path,'w')
		fpy = file(self.name,'w')
		d = 'function sceneInit()\n'
		for v in self.objects:
			d = d+'\t'+v.writeDefinition()
		flua.write(d+'\nend\n')
		flua.close()
		pickle.dump(self.objects,fpy)
		fpy.close()

class ToolBox(wx.Frame):
	def __init__(self,parent,scene):
		wx.Frame.__init__(self,parent, title = 'ToolBox', size = (400,100))
		self.frame = scene
		self.sizer1 = wx.BoxSizer(wx.HORIZONTAL)
		self.toolButtons = {wx.Button(self, -1, 'Select') : SelectionTool(),
		wx.Button(self, -1, 'Rectangle') : RectangleTool(),
		wx.Button(self, -1, 'Polygon') : PolygonTool(),
		wx.Button(self, -1, 'Ball') : BallTool()
		}
		for k in self.toolButtons.keys():
			self.sizer1.Add(k)
			self.Bind(wx.EVT_BUTTON,self.pickTool,k)
		self.sizer2 = wx.BoxSizer(wx.HORIZONTAL)
		self.saveSceneButton = wx.Button(self, -1, 'Save Scene')
		self.testSceneButton = wx.Button(self, -1, 'Test Scene')
		self.Bind(wx.EVT_BUTTON,self.saveScene,self.saveSceneButton)
		self.Bind(wx.EVT_BUTTON,self.testScene,self.testSceneButton)
		self.sizer = wx.BoxSizer(wx.VERTICAL)
		self.sizer2.Add(self.saveSceneButton)
		self.sizer2.Add(self.testSceneButton)
		self.sizer.Add(self.sizer1)
		self.sizer.Add(self.sizer2)
		self.SetSizer(self.sizer)
		self.Layout()
		self.testSceneWindow = testWindow(None,getScene())
		self.testSceneWindow.Show(False)
		
	def testScene(self,e):
		self.testSceneWindow.Show(True)
			
	def saveScene(self,e):
		getScene().writeDefinition()
	def pickTool(self,e):
		self.frame.tool = self.toolButtons[e.GetEventObject()]

class SceneWindow(wx.Frame):
	def __init__(self,parent):
		wx.Frame.__init__(self,parent, title ='Scene',size=(1024,800))
		
		
		self.Bind(wx.EVT_PAINT,self.render)
		self.Bind(wx.EVT_TIMER, self.on_timer_event)
		self.timer = wx.Timer(self)
		self.timer.Start(milliseconds=1000/60, oneShot=False)
		self.Bind(wx.EVT_LEFT_DOWN, self.OnLeftDown)
		self.Bind(wx.EVT_RIGHT_DOWN, self.OnRightDown)
		self.Bind(wx.EVT_LEFT_UP, self.OnLeftUp)
		self.Bind(wx.EVT_RIGHT_UP, self.OnRightUp)
		self.Bind(wx.EVT_MOTION, self.OnMotion)
		self.Bind(wx.EVT_KEY_DOWN, self.OnKeyDown)
		self.Bind(wx.EVT_KEY_UP, self.OnKeyUp)
		self.tool = MouseTool()
		self.Centre()


	def on_timer_event(self, event):
		self.GetEventHandler().ProcessEvent(wx.PaintEvent())
		
	def render(self,event):
		dc = wx.WindowDC(self)
		dc.Clear()
		dc.DrawText(self.tool.__class__.__name__,20,20)
		
		getScene().draw(dc,getScene().shift)
		self.tool.draw(dc,getScene().shift)
		
	def OnLeftDown(self, event):
		x,y = event.GetPosition() # position tuple
		dx,dy = getScene().shift
		self.tool.mouseDown('l',x-dx,y-dy)

	def OnRightDown(self, event):
		x,y = event.GetPosition() # position tuple
		dx,dy = getScene().shift
		self.tool.mouseDown('r',x-dx,y-dy)
		
	def OnLeftUp(self, event):
		x,y = event.GetPosition() # position tuple
		dx,dy = getScene().shift
		self.tool.mouseUp('l',x-dx,y-dy)

	def OnRightUp(self, event):
		x,y = event.GetPosition() # position tuple
		dx,dy = getScene().shift
		self.tool.mouseUp('r',x-dx,y-dy)
		
	def OnMotion(self,event):
		x,y = event.GetPosition() # position tuple
		dx,dy = getScene().shift
		self.tool.mouseMove(x-dx,y-dy)
		
	def OnKeyUp(self,event):
		key = event.GetKeyCode()
		if key == 314:getScene().shift[0] = getScene().shift[0]+100
		elif key == 315:getScene().shift[1] = getScene().shift[1]+100
		elif key == 316:getScene().shift[0] = getScene().shift[0]-100
		elif key == 317:getScene().shift[1] = getScene().shift[1]-100
	
	def OnKeyDown(self,event):pass

		
class stupidGameObject(object):
	def getVertex(self):
		return [[1,1],[101,1],[101,101],[1,101]]

class polygonGameObject(object):
	def getVertex(self):
		self.cosr = math.cos(self.rot)
		self.sinr = math.sin(self.rot)
		return map(lambda x: dotTranscood(x,self.cosr,self.sinr,self.sx,self.sy,self.x,self.y),self.vertex)	
	def setVertex(self,i,x,y):
		self.vertex[i] = [x,y]
	def __init__(self,name,actor,x,y,relativevertex):
		self.vertex = relativevertex	#addV(relativevertex,[x,y])
		self.x,self.y = x,y
		self.name = name
		self.actor = actor
		self.selecting = False
		self.sx,self.sy = 1,1
		self.rot = 0
		self.m = 0
	def draw(self,dc,shift):
		if self.selecting:
			dc.SetBrush(SelectionColourBrush)
		else:
			dc.SetBrush(ColourBrush)
		dc.SetPen(RectPen)
		self.cosr = math.cos(self.rot)
		self.sinr = math.sin(self.rot)
		dc.DrawPolygon(self.getVertex(),*shift)
		dc.DrawText(self.name + ',' + self.actor,self.x+shift[0],self.y+shift[1])
		
	def getSelection(self,f):
		s = PolygonGameObjectSelection(self)
		s.f = f
		return s
		
	def writeDefinition(self):
		vertex = map(lambda x:dotTranscood(x,math.cos(self.rot),math.sin(self.rot),self.sx,self.sy,0,0),self.vertex)
		v='{'
		for x in vertex:
			v=v+str(x[0])+','+str(x[1])+','
		return "sceneobjs['" + self.name + "'] = PolygonBlock:new({"+reduce(lambda y,x: y+x+'='+str(getattr(self,x))+',',('','x','y','rot','actor','m'))+'vertex = '+v[:-1]+"}})\n"
		
class rectangleGameObject(polygonGameObject):
	def __init__(self,name,actor,x,y,w,h):
		self._w,self._h = w,h
		self.m = 0
		relativeVertex = [[-w/2,h/2],[w/2,h/2],[w/2,-h/2],[-w/2,-h/2]]
		polygonGameObject.__init__(self,name,actor,x,y,relativeVertex)
		
	@property
	def sx(self):
		return 1
	@sx.setter
	def sx(self,v):
		self.w = self.w*v
	@property
	def sy(self):
		return 1
	@sy.setter
	def sy(self,v):
		
		#self._sy = v
		self.h = self.h*v
	@property
	def w(self):
		return self._w
	@w.setter
	def w(self,v):
		self._w = v
		self.vertex = [[-self._w/2,self._h/2],[self._w/2,self._h/2],[self._w/2,-self._h/2],[-self._w/2,-self._h/2]]
	@property
	def h(self):
		return self._h
	@h.setter
	def h(self,v):
		self._h = v
		self.vertex = [[-self._w/2,self._h/2],[self._w/2,self._h/2],[self._w/2,-self._h/2],[-self._w/2,-self._h/2]]
		
	def getSelection(self,f):
		s = RectangleGameObjectSelection(self)
		s.f = f
		return s
		
	def writeDefinition(self):
		return "sceneobjs['"+self.name+"'] = Block:new({" + reduce(lambda y,x: y+x+'='+str(getattr(self,x))+',',('','x','y','w','h','rot','m')) + "actor=" + self.actor + "})\n"
circleVertexList = []
for i in range(16):
	cosr,sinr = math.cos(math.pi/8*i),math.sin(math.pi/8*i)
	circleVertexList.append((cosr,sinr))
class ballGameObject(object):
	def __init__(self,name,actor,x,y,rad):
		self.x,self.y,self.rad = x,y,rad
		self.m = 0
		self.name = name
		self.actor = actor
		self.selecting = False
		self.rot = 0
	def getVertex(self):
		return map(lambda n:(n[0]*self.rad+self.x,n[1]*self.rad+self.y),circleVertexList)
	def getSelection(self,f):
		s = BallGameObjectSelection(self)
		s.f = f
		return s
	def draw(self,dc,shift):
		if self.selecting:
			dc.SetBrush(SelectionColourBrush)
		else:
			dc.SetBrush(ColourBrush)
		dc.SetPen(RectPen)
		dc.DrawCircle(self.x+shift[0],self.y+shift[1],self.rad)
		dc.DrawText(self.name + ',' + self.actor,self.x+shift[0],self.y+shift[1])
	def writeDefinition(self):
		return "sceneobjs['"+self.name+"'] = Ball:new({"+reduce(lambda y,x: y+x+'='+str(getattr(self,x))+',',('','x','y','rad','rot','m')) + "actor=" + self.actor + "})\n"

class testWindow(wx.Frame):
	def __init__(self,parent,scene):
		wx.Frame.__init__(self,parent,size = (300,500))
		self.sizer = wx.BoxSizer(wx.VERTICAL)
		#self.characterLabel = wx.StaticText(self,-1,'With Character')
		self.withCharacter = wx.CheckBox(self,-1,'With Character')
		self.withCharacter.SetValue(True)
		self.startTestButton = wx.Button(self,-1,'Start Test')
		self.Bind(wx.EVT_BUTTON,self.startTest,self.startTestButton)
		self.sizer.AddMany((self.withCharacter,self.startTestButton))
		self.SetSizer(self.sizer)
		self.Layout()
	def startTest(self,e):
		# todo:
		print sys.argv[0]
		os.system('/Applications/love.app/Contents/MacOS/love ~/Documents/dream/game')


if __name__ == '__main__':
	_scene = Scene()
	
	_scene.loadDefinition()
	app = wx.App(False)
	ColourBrush = wx.Brush(Colour)
	SelectionColourBrush = wx.Brush(SelectionColour)
	TransformButtonBrush = wx.Brush(wx.Colour(255,255,125,255))
	TransformButtonPen = wx.Pen(wx.Colour(0,0,0,255))
	RectPen = wx.Pen(wx.Colour(0,0,0,255))
	_inspector = InspectorPanel(None)
	_inspector.Show()
	
	scene = SceneWindow(None)
	scene.Show()
	_toolbox = ToolBox(None,scene)
	_toolbox.Show()
	app.MainLoop()
