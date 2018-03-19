#pragma once
#include "Item.h"
#include "Explosive.h"
#include "Throwable.h"
#include "MeleeWeapon.h"


class ItemSpawner
{
public:
	ItemSpawner() = default;
	ItemSpawner(SpawnHandler* _spawner);
	~ItemSpawner();

	//loading stuff
	void loadAllData(RenderData* _RD);
	void loadItem(RenderData* _RD, std::string item_file);
	void loadAllItemProperies(Item* item, std::ifstream& _opened_file);
	void loadThrowableProperies(Throwable* item, std::ifstream& _opened_file);
	void loadExplosiveProperies(Explosive* item, std::ifstream& _opened_file);

	Item* createNewItemWithName(std::string name);

private:
	SpawnHandler* m_spawner;
	std::vector<Item*> allItems;
};

//Item * newInstance = new Item(original_instance) ->makes a copy


