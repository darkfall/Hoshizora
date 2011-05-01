loadBulletConfig("UserBullet.txt");
loadBulletSprite(1, "bullet2.png");

local count = 0;

local sprcover = spritelib.createSprite("stgui.png");
font = fontlib.createFont("ARIALN.ttf", 20);

functionCallList = {}

bgmManager:play("01.ogg", false);
SE_CLICK08 = seManager:load("click_08.wav", 1);
SE_START = seManager:load("start.wav", 2);
SE_SHOOT = seManager:load("shoot.wav", 3);
seManager:setVolume(100);


function update(dt)
	fontlib.renderString(font, 175, 0, FONT_ALIGNMENT_LEFT, "BULLET: "..tostring(getBulletSize()));
	fontlib.renderString(font, 175, 580, FONT_ALIGNMENT_LEFT, "FPS: "..tostring(SoraCore:getFPS()));
	
	if count == 0 then
		boss = bossManager:createBoss("mybosstest.lua", "patchouli.txt.anm");
		if boss == 0 then
			SoraCore:messageBox("error loading boss", "error", MB_OK);
		end
		count = count + 1;
	end
	
	bossManager:update();	
	bossManager:render();
	
	globalBulletManagerUpdate();
	setBulletScreenRect(150, -20, 650, 620);
	spritelib.render(sprcover);
end