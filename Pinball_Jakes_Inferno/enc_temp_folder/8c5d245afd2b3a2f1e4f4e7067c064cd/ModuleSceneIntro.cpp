#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ChainPoints.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	

	//Load Textures
	background_tex = App->textures->Load("Sprites/background.png");
	left_piece1_tex = App->textures->Load("Sprites/left_piece1.png");
	right_piece1_tex = App->textures->Load("Sprites/right_piece1.png");
	left_piece2_tex = App->textures->Load("Sprites/left_piece2.png");
	right_piece2_tex = App->textures->Load("Sprites/right_piece2.png");
	left_wheel_piece_tex = App->textures->Load("Sprites/left_wheel_piece.png");
	right_wheel_piece_tex = App->textures->Load("Sprites/right_wheel_piece.png");
	left_flipper_tex = App->textures->Load("Sprites/left_flipper.png");
	right_flipper_tex = App->textures->Load("Sprites/right_flipper.png");

	//Load Audio
	flipper_sound = App->audio->LoadFx("Audio/flipper_sound.wav");



	//Adding physic Background and statics elements
	background = App->physics->CreateStaticChain(25, 34, background_points, 238);
	left_piece1 = App->physics->CreateStaticChain(59, 939, left_piece1_points, 19);
	right_piece1 = App->physics->CreateStaticChain(25, 34, right_piece1_points, 19);
	left_piece2 = App->physics->CreateStaticChain(25, 34, left_piece2_points, 19);
	right_piece2 = App->physics->CreateStaticChain(25, 34, right_piece2_points, 19);
	left_wheel_piece = App->physics->CreateStaticChain(67, 90, left_wheel_piece_points, 19);
	right_wheel_piece = App->physics->CreateStaticChain(235, 90, right_wheel_piece_points, 19);


	//Flippers
	left_flipper = App->physics->CreateChain(115, 521, left_flipper_points, 19);
	left_flipper->body->SetGravityScale(0.0f);
	right_flipper = App->physics->CreateChain(207, 521, right_flipper_points, 21);
	right_flipper->body->SetGravityScale(0.0f);
	left_flipper_bot = App->physics->CreateChain(115, 521, left_flipper_points, 19);
	left_flipper_bot->body->SetGravityScale(0.0f);
	right_flipper_bot = App->physics->CreateChain(207, 521, right_flipper_points, 21);
	right_flipper_bot->body->SetGravityScale(0.0f);

	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	b2Body* b = App->physics->world->CreateBody(&bd);

	b2PolygonShape dshape;
	dshape.SetAsBox(PIXEL_TO_METERS(32), PIXEL_TO_METERS(16));

	b2FixtureDef dummyfix;
	dummyfix.shape = &dshape;
	dummyfix.density = 1.0f;

	b->CreateFixture(&dummyfix);

	b2MassData* massdata = new b2MassData();
	b->GetMassData(massdata);
	left_flipper->body->SetMassData(massdata);
	right_flipper->body->SetMassData(massdata);

	b2RevoluteJointDef left_flipper_def;
	left_flipper_def.bodyA = background->body;
	left_flipper_def.bodyB = left_flipper->body;
	left_flipper_def.Initialize(left_flipper_def.bodyA, left_flipper_def.bodyB, { PIXEL_TO_METERS(110), PIXEL_TO_METERS(520) });
	left_flipper_def.enableLimit = true;
	left_flipper_def.lowerAngle = -0.15f * b2_pi; 
	left_flipper_def.enableMotor = true;
	left_flipper_def.maxMotorTorque = 10.0f;
	left_flipper_def.motorSpeed = 10.0f;
	left_flipper_joint = (b2RevoluteJoint*)App->physics->world->CreateJoint(&left_flipper_def);

	b2RevoluteJointDef right_flipper_def;
	right_flipper_def.bodyA = background->body;
	right_flipper_def.bodyB = right_flipper->body;
	right_flipper_def.Initialize(right_flipper_def.bodyA, right_flipper_def.bodyB, { PIXEL_TO_METERS(250), PIXEL_TO_METERS(520) });
	right_flipper_def.enableLimit = true;
	right_flipper_def.upperAngle = 0.19f * b2_pi; // 45 degrees
	right_flipper_def.enableMotor = true;
	right_flipper_def.maxMotorTorque = 10.0f;
	right_flipper_def.motorSpeed = -10.0f;
	right_flipper_joint = (b2RevoluteJoint*)App->physics->world->CreateJoint(&right_flipper_def);

	b2RevoluteJointDef left_flipper_bot_def;
	left_flipper_bot_def.bodyA = background->body;
	left_flipper_bot_def.bodyB = left_flipper_bot->body;
	left_flipper_bot_def.Initialize(left_flipper_bot_def.bodyA, left_flipper_bot_def.bodyB, { PIXEL_TO_METERS(110), PIXEL_TO_METERS(520) });
	left_flipper_bot_def.enableLimit = true;
	left_flipper_bot_def.lowerAngle = -0.15f * b2_pi;
	left_flipper_bot_def.enableMotor = true;
	left_flipper_bot_def.maxMotorTorque = 10.0f;
	left_flipper_bot_def.motorSpeed = 10.0f;
	left_flipper_bot_joint = (b2RevoluteJoint*)App->physics->world->CreateJoint(&left_flipper_bot_def);

	b2RevoluteJointDef right_flipper_bot_def;
	right_flipper_bot_def.bodyA = background->body;
	right_flipper_bot_def.bodyB = right_flipper_bot->body;
	right_flipper_bot_def.Initialize(right_flipper_bot_def.bodyA, right_flipper_bot_def.bodyB, { PIXEL_TO_METERS(250), PIXEL_TO_METERS(520) });
	right_flipper_bot_def.enableLimit = true;
	right_flipper_bot_def.upperAngle = 0.19f * b2_pi; // 45 degrees
	right_flipper_bot_def.enableMotor = true;
	right_flipper_bot_def.maxMotorTorque = 10.0f;
	right_flipper_bot_def.motorSpeed = -10.0f;
	right_flipper_bot_joint = (b2RevoluteJoint*)App->physics->world->CreateJoint(&right_flipper_bot_def);



	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	App->textures->Unload(background_tex);
	App->textures->Unload(left_piece1_tex);
	App->textures->Unload(right_piece1_tex);
	App->textures->Unload(left_piece2_tex);
	App->textures->Unload(right_piece2_tex);
	App->textures->Unload(left_wheel_piece_tex);
	App->textures->Unload(right_wheel_piece_tex);
	App->textures->Unload(left_flipper_tex);
	App->textures->Unload(right_flipper_tex);

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	
	
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		left_flipper->body->ApplyForce({ 10, 80 }, { 0, 0 }, true);
		left_flipper->body->ApplyTorque(-500, true);

		left_flipper_bot->body->ApplyForce({ 10, 80 }, { 0, 0 }, true);
		left_flipper_bot->body->ApplyTorque(-500, true);

		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
		{
			left_flipper->body->ApplyForce({ -10, -80 }, { 0, 0 }, true);
			left_flipper_bot->body->ApplyForce({ -10, -80 }, { 0, 0 }, true);
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		right_flipper->body->ApplyForce({ 10, 80 }, { 0, 0 }, true);
		right_flipper->body->ApplyTorque(500, true);

		right_flipper_bot->body->ApplyForce({ 10, 80 }, { 0, 0 }, true);
		right_flipper_bot->body->ApplyTorque(500, true);

		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
		{
			
			right_flipper->body->ApplyForce({ -10, -80 }, { 0, 0 }, true);
			right_flipper_bot->body->ApplyForce({ -10, -80 }, { 0, 0 }, true);
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN))
	{
		App->audio->PlayFx(flipper_sound);
	}

	


	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();

	// All draw functions ------------------------------------------------------
	//Background and statics elements
	App->renderer->Blit(background_tex, METERS_TO_PIXELS(background->body->GetPosition().x) - 25, METERS_TO_PIXELS(background->body->GetPosition().y) - 34);
	App->renderer->Blit(left_piece1_tex, METERS_TO_PIXELS(left_piece1->body->GetPosition().x), METERS_TO_PIXELS(left_piece1->body->GetPosition().y));
	App->renderer->Blit(right_piece1_tex, METERS_TO_PIXELS(right_piece1->body->GetPosition().x), METERS_TO_PIXELS(right_piece1->body->GetPosition().y));
	App->renderer->Blit(left_piece2_tex, METERS_TO_PIXELS(left_piece2->body->GetPosition().x), METERS_TO_PIXELS(left_piece2->body->GetPosition().y));
	App->renderer->Blit(right_piece2_tex, METERS_TO_PIXELS(right_piece2->body->GetPosition().x), METERS_TO_PIXELS(right_piece2->body->GetPosition().y));
	App->renderer->Blit(left_wheel_piece_tex, METERS_TO_PIXELS(left_wheel_piece->body->GetPosition().x), METERS_TO_PIXELS(left_wheel_piece->body->GetPosition().y));
	App->renderer->Blit(right_wheel_piece_tex, METERS_TO_PIXELS(right_wheel_piece->body->GetPosition().x), METERS_TO_PIXELS(right_wheel_piece->body->GetPosition().y));
	//Flippers
	App->renderer->Blit(left_flipper_tex, METERS_TO_PIXELS(left_flipper->body->GetPosition().x), METERS_TO_PIXELS(left_flipper->body->GetPosition().y), NULL, 1.0F, left_flipper->GetRotation(), PIXEL_TO_METERS(1), PIXEL_TO_METERS(1));
	App->renderer->Blit(right_flipper_tex, METERS_TO_PIXELS(right_flipper->body->GetPosition().x), METERS_TO_PIXELS(right_flipper->body->GetPosition().y), NULL, 1.0F, right_flipper->GetRotation(), PIXEL_TO_METERS(1), PIXEL_TO_METERS(1));
	App->renderer->Blit(left_flipper_tex, METERS_TO_PIXELS(left_flipper_bot->body->GetPosition().x), METERS_TO_PIXELS(left_flipper_bot->body->GetPosition().y), NULL, 1.0F, left_flipper_bot->GetRotation(), PIXEL_TO_METERS(1), PIXEL_TO_METERS(1));
	App->renderer->Blit(right_flipper_tex, METERS_TO_PIXELS(right_flipper_bot->body->GetPosition().x), METERS_TO_PIXELS(right_flipper_bot->body->GetPosition().y), NULL, 1.0F, right_flipper_bot->GetRotation(), PIXEL_TO_METERS(1), PIXEL_TO_METERS(1));
	

	

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	//App->audio->PlayFx(flipper_sound);
}