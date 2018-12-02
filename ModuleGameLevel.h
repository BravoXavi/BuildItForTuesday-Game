#ifndef __MODULESTAGE_H__
#define __MODULESTAGE_H__

#include "Module.h"
#include "Font.h"
#include "Block.h"
#include "Box2D/Box2D/Box2D.h"
#include <list>

struct SDL_Texture;
class b2World;

class ModuleGameLevel : public Module, public b2ContactListener
{
public:
	ModuleGameLevel(bool active = true);
	~ModuleGameLevel();

	bool Start();
	update_status Update();
	bool CleanUp();

private:
	const void PrintGUI();
	const void ShowGameOverScreen();
	
	//Block (main game element) factory
	Block * BuildBlock(const int& x, const int& y, const int& width = BLOCK_WIDTH, const int& heigth = BLOCK_HEIGHT, b2BodyType blockType = b2_dynamicBody) const;

	//This three functions are the gameplay core. GenerateJoint is called when a collision between the last block and the top of the tower is detected.
	//If this last block is inside the boundaries (not too far to the left or right) of the top of the tower, a joint will be created on a certain calculated
	//position of both of the bodies. This position is calculated on GetSnapPoint, depending on where the centers of the body where when the collision happened.
	const void GenerateJointIfCorrectCollision(b2Contact * contact);
	const bool IsInsideBoundaries(const int& x, const int& y, const int& x2, const int&y2) const;
	const float GetSnapPoint(const int& x, const int& y, const int& x2, const int& y2);

	//Updates hook (player) position and controls its behaviour
	const void UpdateHook();
	
	//Checks if the last block added to the game needs to be destroyed.
	const void DestroyLastBlockIfNeeded();
	
	//Changes the direction and strength of the wind randomly each turn
	const void ChangeWind();

	//Game Score control (Adding // Saving and loading highscore)
	const void AddPoints(const float& snapValue);
	const void SaveHighscore() const;
	const int LoadHighscore();

	//Box2D Collision Callback
	void BeginContact(b2Contact * contact);

private:
	Block * lastBlock = nullptr;
	Block * floorBlock = nullptr;

	b2World * physicsEnvironment = nullptr;

	std::vector<Block*> gameBlocks;
	std::vector<b2Contact*> jointsToDo;
	std::vector<b2Joint*> blockJoints;

	SDL_Texture * background;
	SDL_Texture * backgroundSecondLayer;
	SDL_Texture * blockTexture;
	SDL_Texture * hookTexture;
	SDL_Texture * softWind;
	SDL_Texture * mediumWind;
	SDL_Texture * strongWind;

	Vector2 hook;
	float xHookRadius;
	float yHookRadius;

	bool startingGame = true;
	bool throwAllowed = true;

	bool destroyLastBlock = false;
	bool triggerGameOver = false;
	int lives;
	int score;
	int highScore;
	int windStrength;
	int spin;
	int spinSpeed;

	unsigned int blockDrop;
	unsigned int blockFail;
	unsigned int blockSuccess;
	unsigned int blockPerfect;
};

#endif // __MODULESTAGE_H__