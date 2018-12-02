#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleGameLevel.h"
#include "ModuleMainMenu.h"
#include "FontManager.h"
#include <math.h>
#include <string>
#include <stdio.h>
#include <fstream>

#include "Box2D/Box2D/Box2D.h"

#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif

ModuleGameLevel::ModuleGameLevel(bool active) : Module(active)
{}

ModuleGameLevel::~ModuleGameLevel()
{}

bool ModuleGameLevel::Start()
{
	//Reseting all values for a fresh start
	lives = 3;
	windStrength = 0;
	score = 0;
	highScore = LoadHighscore();
	spin = 90;
	spinSpeed = 2;
	hook.x = SCREEN_WIDTH / 2;
	hook.y = 40;
	xHookRadius = 100.0f;
	yHookRadius = 20.0f;
	startingGame = true;
	throwAllowed = true;
	triggerGameOver = false;
	App->renderer->camera.y = 0;

	//Loading all game textures
	background = App->textures->Load("assets/game_background.png");
	backgroundSecondLayer = App->textures->Load("assets/backgroundSecondLayer.png");
	blockTexture = App->textures->Load("assets/blockSkin.png");
	hookTexture = App->textures->Load("assets/hookSkin.png");
	softWind = App->textures->Load("assets/softWind.png");
	mediumWind = App->textures->Load("assets/mediumWind.png");
	strongWind = App->textures->Load("assets/strongWind.png");

	//Loading all FX
	blockDrop = App->audio->LoadFx("assets/sfx/blockDrop.wav");
	blockFail = App->audio->LoadFx("assets/sfx/blockBroken.wav");
	blockSuccess = App->audio->LoadFx("assets/sfx/blockOk.wav");
	blockPerfect = App->audio->LoadFx("assets/sfx/blockPerfect.wav");

	//Initialize the physics world, create the floor of the world and smile because you did it in less than 7 days.
	physicsEnvironment = new b2World(b2Vec2(0, 13));
	physicsEnvironment->SetContactListener(this);

	floorBlock = BuildBlock(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 5, SCREEN_WIDTH * 10, 10, b2_staticBody);
	gameBlocks.push_back(floorBlock);
	lastBlock = floorBlock;

	return true;
}

update_status ModuleGameLevel::Update()
{
	App->renderer->Blit(0.0f, false, background, 0.0f, 0.0f, nullptr);
	App->renderer->Blit(0.0f, true, backgroundSecondLayer, 0.0f, 0.0f, nullptr);
	
	for (std::vector<Block*>::iterator it = gameBlocks.begin(); it != gameBlocks.end(); ++it)
	{
		if ((*it) != floorBlock)
		{
			App->renderer->Blit((*it)->GetAngleInDegree(), true, blockTexture, (float)((*it)->GetXCoord() - (BLOCK_WIDTH / 2)), (float)((*it)->GetYCoord() - (BLOCK_HEIGHT / 2)), nullptr);
		}
	}

	PrintGUI();

	if (!triggerGameOver)
	{
		if (lives <= 0)
		{
			if(score >= highScore)
				SaveHighscore();

			triggerGameOver = true;
		}

		App->renderer->UpdateCameraPosition(lastBlock->GetYCoord());

		DestroyLastBlockIfNeeded();

		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && throwAllowed)
		{
			App->audio->PlayFx(blockDrop);

			if (startingGame)
				startingGame = false;

			throwAllowed = false;
			gameBlocks.push_back(BuildBlock((int)hook.x, (int)hook.y - App->renderer->camera.y + (BLOCK_HEIGHT / 2)));
		}

		UpdateHook();
		physicsEnvironment->Step(1.0f / 60.0f, 6, 2);

		for (std::vector<b2Contact*>::iterator it = jointsToDo.begin(); it != jointsToDo.end(); ++it)
		{
			GenerateJointIfCorrectCollision((*it));
		}

		jointsToDo.clear();
	}
	else
	{
		ShowGameOverScreen();
		if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{
			App->ChangeScene(App->main_menu, this);
		}
	}

	return UPDATE_CONTINUE;
}

