royalFire = {}
royalFireCount = 0;

local rfX = boss:getPositionX();
local rfY = boss:getPositionY();
local rfDist = 50;
local rfDir = -PI_2;
local rfChange = 5;

function royalFire.update() 
	local bullet;
	
	rfX = boss:getPositionX() + rfDist * math.cos(PI_2+rfDir);
	rfY = boss:getPositionY() - rfDist * math.sin(PI_2+rfDir);
	
	bullet = shootBullet(rfX, rfY, 4, PI_2+rfDir, 17, 90);
	-- setBulletAlphaBlend(bullet, false);
	
	rfX = boss:getPositionX() + rfDist * math.cos(PI_2-rfDir);
	rfY = boss:getPositionY() - rfDist * math.sin(PI_2-rfDir);
	
	bullet = shootBullet(rfX, rfY, 4, PI_2-rfDir, 17, 90);
	-- setBulletAlphaBlend(bullet, false);
	seManager:play(3);

	rfDir = rfDir + math.rad(rfChange);
	if rfDir >= -PI_2+math.rad(30) then
		rfChange = -5;
	elseif rfDir <= -PI_2-math.rad(30) then
		rfChange = 5;
	end

	royalFireCount = royalFireCount + 1;
end 

function royalFire.init(health)
	boss:setHealth(health);
	boss:setDied(false);
	boss:playAnimation("spellcard", false, false);
end