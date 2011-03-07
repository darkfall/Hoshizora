
-- Main Character

Character = Config:subclass('Character')

function Character:initialize(arg)
	super.initialize(self,arg)
	for i,v in pairs(self._configs) do self[i]=v end
end

function Character:createBody()
	self.body = love.physics.newBody(self.scene.world,self.x,self.y,self.m)
	self.shape = love.physics.newRectangleShape( self.body, 0, 0, self.w, self.h, 0 )
	self.shape:setData(self)
	self.body:setFixedRotation(true)
end

function Character:draw()
	self.actor:draw(self.x,self.y,self.r)
end

function Character:update(dt)
	if self.body then self.x,self.y,self.r = self.body:getX(),self.body:getY(),self.body:getAngle() end
	--print (self.x,self.y,self.r)
	self.actor:update(dt)
end

-- Block

Block = Config:subclass('Block')

function Block:initialize(arg)
	
	super.initialize(self,arg)
	for i,v in pairs(self._configs) do self[i] = v end
	--self.vertex = {-self.w}
	--print (unpack(self.vertex))
	if not self.actor then self.actor = FrameActor:new({mode='fill',w=self.w,h = self.h})
	else
		local w,h = self.actor.ox,self.actor.oy
		self.actor.sx = self.w/w/2
		self.actor.sy = self.h/h/2
		--print (h,self.h)
	end
	--print (self.rot)
end

function Block:createBody()
	self.body = love.physics.newBody(self.scene.world,self.x,self.y,self.m)
	self.shape = love.physics.newRectangleShape(self.body, 0, 0, self.w, self.h )
	self.body:setAngle(self.rot)
	self.shape:setData(self)
	if self.m~=0 then self.body:setMassFromShapes() end
end

function Block:update(dt)
	if self.body then self.x,self.y,self.rot = self.body:getX(),self.body:getY(),self.body:getAngle() end
	--print(self.x)
	--print (self.rot)
	if self.actor and self.actor.update then self.actor:update(dt) end
end

function Block:draw()
	--print(self.x,self.y)
	if self.actor then self.actor:draw(self.x,self.y,self.rot) end
end

PolygonBlock = Config:subclass('PolygonBlock')

function PolygonBlock:initialize(arg)
	super.initialize(self,arg)
	for i,v in pairs(self._configs) do self[i] = v end
	if not self.actor then
		self:createFrameActor()
	end
end

function PolygonBlock:createBody()
	self.body = love.physics.newBody(self.scene.world,self.x,self.y,self.m)
	--
	self.shape = love.physics.newPolygonShape(self.body,unpack(self.vertex))
	if self.m~=0 then self.body:setMassFromShapes() end
	self.body:setAngle(self.rot)
end

function PolygonBlock:update(dt)
	if self.body then self.x,self.y,self.rot = self.body:getX(),self.body:getY(),self.body:getAngle() end
	--print (self.body:getAngle())
	if self.actor and self.actor.update then self.actor:update(dt) end
end

function PolygonBlock:draw()
	if self.actor then self.actor:draw(self.x,self.y,self.rot) end
end

function PolygonBlock:createFrameActor()
	self.actor = FrameActor:new({mode='fill',vertex=self.vertex})
end

Ball = Config:subclass('Ball')
function Ball:initialize(arg)
	super.initialize(self,arg)
	for i,v in pairs(self._configs) do self[i] = v end
	if not self.actor then
		self:createBallActor()
	end
end

function Ball:createBody()
	self.body = love.physics.newBody(self.scene.world,self.x,self.y,self.m)
	--print (self.body,self.x,self.y,self.rad)
	self.shape = love.physics.newCircleShape(self.body,0,0,self.rad)
	if self.m~=0 then self.body:setMassFromShapes() end
	self.body:setAngle(self.rot)
end

function Ball:update(dt)
	if self.body then self.x,self.y,self.rot = self.body:getX(),self.body:getY(),self.body:getAngle() end
	if self.actor and self.actor.update then self.actor:update(dt) end
end

function Ball:draw()
	if self.actor then self.actor:draw(self.x,self.y,self.rot) end
end

function Ball:createBallActor()
	self.actor = BallActor:new({mode = 'fill',rad = self.rad})
end