bool ModuleGameLevel::CleanUp()
{
	bool ret = true;

	App->textures->Unload(background);
	App->textures->Unload(backgroundSecondLayer);
	App->textures->Unload(blockTexture);
	App->textures->Unload(hookTexture);
	App->textures->Unload(softWind);
	App->textures->Unload(mediumWind);
	App->textures->Unload(strongWind);

	for (std::vector<b2Joint*>::iterator it = blockJoints.begin(); it != blockJoints.end(); ++it)
	{
		physicsEnvironment->DestroyJoint((*it));
	}

	blockJoints.clear();

	for (std::vector<Block*>::iterator it = gameBlocks.begin(); it != gameBlocks.end(); ++it)
	{
		physicsEnvironment->DestroyBody((*it)->body);
		delete (*it);
	}

	gameBlocks.clear();
	delete physicsEnvironment;

	return ret;
}

const void ModuleGameLevel::PrintGUI()
{
	float chWidth = (float)App->fontManager->greenFont->characterWidth;
	float chHeight = (float)App->fontManager->greenFont->characterHeight;

	App->fontManager->greenFont->printText("SCORE", 5.0f, 5.0f, 1.7f);
	App->fontManager->redFont->printText(std::to_string(score).c_str(), chWidth * 10.0f, 5.0f, 1.7f);

	App->fontManager->greenFont->printText("HIGH", 5.0f, 15.0f + chHeight, 1.7f);
	App->fontManager->redFont->printText(std::to_string(highScore).c_str(), chWidth * 10.0f, 15.0f + chHeight, 1.7f);

	App->fontManager->greenFont->printText("LIVES", 5.0f, SCREEN_HEIGHT - chHeight * 2.0f, 1.7f);
	App->fontManager->redFont->printText(std::to_string(lives).c_str(), chWidth * 10.0f, SCREEN_HEIGHT - chHeight * 2.0f, 1.7f);

	App->fontManager->greenFont->printText("WIND", 5.0f, ((float)SCREEN_HEIGHT / 2.0f) - chHeight * 2.0f, 1.7f);
	SDL_RendererFlip flip = (windStrength > 0) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
	switch (abs(windStrength))
	{
	case 1:
		App->renderer->Blit(0.0f, false, softWind, 15.0f, (SCREEN_HEIGHT / 2) + 4, nullptr, -1.0f, -1.0f, flip);
		break;
	case 2:
		App->renderer->Blit(0.0f, false, mediumWind, 15.0f, (SCREEN_HEIGHT / 2) + 4, nullptr, -1.0f, -1.0f, flip);
		break;
	case 3:
		App->renderer->Blit(0.0f, false, strongWind, 15.0f, (SCREEN_HEIGHT / 2) + 4, nullptr, -1.0f, -1.0f, flip);
		break;
	}

	if (startingGame)
		App->fontManager->redFont->printText("PRESS SPACE!", (SCREEN_WIDTH / 2) - 88, (SCREEN_HEIGHT / 2) - 15, 2.0f);
}

const void ModuleGameLevel::ShowGameOverScreen()
{
	SDL_Rect screen = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	App->renderer->DrawQuad(screen, 0, 0, 0, 150);

	int cWidth = App->fontManager->redFont->characterWidth;
	int cHeight = App->fontManager->redFont->characterHeight;

	App->fontManager->redFont->printText("GAME OVER", (float)((SCREEN_WIDTH / 2) - cWidth * 9), (float)(SCREEN_HEIGHT / 2), 2.0f);
}

