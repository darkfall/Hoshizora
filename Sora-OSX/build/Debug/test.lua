pausebutton = guilib.getWidgetById("Pause");
guilib.setWidgetCaption(pausebutton, "Fuck!");

playbutton = guilib.getWidgetById("Open");
guilib.removeWidget(playbutton);

newbutton = guilib.createWidget("button", "Open");
guilib.setWidgetResponser(newbutton, "PlayerController", "action")
guilib.setWidgetCaption(newbutton, "Open");
guilib.setWidgetPos(newbutton, 36, 50);
guilib.addWidget(newbutton, "PlayerWindow");

spr = spritelib.createSprite("magicCircle.png");
spritelib.setScale(spr, 0.5, 0.5);
spritelib.setPosition(spr, SoraCore:getScreenWidth()-spritelib.getSpriteWidth(spr), SoraCore:getScreenHeight()-spritelib.getSpriteHeight(spr));

function update(dt)

end

function render()
	spritelib.render(spr);
end