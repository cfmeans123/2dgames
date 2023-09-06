#include "Item.h"

Item::Item(ItemType type)
	: mType(type)
{
	switch (mType)
	{
	case PurpleToken:
		//this->initWithFile("Token/purpletoken");

		break;

	case GreenToken:
		//this->initWithFile("Token/greentoken");
		break;

	case PurpleEssence:

		break;

	case GreenEssence:

		break;
	}
};