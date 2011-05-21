loadBulletConfig("UserBullet.txt");

local count = 0;

local sprcover = spritelib.createSprite("stgui.png");
font = fontlib.createFont("ARIALN.ttf", 20);

functionCallList = {}

BGMManager:play("奇跡の向こうで.mp3", false);
SE_CLICK08 = SEManager:load("click_08.wav", 1);
SE_START = SEManager:load("start.wav", 2);
SE_SHOOT = SEManager:load("shoot.wav", 3);
BGMManager:setVolume(100);

BossManager:clearBosses();
PlayerManager:clearPlayers();

function update(dt)
	
	if count == 0 then
		boss = BossManager:createBoss("mybosstest.lua", "patchouli.txt.anm");
		player1 = PlayerManager:createPlayer("myPlayertest.lua", "UserBullet.txt", "patchouli.txt.anm");

		if boss == 0 then
			SoraCore:messageBox("error loading boss", "error", MB_OK);
		end
		count = count + 1;
	end
	
	BossManager:update();	
	BossManager:render();
	
	PlayerManager:update();
	PlayerManager:render();
	
	globalBulletManagerUpdate();
	setBulletScreenRect(150, -20, 650, 620);
	
	fontlib.renderString(font, 175, 20, FONT_ALIGNMENT_LEFT, "BULLET: "..tostring(getBulletSize()));

	
	spritelib.render(sprcover);
		fontlib.renderString(font, 0, 580, FONT_ALIGNMENT_LEFT, "FPS: "..tostring(SoraCore:getFPS()));
end