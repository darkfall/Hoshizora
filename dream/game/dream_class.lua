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
Config = Object:subclass('Config')

function Config:initialize(arg)
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

-- Camera

Camera = Config:subclass('Camera')

function Camera:initialize()
	self.x,self.y,self.sx,self.sy,self.r=0,0,1,1,0
end

function Camera:apply()
	--print 'hahaha'
	love.graphics.translate(self.x,self.y)
	love.graphics.scale(self.sx,self.sy)
	love.graphics.rotate(self.r)
	--print (self.x,self.y)
end

-- Follower Camera

FollowerCamera = Camera:subclass('FollowerCamera')

function FollowerCamera:initialize(t)
	super.initialize(self,arg)
	self.t = t
	--print ('c is',self.t)
end

function Camera:apply()
	--print 'hahaha'
	--print (self.t)
	love.graphics.translate(-self.t.x+screenWidth/2,-self.t.y+screenHeight/2)
	love.graphics.scale(self.sx,self.sy)
	love.graphics.rotate(self.r)
	--print (self.x,self.y)
end


-- Actor
-- image: image path
-- quad: image quad
-- visible: change visiblity

StaticImageActor = Config:subclass('StaticImageActor')

function StaticImageActor:draw(x,y,r)
	if self.visible then
		if self.quad then love.graphics.drawq(self.image,self.quad,self.x+x,self.y+y,self.r+r,self.sx,self.sy,self.ox,self.oy)
		else love.graphics.draw(self.image,self.x+x,self.y+y,self.r+r,self.sx,self.sy,self.ox,self.oy) end
	end
end

function StaticImageActor:initialize(arg)
	super.initialize(self,arg)
	for i,v in pairs(self._configs) do self[i]=v end
	self.image = love.graphics.newImage(getimagedata(self.imagep))
	if self.quad then self.quad=love.graphics.newQuad(unpack(self.quadp)) end
	self.visible = self.visible or true
end

function StaticImageActor:clone()
	local o = super.clone(self)
	o.image = love.graphics.newImage(getimagedata(self.imagep))
	if o.quad then o.quad = love.graphics.newQuad(unpack(self.quadp)) end
	return o
end
-- SequenceActor
-- seqs: [time,{table of quad info},{...}]
-- time: currenttime
-- i: sequence index

SequenceActor = StaticImageActor:subclass('SequenceActor')

function SequenceActor:initialize(arg)
	super.initialize(self,arg)
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
FrameActor = Config:subclass('FrameActor')
function FrameActor:initialize(arg)
	super.initialize(self,arg)
	for i,v in pairs(self._configs) do self[i]=v end
	if self.w then
		self.vertex={-self.w/2,self.h/2,self.w/2,self.h/2,self.w/2,-self.h/2,-self.w/2,-self.h/2}
	end
end

function FrameActor:draw(x,y,r)
	local sinr = math.sin(r)
	local cosr = math.cos(r)
	local fv = {}
	--print (x,y,r)
	--if self.w then
	--	love.graphics.rectangle(self.mode,x-self.w/2,y-self.h/2,self.w,self.h)
	--else
		for i=1,#self.vertex,2 do
			local ox,oy=self.vertex[i],self.vertex[i+1]
			local fx = ox*cosr-oy*sinr+x
			local fy = ox*sinr+oy*cosr+y
			fv[i]=fx
			fv[i+1]=fy
		end
		love.graphics.polygon(self.mode,unpack(fv))
	--end
	
end

-- Ball Actor
-- Simple shape
-- style = 'fill'/'line'
BallActor = Config:subclass('BallActor')
function BallActor:initialize(arg)
	super.initialize(self,arg)
	for i,v in pairs(self._configs) do self[i]=v end
	self.seg = self.rad/2.4
end

function BallActor:draw(x,y,r)
	love.graphics.circle(self.mode,x,y,self.rad,self.seg)
end
-- Scene

Scene = Config:subclass('Scene')

function Scene:initialize(arg)
	super.initialize(self,arg)
	for i,v in pairs(self._configs) do self[i]=v end
	self.currentcamera = Camera:new()
	--self.currentcamera:initialize()
	self.drawables = {}
	self.world = love.physics.newWorld(self.x1,self.x1,self.y1,self.y2,0,100,false)
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
	--print (dt)
	self.world:update(math.min(dt,0.0416))
	for i,v in ipairs(self.drawables) do
		if v.update then v:update(dt) end
	end
	if self.currentcamera.update then self.currentcamera:update(dt) end
end
