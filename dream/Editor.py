#!/usr/bin/python
''' Naming Convention:
_memberObject
memberFunction
ClassName
temperaryvariable
property
'''
import wx,os
DEBUG = True
OBJECT_LIST = ['GameObject','EditorObject']
ID_NEW_OBJECT = wx.NewId()
ID_DUPLICATE_OBJECT = wx.NewId()
ID_DELETE_OBJECT = wx.NewId()
class ResourceRepository(object):
	def __init__(self):
		self._bitmaps = {}
	def loadImage(self,name):
		if name not in self._bitmaps:
			if not os.path.exists(name):
				return None
			try:
				self._bitmaps[name] = wx.Bitmap(name)
			except:
				return None
		return self._bitmaps[name]
	
		
_resourceFactory = ResourceRepository()		

class EditorObject(object):
	def __init__(self):
		self._data = {}
		self._components = {}
		self._additionalUI = {}
	def defaultDefs(self):
		return {'name' : ''}
	def createUI(self,container,sizer = None):
		self._components['name'] = wx.TextCtrl(container, -1, '',name = 'name')
		self._additionalUI['name'] = wx.StaticText(container,-1, 'Name')
		#vboxl,vboxr = wx.BoxSizer(wx.VERTICAL), wx.BoxSizer(wx.VERTICAL)
		newSizer = False
		if sizer is None:
			newSizer = True
			print 'newsizer'
			sizer = wx.BoxSizer(wx.VERTICAL)
		for key,v in self._components.iteritems():
			hbox = wx.BoxSizer(wx.HORIZONTAL)
			hbox.Add(self._additionalUI[key],1,wx.LEFT | wx.EXPAND,20)
			hbox.Add(self._components[key],1,wx.RIGHT | wx.EXPAND,20)
			#print container
			container.Bind(wx.EVT_TEXT,self.onChangeValue,self._components[key])
			v.SetValue(str(self._data[v.GetName()]))
			sizer.AddSizer(hbox,0,wx.EXPAND | wx.ALL,20)
		if newSizer:
			container.SetSizer(sizer)
			container.Layout()
		self.container = container
	def destroyUI(self):
		for key,v in self._components.iteritems():
			v.Destroy()
		for key,v in self._additionalUI.iteritems():
			v.Destroy()
		self._components.clear()
		self._additionalUI.clear()
		#self.container.SetSizer(None)
	def render(self,dc):
		#print 'rendered'
		pass
	def onChangeValue(self,event):
		field = event.GetEventObject()
		setattr(self,field.GetName(),event.GetString())
		#self.container.ProcessEvent(wx.PaintEvent())
	def loadDictionary(self,**defs):
		for k,v in defs.iteritems():
			#if hasattr(self,k):
			setattr(self,k,v)
	@property
	def name(self):
		return self._data['name']
	@name.setter
	def name(self,value):
		self._data['name'] = str(value)
		
class GameObject(EditorObject):
	# todo: figure this out
	def __init__(self):
		EditorObject.__init__(self)
		
	def defaultDefs(self):
		return {'image' : '','x':0,'y':0,'w':64,'h':64,'name' : ''}
	def createUI(self,container,sizer = None):
		self._components['x'] = wx.TextCtrl(container, -1, '',name = 'x')
		self._additionalUI['x'] = wx.StaticText(container,-1, 'Position X')
		self._components['y'] = wx.TextCtrl(container, -1, '',name = 'y')
		self._additionalUI['y'] = wx.StaticText(container,-1, 'Position Y')
		self._components['w'] = wx.TextCtrl(container, -1, '',name = 'w')
		self._additionalUI['w'] = wx.StaticText(container,-1, 'Width')
		self._components['h'] = wx.TextCtrl(container, -1, '',name = 'h')
		self._additionalUI['h'] = wx.StaticText(container,-1, 'Height')
		self._components['image'] = wx.TextCtrl(container, -1, '',name = 'image')
		self._additionalUI['image'] = wx.StaticText(container, -1, 'Image Path', name = 'image')
		EditorObject.createUI(self,container,sizer)
	@property
	def image(self):
		return self._data['image']
	@image.setter
	def image(self,value):
		self._data['image'] = str(value)
		self._image = _resourceFactory.loadImage(value)
	@property
	def x(self):
		return self._data['x']
	@x.setter
	def x(self,value):
		self._data['x'] = float(value)
	@property
	def y(self):
		return self._data['y']
	@y.setter
	def y(self,value):
		self._data['y'] = float(value)
	@property
	def w(self):
		return self._data['w']
	@w.setter
	def w(self,value):
		'''try:
			s = self._scene
			s.removeObject(self)
			self._data['w'] = float(value)
			s.addObject(self)'''
		#except:
		self._data['w'] = float(value)
	@property
	def h(self):
		return self._data['h']
	@h.setter
	def h(self,value):
		self._data['h'] = float(value)
	def render(self,dc,x,y):
		dc.DrawBitmap(self._image,self._data['x'] + x,self._data['y'] + y)
		
