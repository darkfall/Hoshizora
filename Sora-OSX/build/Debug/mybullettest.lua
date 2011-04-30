loadBulletConfig("UserBullet.txt");
loadBulletSprite(1, "bullet2.png");

local count = 0;

font = fontlib.createFont("ARIALN.ttf", 20);

functionCallList = {}

function update(dt)
	fontlib.renderString(font, 0, 0, FONT_ALIGNMENT_LEFT, "BULLET: "..tostring(getBulletSize()));

	if count == 0 then
		boss = bossManager:createBoss("mybosstest.lua", "patchouli.txt.anm");
		if boss == 0 then
			SoraCore:messageBox("error loading boss", "error", MB_OK);
		end
		count = count + 1;
	end
end