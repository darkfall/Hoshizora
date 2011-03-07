
-- Main Character

Character = class(Config)

function Character:__init(arg)
	Config.__init(self,arg)
	for i,v in pairs(self._configs) do self[i]=v end
end

function Character:createBody()
	self.body = love.physics.newBody(self.scene.world,self.x,self.y,self.m,self.it)
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

Block = class(Config)

function Block:__init(arg)
	Config.__init(self,arg)
	for i,v in pairs(self._configs) do self[i] = v end
	if self.actor then
		local w,h = self.actor.ox,self.actor.oy
		self.actor.sx = self.w/w/2
		self.actor.sy = self.h/h/2
		--print (h,self.h)
	end
	--print (self.rot)
end

function Block:createBody()
	self.body = love.physics.newBody(self.scene.world,self.x,self.y,self.m,self.i)
	self.body:setAngle(self.rot)
	self.shape = love.physics.newRectangleShape( self.body, 0, 0, self.w, self.h, 0 )
	self.shape:setData(self)
	
end

function Block:update(dt)
	if self.body then self.x,self.y,self.rot = self.body:getX(),self.body:getY(),self.body:getAngle() end
	--print(self.x)
	--print (self.rot)
	if self.actor.update then self.actor:update(dt) end
end

function Block:draw()
	--print(self.x,self.y)
	if self.actor then self.actor:draw(self.x,self.y,self.rot) end
end

PolygonBlock = class(Config)

function PolygonBlock:__init(arg)
	Config.__init(self,arg)
	for i,v in pairs(self._configs) do self[i] = v end
	--print (self._configs)
end

function PolygonBlock:createBody()
	self.body = love.physics.newBody(self.scene.world,self.x,self.y,self.m,self.i)
	self.body:setAngle(self.rot)
	self.shape = love.physics.newPolygonShape(self.body,unpack(self.vertex))
end

function PolygonBlock:update(dt)
	if self.body then self.x,self.y,self.rot = self.body:getX(),self.body:getY(),self.body:getAngle() end
	if self.actor.update then self.actor:update(dt) end
end

function PolygonBlock:draw()
	print (self.mode)
	if self.actor then self.actor:draw(self.x,self.y,self.rot) end
end