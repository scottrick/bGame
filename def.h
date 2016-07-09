#ifndef DEF_H
#define DEF_H

#include <string>
using namespace std;

const string GAME_NAME = "Four Dragons";

const int NUM_DRAGONS = 4; //number of dragons to slay to win

const int DRAGON_LEVEL = 7; //the level of the dragons

const int BOARD_SIZE = 32;//32;//36; //actual board size is two less, since the outer ring is all unpassable boundary walls
const float lightPosition[] = {0.0f, 0.0f, (-(float)BOARD_SIZE) / 2.0f, 1};

const int NORTH = 1;
const int SOUTH = 2;
const int EAST = 3;
const int WEST = 4;

const int TURN_FRAME_LENGTH = 24;

const float COLOR_VARIATION = 0.04f;

const float MOUNTAINS_CHANCE = 0.004f; //chance a random square will be initially seeded as a mountain
const float GRASS_CHANCE = 0.035f; //chance for grass
const float LAKE_CHANCE = 0.01f; //chance for lake
const float PLAINS_CHANCE = 0.022f; //chance for plains

const int MOUNTAINS = 0;
const int LAKE = 1;
const int GRASS = 2;
const int PLAINS = 3;

const int MAINHAND = 1;
const int OFFHAND = 2;
const int CHEST = 3;
const int HELM = 4;

//Different types of effects
const int MONSTER_DEATH = 1;

//gravity constant per frame
const float GRAVITY = 15.0f;

const float MONSTER_CHANCE = 0.075f;

const int MESSAGE_LIST_SIZE = 20;

const int NUM_ITEM_ATTRIBUTES = 4;

//EQUIPMENT SIZE is determined by the size of the other two;  
const float PLAYER_INFO_SIZE = 0.2f;
const float MESSAGE_INFO_SIZE = 0.35f;

const string MAIN_HAND_STRINGS[] = {"Short Sword", "Broad Sword", "Candlestick", "Mace", "Dagger", "Flail", "Harpoon", "Axe", "Club", "Sabre", "Scimitar", "Blade", "Wand", "Branch", "Spike", "Femur", "Broom"};
const int MAIN_HAND_STRINGS_SIZE = 17;

const string OFF_HAND_STRINGS[] = {"Pumpkin", "Tower Shield", "Round Shield", "Buckler", "Stone", "Shield", "Keg", "Book", "Tome", "Directory", "Anvil", "Coin", "Lantern", "Great Shield", "Chalice", "Mark"};
const int OFF_HAND_STRINGS_SIZE = 16;

const string CHEST_STRINGS[] = {"Leather Armor", "Hide", "Chain Mail", "Plate Mail", "Shirt", "Robes", "Vest", "Dress", "Splint Mail", "Battle Harness", "Jacket", "Cloak", "Raincoat", "Wraps", "Suit"};
const int CHEST_STRINGS_SIZE = 15;

const string HELM_STRINGS[] = {"Helm", "Hat", "Cap", "Wizard's Hat", "Crown", "Tiara", "Visor", "Monocle", "Glasses", "Top Hat", "Sombrero", "Fedora", "Helmet", "Bonnet", "Chapeau"};
const int HELM_STRINGS_SIZE = 15;

const string SUFFIX_STRINGS[] = {"of the Clouds", "of the Seven Towers", "of Wrath", "of Might", "of Grace", "of Majesty", "of Hate", "of the Moon", "of the Forest", "of Rabes", "of Velocity", "of Triumph", "of Vision", "of Wisdom", "of Courage", "of Caution", "of Power", "of Cruelty", "of the Ocean", "of the Sun", "of Winter", "of Sugar"};
const int SUFFIX_STRINGS_SIZE = 22;

const string PREFIX_STRINGS[] = {"Oozing", "Wicked", "Intricate", "Melodious", "King's", "Wizard's", "Troll's", "Dragon", "Ruby", "Emerald", "Ugly", "Toxic", "Renegade's", "Bard's", "Crafty", "Holy", "Unholy", "Chaotic", "Grinning", "Smiling", "Maniac's", "Fiery", "Freezing", "Cunning", "Bleeding", "Bleached", "Perfect"};
const int PREFIX_STRINGS_SIZE = 27;

