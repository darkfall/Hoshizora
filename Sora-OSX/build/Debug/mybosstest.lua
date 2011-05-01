currentSc = {}
bosscount = 0;

local sprbg = spritelib.createSprite("Tex_FrameBackground.png");
spritelib.setScale(sprbg, 3, 3);
spritelib.setCenter(sprbg, 320, 240);
spritelib.addEffect(sprbg, spritelib.makeEffectRotation(math.rad(0), math.rad(360), 10, IMAGE_EFFECT_REPEAT));
spritelib.setRotation(sprbg, math.rad(-90));
spritelib.setAlpha(sprbg, 0.7);

local sprbgu = spritelib.createSprite("Tex_FrameBackground_r.png");
spritelib.setScale(sprbgu, 3, 3);
spritelib.setCenter(sprbgu, 320, 240);
spritelib.addEffect(sprbgu, spritelib.makeEffectTransition(400, 0, 400, 300, 10, IMAGE_EFFECT_PINGPONG));
spritelib.setRotation(sprbgu, math.rad(-90));

local healthBarSpr = spritelib.createSpriteWH(400, 10);
spritelib.setColor(healthBarSpr, 1.0, 1.0, 1.0, 0.7);

function newspellcard(health, initFunc, updateFunc) 
	initFunc(health);
	return {
		scInit = initFunc,
		scUpdate = updateFunc
	}
end

function drawbg() 
	spritelib.update(sprbg);
	spritelib.update(sprbgu);
	spritelib.render(sprbgu);
	spritelib.renderWithPos(sprbg, 400, 300);
end

function update(dt)
	currentSc.scUpdate();
	
	drawbg();
	
	spritelib.update(sprbg);
	spritelib.update(sprbgu);
	spritelib.render(sprbgu);
	spritelib.renderWithPos(sprbg, 400, 300);
	
	spritelib.renderWithPos(healthBarSpr, 200, 10);
	
	if bosscount == 120 then
		boss:setDied(true);
	end
	bosscount = bosscount + 1;
end

function init()
	boss:setPosition(400, 100);
	boss:setHealth(500);
	boss:setLifeNumber(3);
	boss:playAnimation("stand", true, false);
	
	boss:loadScript("patchouli_sc1.lua");

	
	currentSc = newspellcard(500, royalFire.init, royalFire.update);
end

function finish()
	spritelib.release(sprbg);
	spritelib.release(sprbgu);
	spritelib.release(healthBarSpr);
end