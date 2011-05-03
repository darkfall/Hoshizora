loadBulletConfig("UserBullet.txt");
loadBulletSprite(1, "bullet2.png");

local count = 0;

local sprcover = spritelib.createSprite("stgui.png");
font = fontlib.createFont("ARIALN.ttf", 20);

functionCallList = {}

BGMManager:play("01.ogg", false);
SE_CLICK08 = SEManager:load("click_08.wav", 1);
SE_START = SEManager:load("start.wav", 2);
SE_SHOOT = SEManager:load("shoot.wav", 3);
BGMManager:setVolume(100);

function update(dt)
	
	if count == 0 then
		boss = BossManager:createBoss("mybosstest.lua", "patchouli.txt.anm");
		if boss == 0 then
			SoraCore:messageBox("error loading boss", "error", MB_OK);
		end
		count = count + 1;
	end
	
	BossManager:update();	
	BossManager:render();
	
	globalBulletManagerUpdate();
	setBulletScreenRect(150, -20, 650, 620);
	
	fontlib.renderString(font, 175, 20, FONT_ALIGNMENT_LEFT, "BULLET: "..tostring(getBulletSize()));
	fontlib.renderString(font, 175, 580, FONT_ALIGNMENT_LEFT, "FPS: "..tostring(SoraCore:getFPS()));
	
	spritelib.render(sprcover);
end