-- Basic Classes

-- Configuration Base
-- Provide base for configures



local _configspool={}
local _configtables={}
local _imagedatapool={}




function getimagedata(name)
	if not _imagedatapool[name] then _imagedatapool[name]=love.image.newImageData(name) end
	return _imagedatapool[name]
end
-- Config Base Class
Config = class()

function Config:__init(arg)
	if type(arg)=='table' then self._configs = arg end
	if type(arg)=='string' then self:load(arg)  end
	if arg==nil then self._configs = {} end
end

function Config:store(name)
	--print (self._configs)
	_configspool[name] = table.save(self._configs)
	_configtables[name] = self._configs
end

function Config:load(name)
	self._configs = _configtables[name] or table.load(_configspool[name])
end
function Config:copy()
	local o = {}
	for k,v in pairs(self) do
		if v.copy then
			o[k] = v:copy()
		else
			o[k] = v
		end
	end
	return o
end

-- Camera

Camera = class()

function Camera:__init()
	self.x,self.y,self.sx,self.sy,self.r=0,0,1,1,0
end

function Camera:apply()
	love.graphics.translate(self.x,self.y)
	love.graphics.scale(self.sx,self.sy)
	love.graphics.rotate(self.r)
end

-- Follower Camera

FollowerCamera = class(Camera)
function FollowerCamera:update()
	self.x = self.t.x
	self.y = self.t.y
end


-- Actor
-- image: image path
-- quad: image quad
-- visible: change visiblity

StaticImageActor = class(Config)

function StaticImageActor:draw(x,y,r)
	if self.visible then
		if self.quad then love.graphics.drawq(self.image,self.quad,self.x+x,self.y+y,self.r+r,self.sx,self.sy,self.ox,self.oy)
		else love.graphics.draw(self.image,self.x+x,self.y+y,self.r+r,self.sx,self.sy,self.ox,self.oy) end
	end
end

function StaticImageActor:__init(arg)
	Config.__init(self,arg)
	for i,v in pairs(self._configs) do self[i]=v end
	self.image = love.graphics.newImage(getimagedata(self.imagep))
	if self.quad then self.quad=love.graphics.newQuad(unpack(self.quadp)) end
	self.visible = self.visible or true
end

function StaticImageActor:copy()
	local o = StaticImageActor:new()
	for k,v in pairs(self) do
		o[k] = v
	end
	o.image = love.graphics.newImage(getimagedata(self.imagep))
	if o.quad then o.quad = love.graphics.newQuad(unpack(self.quadp)) end
	return o
end
-- SequenceActor
-- seqs: [time,{table of quad info},{...}]
-- time: currenttime
-- i: sequence index

SequenceActor = class(StaticImageActor)

function SequenceActor:__init(arg)
	StaticImageActor.__init(self,arg)
	self.currentseq = self.stand
	self.quad = love.graphics.newQuad(unpack(self.currentseq[2]))
	self.time = 0
	self.i = 2
end

function SequenceActor:update(dt)
	self.time = self.time + dt
	--print (self.i)
	if self.time >= self.currentseq[1] then 
		self.time = self.time - self.currentseq[1]
		self.i = self.i+1
		if self.i > #self.currentseq then
			self.i=2 
		end
		self.quad:setViewport(unpack(self.currentseq[self.i]))
	end
end

-- Frame Actor
-- Simple polygon shape
-- style = 'fill'/'line'
FrameActor = class(Config)
function FrameActor:__init(arg)
	Config.__init(self,self.arg)
	for i,v in pairs(self._configs) do self[i]=v end
end

function FrameActor:draw(x,y,r)
	local sinr = math.sin(r)
	local cosr = math.cos(r)
	local fv = {}
	for i = 1,#self.vertex+1,2 do
		fv[i]=v[i]*cosr-v[i+1]*sinr+x
		fv[i+1]=v[i]*sinr+v[i+1]*cosr+y
	end
	love.graphics.polygon(self.mode,fv)
end

-- Scene

Scene = class(Config)

function Scene:__init(arg)
	Config.__init(self,arg)
	for i,v in pairs(self._configs) do self[i]=v end
	self.currentcamera = Camera:new()
	self.currentcamera:__init()
	self.drawables = {}
	self.world = love.physics.newWorld(self.x1,self.x2,self.y1,self.y2,0,100,false)
	self.world:setMeter(64)
end

function Scene:addObject(obj)
	table.insert(self.drawables,obj)
	obj.scene = self
	if obj.createBody then obj:createBody() end
end

function Scene:draw()
	self.currentcamera:apply()
	for i,v in ipairs(self.drawables) do
		if v.draw then v:draw() end
	end
end

function Scene:update(dt)
	self.world:update(dt)
	for i,v in ipairs(self.drawables) do
		if v.update then v:update(dt) end
	end
	if self.currentcamera.update then self.currentcamera:update(dt) end
end
