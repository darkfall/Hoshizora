royalFire = {}
royalFireCount = 0;

function royalFire.update() 
	if royalFireCount % 120 == 0 then
		for i=1,36,1 do
			shootBullet(boss:getPositionX(), boss:getPositionY(),
						4, math.rad(i*10),
						1, 0);
		end
	end
	royalFireCount = royalFireCount + 1;
end 

function royalFire.init(health)
	boss:setHealth(health);
	boss:setDied(false);
	boss:playAnimation("spellcard", false, false);
end