//This three functions are the gameplay core. GenerateJoint is called when a collision between the last block and the top of the tower is detected.
//If this last block is inside the boundaries (not too far to the left or right) of the top of the tower, a joint will be created on a certain calculated
//position of both of the bodies. This position is calculated on GetSnapPoint, depending on where the centers of the body where when the collision happened.
const void ModuleGameLevel::GenerateJointIfCorrectCollision(b2Contact * contact)
{
	float snapPoint = 0.0f;

	int x = gameBlocks.back()->GetXCoord();
	int y = gameBlocks.back()->GetYCoord();

	int x2 = lastBlock->GetXCoord();
	int y2 = lastBlock->GetYCoord();

	if (IsInsideBoundaries(x, y, x2, y2))
	{
		snapPoint = GetSnapPoint(x, y, x2, y2);

		//Generate a PrismaticJoint in the given SnapPoint
		b2PrismaticJointDef pDef;
		pDef.bodyA = contact->GetFixtureA()->GetBody();
		pDef.bodyB = contact->GetFixtureB()->GetBody();
		pDef.collideConnected = true;
		pDef.localAxisA = b2Vec2(0.0f, 1.0f);

		if (lastBlock == floorBlock)
		{
			pDef.localAnchorA = b2Vec2(0.0f, 0.0f);
			pDef.localAnchorB = b2Vec2(0.0f, 0.0f);
		}
		else
		{
			pDef.localAnchorA = b2Vec2(snapPoint, snapPoint);
			pDef.localAnchorB = b2Vec2(-snapPoint, -snapPoint);
		}

		//Save the joints for previous cleansing
		blockJoints.push_back(physicsEnvironment->CreateJoint(&pDef));
		
		//Make sure that the joint block is saved as LastBlock
		lastBlock = gameBlocks.back();

		//When a block has 6 others over him, block its rotation. This wont allow the tower to fall but will let it be wiggly.
		if (gameBlocks.size() > 7)
			gameBlocks[gameBlocks.size() - 7]->body->SetFixedRotation(true);

		AddPoints(snapPoint * 10);
		ChangeWind();

		throwAllowed = true;
	}
	else
	{
		destroyLastBlock = true;
	}
}

const bool ModuleGameLevel::IsInsideBoundaries(const int& x, const int& y, const int& x2, const int& y2) const
{
	bool toReturn = true;

	if ((x < (x2 - (BLOCK_WIDTH / 2) - BLOCK_PORTION) || x >(x2 + (BLOCK_WIDTH / 2) + BLOCK_PORTION)))
		toReturn = false;

	return toReturn;
}

const float ModuleGameLevel::GetSnapPoint(const int & x, const int & y, const int & x2, const int & y2)
{
	float toReturn = 0.0f;

	if (x <= x2)
	{
		if (x >= x2 - BLOCK_PORTION)
			toReturn = 0.0f;
		else if (x >= x2 - (BLOCK_PORTION * 2))
			toReturn = -0.1f;
		else if (x >= x2 - (BLOCK_PORTION * 3))
			toReturn = -0.20f;
		else
			toReturn = -0.30f;
	}
	else
	{
		if (x <= x2 + BLOCK_PORTION)
			toReturn = 0.0f;
		else if (x <= x2 + (BLOCK_PORTION * 2))
			toReturn = 0.1f;
		else if (x <= x2 + (BLOCK_PORTION * 3))
			toReturn = 0.20f;
		else
			toReturn = 0.30f;
	}

	return toReturn;
}

//Changes the direction and strength of the wind randomly each turn
const void ModuleGameLevel::ChangeWind()
{
	double randomValue = rand() / (double)(RAND_MAX + 1);
	windStrength = 1 + (int)(randomValue * 3);

	randomValue = rand() / (double)(RAND_MAX + 1);
	int windDirection = ((1 + (int)(randomValue * 2)) % 2 == 0) ? 1 : -1;

	if (windDirection < 0)
	{
		windStrength *= windDirection;
	}

	physicsEnvironment->SetGravity(b2Vec2(windStrength / 1.5f, 13));
}

