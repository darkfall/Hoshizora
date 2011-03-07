function love.load()
	cost,sint = math.cos(math.pi/180*30),math.sin(math.pi/180*30)
	ratio = 1/math.sqrt(3)
	depth = 0
	recolor = true
end

function drawFrac(x1,y1,x2,y2,n)
---	if recolor then love.graphics.setColor(math.random(100),math.random(100),math.random(100)) end
	n = n or 0
	local indices = {
	{x1,y1},
	{x1+(x2-x1)/3,y1+(y2-y1)/3},
	{(x2-x1)*ratio*cost - (y2-y1)*ratio*sint + x1, (x2-x1)*ratio*sint + (y2-y1)*ratio*cost +y1},
	{x2-(x2-x1)/3,y2-(y2-y1)/3},
	{x2,y2}}
	local joints = {{2,3},{3,4}}
	love.graphics.line(x1,y1,x2,y2)
	if n == 0 then
		for j,joint in ipairs(joints) do
			x1,y1=unpack(indices[joint[1]])
			x2,y2=unpack(indices[joint[2]])
			
			love.graphics.line(x1,y1,x2,y2)
		end
	else
		for i =1,4 do
			x1,y1 = unpack(indices[i])
			x2,y2 = unpack(indices[i+1])
			drawFrac(x1,y1,x2,y2,n-1)
		end
	end
	
end

function love.keypressed(k)
	if k == 'u' then depth = depth + 1 end
	if k == 'd' then depth = depth - 1 end
	recolor = true
end

function love.draw()
	
	drawFrac(0,0,800,600,depth)
	recolor = false
end
