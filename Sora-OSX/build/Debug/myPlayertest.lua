function onInit()
	player:setPosition(300, 500);
	player:setScreenRect(175, 0, 625, 600);
	player:setCollisionBox(32, 48);
end

function onDestroy()

end

local shootDur = 0;

function onShoot()
	if shootDur % 5 == 0 then 
		bullet:shootBullet(player:getPositionX()-5, player:getPositionY(), 5, math.rad(90), 3, 0);
		bullet:shootBullet(player:getPositionX()+5, player:getPositionY(), 5, math.rad(90), 3, 0);
	end
	shootDur = shootDur + 1;
end

function onBomb()

end

function onUpdate()

end

function onCustomKey(key, flag)

end

function onDie()

end

function onCollision()

end

function onItem(item)

end

