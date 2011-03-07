function love.load()
	require ('love.graphicsextra')
	
	light = love.graphics.newImage("light.png")
	back = love.graphics.newImage("sunset.jpg")
	lightsource = {400,300}
	walls = {}
--	quad = love.graphics.newQuad(400,300,800,600,800,600)
--	putwall(500,350,400,450)
--	putwall(600,300,500,400)
--	putwall(300,300,200,50)
--	putwall(250,300,180,500)
--	genWall(400,350,500,450)
--	genWall(500,300,500+100,300+100)
	vl = love.graphics.newFramebuffer()
	box1 = {'a',400,350,100,100}
	box2 = {'b',500,300,100,100}
	genBox(box1)
	genBox(box2)
end

function genBox(box)
	walls[box[1]..'1'] = nil
	walls[box[1]..'2'] = nil
	walls[box[1]..'3'] = nil
	walls[box[1]..'4'] = nil
	genWall(box[1],box[2],box[3],box[2]+box[4],box[3]+box[5])
	ex,ey = 0,0
end

function putwall(n,x1,y1,x2,y2)
	walls[n]={x1,y1,x2,y2,(x2-lightsource[1])*1000,(y2-lightsource[2])*1000,(x1-lightsource[1])*1000,(y1-lightsource[2])*1000}
end

function love.draw()
	love.graphics.setColor(255,255,255,255)
	love.graphics.setRenderTarget(vl)
	--love.graphics.draw(light,-200,-200)
	--love.graphics.polygon('fill',0,0,800,0,800,600,0,600)
	love.graphics.draw(light,lightsource[1]-512,lightsource[2]-512)
	love.graphics.draw(light,lightsource[1]-512,lightsource[2]-512)
	love.graphicsextra.setMaskMode('masking')
	love.graphics.setColor(0,0,0,40)
	for i,v in pairs(walls) do love.graphics.polygon('fill',v) end
	love.graphicsextra.setMaskMode('normal')
	love.graphics.setRenderTarget()
	love.graphics.setColor(255,255,255,255)
	love.graphics.draw(back,0,0)
	love.graphics.draw(back,640,0)
	love.graphics.draw(vl,0,0)
	love.graphics.setColor(255,255,255,255)
	
	love.graphics.rectangle('fill',box1[2],box1[3],box1[4],box1[5])
	love.graphics.setColor(255,255,255,255)
	love.graphics.rectangle('fill',box2[2],box2[3],box2[4],box2[5])
	love.graphics.print('press 1,2 to move around blocks. press 3 to move light source.',100,100)
	--love.graphics.draw(light,0,0)
end

function genWall(n,x1,y1,x2,y2)
	putwall(n..'1',x1,y1,x2,y1)
	putwall(n..'2',x1,y1,x1,y2)
	putwall(n..'3',x2,y1,x2,y2)
	putwall(n..'4',x1,y2,x2,y2)
end

function love.keypressed(k)
	if k == '1' then love.update = box1move end
	if k == '2' then love.update = box2move end
	if k == '3' then love.update = lightmove end
end

function box1move(dt)
	x,y = love.mouse.getPosition()
	if  ex ~= x or ey~= y then
		box1[2],box1[3] = x,y
		genBox(box1)
	end
	ex,ey = x,y
end

function box2move(dt)
	x,y = love.mouse.getPosition()
	if ex ~= x or ey ~= y then
		box2[2],box2[3] = x,y
		genBox(box2)
	end
	ex,ey = x,y
end

function lightmove(dt)
	x,y = love.mouse.getPosition()
	if ex ~= x or ey ~= y then
		lightsource = {x,y}
		genBox(box2)
		genBox(box1)
	end
	ex,ey = x,y
end
