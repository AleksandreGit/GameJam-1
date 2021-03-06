#pragma once

#include "Natural.h"
#include "MineralType.h"
#include "./../Inventory/RockItem.h"
#include "DefaultEnvironmentElementBuilder.h"

class Mineral : public Natural {
public:
	Mineral() = default;
	Mineral(MineralType mineralType, int position);
private:
	MineralType m_type;
};