const string MONSTER_STRINGS[] = {"Necromancer", "Orc", "Snake", "Troll", "Kobold", "Tiger", "Goblin", "Elf", "Robber", "Crook", "Beggar", "Fool", "Count", "Duchess", "Prince", "Maggot", "Hound", "Mummy", "Skeleton", "Ghoul", "Zombie", "Vampire", "Dwarf", "Witch", "Medusa", "Wizard", "Magician", "Warrior", "Cleric", "Barbarian", "Thief", "Archer", "Guardsman", "Knight", "Servant", "Dragonspawn", "Hawk", "Coward", "Spy", "Ninja", "Golem", "Pirate", "Admiral", "Commodore", "Ghost", "Werewolf", "Jester", "Samuari", "Soul", "Executioner", "Chemist", "Hermit", "Noob"};
const int MONSTER_STRINGS_SIZE = 54;

const string MONSTER_ADJ[] = {"Haunted", "Wicked", "Fiery", "Unholy", "Evil", "Vile", "Cunning", "Bleeding", "Oozing", "Cursed", "Crazed", "Psychotic", "Tainted", "Wretched", "Venomous", "Unearthed", "Shadow", "Night", "Moon", "Vexing", "Horned", "Flaming", "Greedy", "Hateful", "Gargantuan", "Granite", "Doomed", "Lost", "Forsaken", "Forgotten", "Abandoned", "Mischievous", "Villainous", "Deadly", "Vicious", "Hungry", "Merciless", "Cunning", "Zealous", "Raging", "Ravaging", "Sinful", "Depraved", "Corrupt", "Malignant", "Malevolent", "Ruinous", "Wasted", "Degenerate", "Bereft", "Cruel", "Cold Blooded", "Frozen", "Barbarous", "Savage", "Brutal", "Sadistic", "Slimy", "Filthy", "Unclean", "Foul", "Treacherous", "Corpulent", "Calloused", "Obese", "Vulgar", "Grotesque", "Disfigured", "Furious", "Mighty", "Powerful", "Crusty"};
const int MONSTER_ADJ_SIZE = 72;

const float MAINHAND_COLOR[] = {0.05f, 0.51f, 1.0f};
const float OFFHAND_COLOR[] = {0.46f, 0.18f, 0.6f};
const float CHEST_COLOR[] = {0.89f, 0.15f, 0.21f};
const float HELM_COLOR[] = {0.04f, 0.79f, 0.17f};
const float STAT_COLOR[] = {0.7f, 0.7f, 0.7f};

const string ATTACK_VERBS[] = {"strike", "hit"} ;
const int ATTACK_VERBS_SIZE = 2;

const string PLAYER_TITLES[] = {"Peasant", "Soldier", "Knight", "Champion"};

//first 21 light values calculated;  anything farther simply gets the lowest value;
const float LIGHT_FADEOUT[] = {1.0f, 0.9f, 0.81f, 0.729f, 0.656f, 0.59f, 0.531f, 0.478f, 0.43f, 0.387f, 0.348f, 0.314f, 0.282f, 0.254f, 0.228f, 0.205f, 0.185f, 0.166f, 0.15f, 0.135f, 0.122f};

const int DRAW_NORMAL = 0;
const int DRAW_INFO = 1;

const string RANDOM_NAMES[] = {"Raphael", "Rand", "Lothar", "Michael", "James", "Richard", "Alexi", "Brienne", "Heather", "Sarah", "Brigham", "Fred", "Ben", "Donatello", "Leonardo", "Splinter", "Catelyn", "Arya", "Jon", "Samwise"};
const int RANDOM_NAMES_SIZE = 20;

const string HIGHSCORE_FILENAME = "scores.bgame";

const float MONSTER_SPIN_MIN = 12.0f;
const float MONSTER_SPIN_MAX = 30.0f;

const float DRAGON_SPIN = 210.0f;

const int MONSTER_POLY_MIN = 3;
const int MONSTER_POLY_MAX = 9;

const float TERRAIN_VAR = 0.5f;

const float ITEM_DROP_CHANCE = 0.5f;

const float MONSTER_RANDOM_MOVE = 0.6f;

#endif