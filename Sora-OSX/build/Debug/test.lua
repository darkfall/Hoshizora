spr = spritelib.createSprite("magicCircle.png");
spritelib.setScale(spr, 0.5, 0.5);
spritelib.setPosition(spr, SoraCore:getScreenWidth()-spritelib.getSpriteWidth(spr), SoraCore:getScreenHeight()-spritelib.getSpriteHeight(spr));

function update(dt)

end

function render()
	spritelib.render(spr);
end