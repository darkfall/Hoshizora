
function add(a, b, coll)
    --if (a==leon and b==block) or (a==block and b==leon) then
	--	onGround=true
	--end
end

function persist(a, b, coll)
    if a == leon then
		nx,ny = coll:getNormal()
		if ny>0 then onGround = true end
		if math.abs(nx)/math.abs(ny)<1 then onWall = true end
	elseif b == leon then
		nx,ny = coll:getNormal()
		nx,ny = -nx,-ny
		if ny>0 and math.abs(nx)/math.abs(ny)<1 then onGround = true end
		if math.abs(nx)/math.abs(ny)<1 then onWall = true end
	end
end

function rem(a, b, coll)
end

function result(a, b, coll)
end

function love.load()
	love.filesystem.load('dream_lib.lua')()
	love.filesystem.load('dream_class.lua')()
	love.filesystem.load('preset.lua')()
	love.filesystem.load('testscene.lua')()
	scene = Scene:new({x1=0,x2=0,y1=2000,y2=2000})
	actor = StaticImageActor:new({imagep = 'cloud.jpg',x=0,y=0,r=0,sx=1,sy=1,ox=506/2,oy=334/2})
	actor3 = actor:clone()
	actor2 = SequenceActor:new({imagep = 'cloud.jpg',x=0,y=0,r=0,sx=1,sy=1,ox=32,oy=32,stand={1,{0,0,64,64,500,500},{64,0,64,64,500,500}}})
	f = FrameActor:new({mode = 'fill',vertex = {-67.0, -30.0,105.0,-54.0,-6.0,80.0}})
	leon = Character:new({x=100,y=0,r=0,w=64,h=64,actor = actor2,m=100})
	scene:addObject(leon)
	sceneobjs = {}
	sceneInit(sceneobjs)
	for k,v in pairs(sceneobjs) do
		scene:addObject(v)
	end
	scene.world:setCallbacks(add, persist, rem, result)
	
	-- Camera Related --
	scene.currentcamera = FollowerCamera:new(leon)
end

function love.update(dt)
	x,y = leon.body:getLinearVelocity()
	onGround = false
	scene:update(dt)
	onWall = false
	
	if love.keyboard.isDown('left') and x>-80 and onGround then leon.body:applyForce(-1000,0,leon.body:getWorldCenter())
	elseif love.keyboard.isDown('right') and x<80 and onGround then leon.body:applyForce(1000,0,leon.body:getWorldCenter()) end
	
	--else leon.body:setLinearVelocity(0,y) end
	if love.keyboard.isDown('up') and onGround and not onWall then
		leon.body:applyImpulse(0,-150,leon.body:getWorldCenter())
	end
	--camera:update(dt)
	--camera:apply()
end

function love.draw()
	scene:draw()
end