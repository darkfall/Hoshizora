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
	
	bullet = shootBullet(rfX, rfY, 4, PI_2+rfDir, 17, 10);
	-- setBulletAlphaBlend(bullet, false);
	
	rfX = boss:getPositionX() + rfDist * math.cos(PI_2-rfDir);
	rfY = boss:getPositionY() - rfDist * math.sin(PI_2-rfDir);
	
	bullet = shootBullet(rfX, rfY, 4, PI_2-rfDir, 17, 10);
	-- setBulletAlphaBlend(bullet, false);
	SEManager:play(3);

	rfDir = rfDir + math.rad(rfChange);
	
	local ldir = math.rad(math.random(0, 360));
	
	shootLaser(boss:getPositionX(), boss:getPositionY(), 3, ldir, 90, 3, 30);
	bullet = shootLaserEx(boss:getPositionX(), boss:getPositionY(), 
				10,
				ldir, 56, 10, 90);
	


	royalFireCount = royalFireCount + 1;
end 

function royalFire.init(health)
	boss:setHealth(health);
	boss:setDied(false);
	boss:playAnimation("spellcard", false, false);
end
