spr = spritelib.createSprite("magicCircle.png");
spritelib.setScale(spr, 0.5, 0.5);
spritelib.setPositionss(spr, SoraCore:getScreenWidth()-spritelib.getSpriteWidth(spr), SoraCore:getScreenHeight()-spritelib.getSpriteHeight(spr));
spritelib.setScale(spr, 1.0, 1.0);

function update(dt)

end

function render()
	spritelib.render(spr);
end