function onInit()
	player:setPosition(500, 500);
	player:setScreenRect(175, 0, 625, 600);
	player:setCollisionBox(32, 48);
	
	player:setInputKey(PLAYER_INPUT_UP, SORA_KEY_W);
	player:setInputKey(PLAYER_INPUT_DOWN, SORA_KEY_S);
	player:setInputKey(PLAYER_INPUT_LEFT, SORA_KEY_A);
	player:setInputKey(PLAYER_INPUT_RIGHT, SORA_KEY_D);
	player:setInputKey(PLAYER_INPUT_SHOOT, SORA_KEY_J);
	player:setInputKey(PLAYER_INPUT_BOMB, SORA_KEY_K);
	player:setInputKey(PLAYER_INPUT_SLOW, SORA_KEY_L);
end

function onDestroy()

end

local shootDur = 0;

function onShoot()
	if shootDur % 5 == 0 then 
		bullet:shootBullet(player:getPositionX()-5, player:getPositionY(), 5, math.rad(90), 7, 0);
		bullet:shootBullet(player:getPositionX()+5, player:getPositionY(), 5, math.rad(90), 7, 0);
	end
	shootDur = shootDur + 1;
end

function onBomb()

end

function onUpdate()

end

function onLossHealth()

end