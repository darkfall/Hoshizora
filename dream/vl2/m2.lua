function love.load()
	require ('love.graphicsextra')
	
	light = love.graphics.newImage("light.png")
	back = love.graphics.newImage("sunset.jpg")
	lightsource = {100,100}
	walls = {}
--	quad = love.graphics.newQuad(400,300,800,600,800,600)
	putwall(400,200,600,700)

	putwall(300,100,200,200)
--	putwall(250,300,180,500)
end

function putwall(x1,y1,x2,y2)
	table.insert(walls,{x1,y1,x2,y2,(x2-lightsource[1])*100,(y2-lightsource[2])*100,(x1-lightsource[1])*100,(y1-lightsource[2])*100})
end

function love.draw()
	--love.graphics.clear()
	love.graphics.draw(back,0,0)
	love.graphics.setColor(255,255,255,255)
	love.graphics.draw(back,640,0)
	love.graphicsextra.setMaskMode('masking')
	for i,v in ipairs(walls) do love.graphics.polygon('fill',v) end
	--love.graphicsextra.setMaskMode('drawinverse')
	love.graphics.setColor(255,255,255,100)
	love.graphicsextra.setMask(1)
	love.graphics.polygon('fill',0,0,800,0,800,600,0,600)
	--love.graphicsextra.setMask(2)
	--love.graphics.setColor(255,0,255,100)
	--love.graphics.polygon('fill',0,0,800,0,800,600,0,600)
	
	love.graphics.setColor(255,255,255,255)
	love.graphicsextra.setMaskMode('normal')
	love.graphics.polygon('fill',200,100,300,100,300,200,200,200)
end
