if count % 120 == 0 then
		local color = math.random(1,16);
		for i=1,6,1 do
			bullet = shootBullet(400, 300, 1, math.rad(i*60), color, 0);
			functionCallList[i] = functionCall.new(bullet, 1, 30, 1, 
													function(bullet)
														if isBulletValid(bullet)  then
															local startDirection = getBulletDirection(bullet) - math.rad(32);
															for j=0, 8, 1 do
																shootBullet(getBulletPosX(bullet), 
																							getBulletPosY(bullet), 
																							1, 
																							startDirection+math.rad(j*8), 
																							getBulletId(bullet)+1, 
																							0);
															end
															removeBullet(bullet, true);
														end
													end);
		end
	else
		for i=1, 6, 1 do
			functionCall.update(functionCallList[i]);
		end
	end
	count = count + 1;
	
	
	functionCall = {}
function functionCall.new(bullet, times, delay, interval, func) return {
	callTimes=times, callDelay=delay, callInterval=interval,
	currCallTimes=0, currCallDelay=0, currCallInterval=interval,
	funcHandle=func, bulletHandle=bullet }
end

function functionCall.update(f)
	if f.callTimes == -2 then
		return -1;
	end
		
	if f.currCallDelay < f.callDelay then
		f.currCallDelay = f.currCallDelay+1;
	else
		if f.currCallInterval < f.callInterval then
			f.currCallInterval = f.currCallInterval + 1;
		else
			f.funcHandle(f.bulletHandle);
			f.currCallInterval = 0;
			
			f.currCallTimes = f.currCallTimes + 1;
			if f.currCallTimes >= f.callTimes then
				f.callTimes = -2;
				return -1;
			end;
			return 1;
		end
	end
	return 0;
end