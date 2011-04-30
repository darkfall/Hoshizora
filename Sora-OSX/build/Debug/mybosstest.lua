currentSc = {}
bosscount = 0;

function newspellcard(health, initFunc, updateFunc) 
	initFunc(health);
	return {
		scInit = initFunc,
		scUpdate = updateFunc
	}
end

function update(dt)
	currentSc.scUpdate();
	
	bosscount = bosscount + 1;
	if bosscount % 120 == 0 then
		boss:moveToPosition(math.random(200, 600), math.random(100, 300), 120);
	end
end

function init()
	boss:setPosition(400, 100);
	boss:setHealth(500);
	boss:setLifeNumber(3);
	boss:playAnimation("stand", true, false);
	
	boss:loadScript("patchouli_sc1.lua");

	
	currentSc = newspellcard(500, royalFire.init, royalFire.update);

end