class SelectionTool(object):
	def __init__(self,Scene,Inspector):
		self._scene,self._inspector = Scene,Inspector
	def on_click(self,event):
		x,y = event.GetPositionTuple()
		objs = self._scene._objects
		for k,obj in objs.iteritems():
			if obj.x < x and obj.x+obj.w > x:
				if obj.y < y and obj.y+obj.h > y:
					self._inspector.selectObject(obj)
					return
		self._inspector.selectObject(None)
		
		
class SceneWindow(wx.Frame):
	def __init__(self,parent,objs,w,h):
		wx.Frame.__init__(self,parent, title ='Scene', size=(600,480))
		self.Bind(wx.EVT_PAINT,self.render)
		self.Bind(wx.EVT_LEFT_DOWN,self.on_click)
		self.Bind(wx.EVT_TIMER, self.on_timer_event)
		self._objects = objs
		self._x,self._y = 0,0
		self._w,self._h = w,h
		self._indexes = {}
		self._ratio = 64
		self.timer = wx.Timer(self)
		self.timer.Start(milliseconds=1000/30, oneShot=False)
		
		
 	def on_timer_event(self, event):
		self.GetEventHandler().ProcessEvent(wx.PaintEvent())
		
	def on_click(self,event):
		self._tool.on_click(event)
					
		#print self._indexes
		
	def addObject(self,obj):
		'''for xkey in range(int(obj.x/self._ratio),int(obj.x/self._ratio+obj.w/self._ratio)+1):
			for ykey in range(int(obj.y/self._ratio),int(obj.y/self._ratio+obj.h/self._ratio)+1):
				if not self._indexes.has_key(xkey):
					self._indexes[xkey] = {}
				if not self._indexes[xkey].has_key(ykey):
					self._indexes[xkey][ykey] = set()
				self._indexes[xkey][ykey].add(obj)'''
		self._objects[obj.name] = obj
		
	def removeObject(self,obj):
		'''if self._objects.has_key(obj):
			obj = self._objects[obj]
		for xkey in range(int(obj.x/self._ratio),int(obj.x/self._ratio+obj.w/self._ratio)+1):
			for ykey in range(int(obj.y/self._ratio),int(obj.y/self._ratio+obj.h/self._ratio)+1):
				#xkey,ykey = int(obj.x / self._ratio), int(obj.y / self._ratio)
				print xkey,ykey
				self._indexes[xkey][ykey].remove(obj)'''
		#del self._indexes[obj.name];
		del self._objects[obj.name]
	
		'''def acquireObjectsInArea(self,x,y,w = 0,h = 0):
		result = set()
		for obj in self._objects:
			if obj.x'''
		
	def render(self,event):
		dc = wx.WindowDC(self)
		dc.Clear()
		dc.DrawText('WELCOME TO DREAM EDITOR',20,20)
		for k,v in self._objects.iteritems():
			v.render(dc,self._x,self._y)
		if DEBUG == True:
			global obj
			#obj.render(dc,0,0)
		
class InspectorPanel(wx.Frame):
	def __init__(self,parent):
		wx.Frame.__init__(self,parent, title ='Scene', size=(320,480))
		
		self.obj = None
	def selectObject(self,obj):
		#print obj
		if self.obj != None:
			self.obj.destroyUI()
		self.obj = obj
		if obj != None:
			self.obj.createUI(self)
		
