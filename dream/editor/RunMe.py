from Inspector import *
from taste import *

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
	def display(self,dc):
		pass
		
class TransformationButton(object):
	pass
	
class DotButton(TransformationButton):
	def draw(self,dc,x,y):
		dc.SetBrush(TransformButtonBrush)
		dc.SetPen(TransformButtonPen)
		dc.DrawCircle(x,y,5)


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
		x1,y1,x2,y2=9999,9999,-1,-1
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

				self.createBoundingBox()
				self.updateButtonCord()
			elif self.status == 'drag':
				pass
			elif self.status == 'rotation':
				self.createBoundingBox()
				self.updateButtonCord()
				
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
			nrot = math.atan2(y-dy,x-dx)
			self.rot,self.drot = nrot,nrot-self.rot
			self.updateButtonCord()
			cosr,sinr=math.cos(self.drot),math.sin(self.drot)
			for v in self.selection:
				x,y = v.x-dx+self.centre[0],v.y-dy+self.centre[1]
				v.x = (x*cosr - y*sinr)+dx-self.centre[0]
				v.y = (x*sinr + y*cosr)+dy-self.centre[1]
				v.rot = v.rot+self.drot
	
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
			
			
	def draw(self,dc):
		if self.lMouseDown:
			if self.status=='selection' and self.rectW*self.rectH>0:
				dc.SetBrush(wx.Brush(wx.Colour(0,0,0,0)))
				dc.DrawRectangle(self.rectX,self.rectY,self.rectW,self.rectH)
		if len(self.selection)>0:
			x1,y1,x2,y2 = self.boundingBox
			for i,v in self.buttons.iteritems():
				self.dotButton.draw(dc,*v)
			
		
				
class Scene(object):
	def __init__(self):
		self.objects = []
	def addObject(self,obj):
		self.objects.append(obj)
	def removeObject(self,obj):
		self.objects.remove(obj)
	def selectObject(self,x,y,w=1,h=1):
		result = []
		for v in self.objects:
			
			if intercept(v.getVertex(),[[x,y],[x+w,y],[x+w,y+h],[x,y+h]]):
				result.append(v)
		return result
	def draw(self,dc):
		for v in self.objects:
			v.draw(dc)
		

class SceneWindow(wx.Frame):
	def __init__(self,parent):
		wx.Frame.__init__(self,parent, title ='Scene', size=(600,480))
		self.Bind(wx.EVT_PAINT,self.render)
		self.Bind(wx.EVT_TIMER, self.on_timer_event)
		self.timer = wx.Timer(self)
		self.timer.Start(milliseconds=1000/60, oneShot=False)
		self.Bind(wx.EVT_LEFT_DOWN, self.OnLeftDown)
		self.Bind(wx.EVT_RIGHT_DOWN, self.OnRightDown)
		self.Bind(wx.EVT_LEFT_UP, self.OnLeftUp)
		self.Bind(wx.EVT_RIGHT_UP, self.OnRightUp)
		self.Bind(wx.EVT_MOTION, self.OnMotion)
		self.tool = SelectionTool()

 	def on_timer_event(self, event):
		self.GetEventHandler().ProcessEvent(wx.PaintEvent())
		
	def render(self,event):
		dc = wx.WindowDC(self)
		dc.Clear()
		dc.DrawText('WELCOME TO DREAM EDITOR',20,20)
		
		getScene().draw(dc)
		self.tool.draw(dc)
	def OnLeftDown(self, event):
		pt = event.GetPosition() # position tuple
		self.tool.mouseDown('l',*pt)

	def OnRightDown(self, event):
		pt = event.GetPosition()
		self.tool.mouseDown('r',*pt)
		
	def OnLeftUp(self, event):
		pt = event.GetPosition() # position tuple
		self.tool.mouseUp('l',*pt)

	def OnRightUp(self, event):
		pt = event.GetPosition()
		self.tool.mouseUp('r',*pt)
		
	def OnMotion(self,event):
		pt = event.GetPosition()
		self.tool.mouseMove(*pt)
		
class stupidGameObject(object):
	def getVertex(self):
		return [[1,1],[101,1],[101,101],[1,101]]
		
class RectangleGameObject(object):
	pass

class polygonGameObject(object):
	def getVertex(self):
		self.cosr = math.cos(self.rot)
		self.sinr = math.sin(self.rot)
		
		return map(lambda x: dotTranscood(x,self.cosr,self.sinr,self.sx,self.sy,self.x,self.y),self.vertex)
		
	def __init__(self,name,x,y,relativevertex):
		self.vertex = relativevertex	#addV(relativevertex,[x,y])
		self.x,self.y = x,y
		self.name = name
		self.selecting = False
		self.sx,self.sy = 1,1
		self.rot = 0
		

		
	def draw(self,dc):
		if self.selecting:
			dc.SetBrush(SelectionColourBrush)
		else:
			dc.SetBrush(ColourBrush)
		dc.SetPen(RectPen)
		self.cosr = math.cos(self.rot)
		self.sinr = math.sin(self.rot)
		dc.DrawPolygon(map(lambda x: dotTranscood(x,self.cosr,self.sinr,self.sx,self.sy,self.x,self.y),self.vertex))
		dc.DrawText(self.name,self.x,self.y)

if __name__ == '__main__':
	_scene = Scene()
	_scene.addObject(polygonGameObject('a random object',200.0,200.0,[[-100.0,100.0],[100.0,100.0],[50.0,-100.0]]))
	_scene.addObject(polygonGameObject('a random object',400.0,400.0,[[-100.0,100.0],[100.0,100.0],[100.0,-100.0],[-100.0,-100.0]]))
	app = wx.App(False)
	ColourBrush = wx.Brush(Colour)
	SelectionColourBrush = wx.Brush(SelectionColour)
	TransformButtonBrush = wx.Brush(wx.Colour(255,255,125,255))
	TransformButtonPen = wx.Pen(wx.Colour(0,0,0,255))
	RectPen = wx.Pen(wx.Colour(0,0,0,255))
	scene = SceneWindow(None)
	scene.Show()
	app.MainLoop()
