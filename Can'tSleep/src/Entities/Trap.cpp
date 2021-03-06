#include "./../../header/Entities/Trap.h"

Trap::Trap(int position) : Constructible(DefaultEnvironmentElementBuilder()
	.setPosition(position)
	.setWidth(1)
	.setTexture("Trap")
	.build()) {
	m_animation = &DefaultAnimableBuilder()
		.setFrameRate(1.0f)
		.setTotalFrame(1)
		.setTexture("Trap")
		.setHeight(49)
		.setWidth(282)
		.build();
	m_animation->moveTo(m_realPosition);
	m_animation->setOffsetY(10);
	m_hitBox.setSize(200, TILE_HEIGHT / 1.5f);
	m_hitBox.setPosition(m_realPosition - m_hitBox.getWidth() / 2, -m_hitBox.getHeight());
	TrapItem* trap = new TrapItem();
	trap->setConstructible(this);
	m_dropedItem.push_back(new DropedItem(trap, m_realPosition - m_hitBox.getWidth() / 2));
}