//Updates hook (player) position and controls its behaviour
const void ModuleGameLevel::UpdateHook()
{	
	hook.x = xHookRadius * (float)cos(spin * M_PI / 180.0f) + (SCREEN_WIDTH / 2);
	hook.y = yHookRadius * (float)sin(spin * M_PI / 180.0f) + 60;
	int hookBaseX = (int)((xHookRadius / 2) * cos(spin * M_PI / 180.0f) + (SCREEN_WIDTH / 2));
	
	if (!startingGame)
	{
		spin += spinSpeed;

		if (spin == 360)
			spin = 0;
	}	

	if (throwAllowed)
		App->renderer->Blit(0, false, blockTexture, hook.x - (BLOCK_WIDTH / 2), hook.y, nullptr);

	App->renderer->Blit(0, false, hookTexture, hook.x - 10.0f, hook.y - 4.0f, nullptr);
	App->renderer->DrawLine(false, hookBaseX, 0, (int)hook.x, (int)hook.y, 0, 0, 0);
	App->renderer->DrawLine(false, hookBaseX - 3, 0, (int)hook.x - 3, (int)hook.y, 0, 0, 0);
	App->renderer->DrawLine(false, hookBaseX + 3, 0, (int)hook.x + 3, (int)hook.y, 0, 0, 0);
}

//Checks if the last block added to the game needs to be destroyed.
const void ModuleGameLevel::DestroyLastBlockIfNeeded()
{
	//The second check needs to be done because Blox2D sometimes detect repeated collisions over
	//bodies that where already checked because of physical movement. With this, we ensure that
	//the system does not try to delete a block in the middle of the tower when moving.
	if (destroyLastBlock && (lastBlock != gameBlocks.back()))
	{
		App->audio->PlayFx(blockFail);

		physicsEnvironment->DestroyBody(gameBlocks.back()->body);
		delete gameBlocks.back();
		gameBlocks.pop_back();

		lives -= 1;
		throwAllowed = true;
	}

	destroyLastBlock = false;
}

//Block (main game element) factory
Block * ModuleGameLevel::BuildBlock(const int& x, const int& y, const int& width, const int& heigth, b2BodyType blockType) const
{
	b2BodyDef blockDef;
	blockDef.type = blockType;
	blockDef.position.Set(PIXELS_TO_METERS(x), PIXELS_TO_METERS(y));

	//Define a Shape
	b2PolygonShape blockShape;
	blockShape.SetAsBox(PIXELS_TO_METERS(width) * 0.5f, PIXELS_TO_METERS(heigth) * 0.5f);

	//Create a fixture and like the shape or shapes
	b2FixtureDef blockFixture;
	blockFixture.shape = &blockShape;
	blockFixture.density = 9999.0f;
	blockFixture.friction = 9999.0f;

	//Generate the body with the fixture
	b2Body * blockBody = physicsEnvironment->CreateBody(&blockDef);
	blockBody->CreateFixture(&blockFixture);

	Block * newBlock = new Block();
	newBlock->body = blockBody;
	newBlock->width = width;
	newBlock->height = heigth;

	return newBlock;
}

//Game Score control (Adding // Saving and loading highscore)
const void ModuleGameLevel::AddPoints(const float& snapValue)
{
	if (abs(snapValue) == 0.0f)
		App->audio->PlayFx(blockPerfect);
	else
		App->audio->PlayFx(blockSuccess);

	score += (100 - (int)(abs(snapValue) * 10.0f));
	if (score > highScore)
		highScore = score;
}

const void ModuleGameLevel::SaveHighscore() const
{
	std::ofstream highScoreFile;
	highScoreFile.open("data/gameInfo.txt", std::ofstream::out);
	highScoreFile << highScore;
	highScoreFile.close();
}

const int ModuleGameLevel::LoadHighscore()
{
	std::string stringScore;
	std::ifstream highScoreFile("data/gameInfo.txt", std::ifstream::binary);

	if (highScoreFile)
	{
		std::ifstream read("data/gameInfo.txt");
		read >> stringScore;
		return std::stoi(stringScore);
	}
	else
		return 0;
}

//Box2D Collision Callback - Collisions are filtered to avoid checking or dealing with random collisions over the tower
void ModuleGameLevel::BeginContact(b2Contact * contact)
{
	if (contact->GetFixtureA()->GetBody() == lastBlock->body)
	{
		jointsToDo.push_back(contact);
	}
	else if (contact->GetFixtureA()->GetBody() == gameBlocks.back()->body || contact->GetFixtureB()->GetBody() == gameBlocks.back()->body)
	{
		destroyLastBlock = true;
	}
}