class NewObjectPanel(wx.Frame):
	def __init__(self,parent):
		wx.Frame.__init__(self,parent,title = 'New Object', size = (320,600))
		sizer = wx.BoxSizer(wx.VERTICAL)
		hbox1 = wx.BoxSizer(wx.HORIZONTAL)
		vbox1 = wx.BoxSizer(wx.VERTICAL)
		hbox2 = wx.BoxSizer(wx.HORIZONTAL)
		self.createButton = wx.Button(self,label = "Create Object")
		self.cancelButton = wx.Button(self,label = "Cancel")
		self.Bind(wx.EVT_BUTTON,self.on_create,self.createButton)
		self.Bind(wx.EVT_BUTTON,self.on_cancel,self.cancelButton)
		hbox2.Add(self.createButton,1,wx.ALL | wx.EXPAND | wx.ALIGN_LEFT,10)
		hbox2.Add(self.cancelButton,1,wx.ALL | wx.EXPAND | wx.ALIGN_RIGHT,10)
		self.currentType = 'GameObject'
		self.pick =  wx.ComboBox(self, value = self.currentType, choices = ['GameObject','EditorObject', 'anotherobject'],size = (150,-1))
		self.Bind(wx.EVT_COMBOBOX,self.on_selection,self.pick)
		self.picktext = wx.StaticText(self, -1, 'Object Type')
		#self.list.InsertColumn(0,'Title')
		hbox1.Add(self.picktext,1,wx.ALL | wx.EXPAND | wx.ALIGN_LEFT,10)
		hbox1.Add(self.pick,1,wx.ALL | wx.EXPAND | wx.ALIGN_RIGHT,5)
		sizer.AddSizer(hbox1)
		self.obj = GameObject()
		self.obj.loadDictionary(**self.obj.defaultDefs())
		#print self.obj._data
		self.obj.createUI(self,vbox1)
		sizer.AddSizer(vbox1)
		sizer.AddSizer(hbox2)
		self.SetSizer(sizer)
		self.vbox1 = vbox1
		self.Layout()
	def on_create(self,event):
		self._scene.addObject(self.obj)
		#self.Show(False)
	def on_cancel(self,event):
		self.Show(False)
	def on_selection(self,event):
		if self.currentType == event.GetString() or event.GetString() not in OBJECT_LIST:
			return
		try:
			obj = eval(event.GetString())()
			obj.loadDictionary(**obj.defaultDefs())
			self.vbox1.Clear()
			self.obj.destroyUI()
			self.obj = obj
		except Exception as e:
			return
		
		self.currentType = event.GetString()
		self.obj.createUI(self,self.vbox1)
		self.Layout()
		

class Editor(object):
	def __init__(self):
		self._objects={}
		self._sceneWindow = SceneWindow(None,self._objects,1200,480)
		self._inspectorPanel = InspectorPanel(None)
		self._newObjPanel = NewObjectPanel(None)
		if DEBUG == True:
			obj = GameObject()
			obj.loadDictionary(image = 'Cloud.png',x = 0,y = 100,w = 506,h=334, name = 'haha')
			self._sceneWindow.addObject(obj)
			obj._scene = self._sceneWindow
			#self._inspectorPanel.selectObject(obj)
		self._sceneWindow.Show()
		self._newObjPanel.Show()
		self._inspectorPanel.Show()
		self._sceneWindow._tool = SelectionTool(self._sceneWindow,self._inspectorPanel)
		self._newObjPanel._scene = self._sceneWindow
		# Menu Setup
		self.menu_bar  = wx.MenuBar()
		self.file_menu = wx.Menu()
		self.edit_menu = wx.Menu()

		# File Menu Setup
		self.file_menu.Append(wx.ID_ABOUT,   "&About MyApp")
		
		# Edit Menu Setup
		self.edit_menu.Append(ID_NEW_OBJECT,	"&New Object")
		
		# Append Menu
		self.menu_bar.Append(self.file_menu, "&File")
		self.menu_bar.Append(self.edit_menu, "&Edit")
		

		self._sceneWindow.SetMenuBar(self.menu_bar)
		self._sceneWindow.Bind(wx.EVT_MENU, self.on_about_request, id=wx.ID_ABOUT)
		self._sceneWindow.Bind(wx.EVT_MENU, self.on_new_object, id = ID_NEW_OBJECT)
		
	def on_about_request(self,event):
		print 'hi'
	
	def on_new_object(self,event):
		self._newObjPanel.Show()
		

if __name__=='__main__':
	app = wx.App(False)
	editor = Editor()
	app.MainLoop()