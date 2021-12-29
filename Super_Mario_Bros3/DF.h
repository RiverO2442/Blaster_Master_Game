#pragma once
//GENERAL
#define STORING_LOCATION_X		-99999
#define STORING_LOCATION_Y		-99999

#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_GOOMBA	2
#define OBJECT_TYPE_LASERGUARD	3
#define OBJECT_TYPE_BALLCARRY	4
#define OBJECT_TYPE_BALLBOT	5
#define OBJECT_TYPE_DRAP	6
#define OBJECT_TYPE_CGX680	7
#define OBJECT_TYPE_CGX680S	8
#define OBJECT_TYPE_CSTUKA	9
#define OBJECT_TYPE_EYELET	10
#define OBJECT_TYPE_CINTERCRUPT	11
#define OBJECT_TYPE_CINTERCRUPT_BULLET	12
#define OBJECT_TYPE_RED_WORM	13
#define OBJECT_TYPE_EFFECT	14
#define OBJECT_TYPE_CBOOM	15
#define OBJECT_TYPE_JASON	16
#define OBJECT_TYPE_NoCollisionObject	17
#define OBJECT_TYPE_JASON_BULLET_1	18
#define OBJECT_TYPE_JASON_GRENADE	19
#define OBJECT_TYPE_CGX_BULLET	20
#define OBJECT_TYPE_CLASER_BULLET	21
#define OBJECT_TYPE_STATBAR	22
#define OBJECT_TYPE_MINI_JASON	23
#define OBJECT_TYPE_TANKDOOR	24

#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 1024

#define SCENE_SECTION_MapObj	7


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
#define SCENE_SECTION_MAP	7
#define SCENE_SECTION_QUADTREE	8
#define SCENE_SECTION_SETTING	9
#define SCENE_SECTION_MAPCAM	10

#define OBJECT_TYPE_SOPHIA	0
#define OBJECT_TYPE_TANK_WHEEL	100
#define OBJECT_TYPE_TANK_BODY	101
#define OBJECT_TYPE_TANK_TURRET	102
#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_CTANKBULLET	2


#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 1024
//PLAYER

#define TANKWHEEL_STATE_DIE 0
#define TANKWHEEL_BBOX_WIDTH 10
#define TANKWHEEL_BBOX_HEIGHT 10

#define TANKWHEEL_WHEEL_DISTANT_X_1 1
#define TANKWHEEL_WHEEL_DISTANT_X_2 17
#define TANKWHEEL_WHEEL_DISTANT_Y 10

#define TURRET_ANI_IDLE_LEFT 1
#define TURRET_ANI_IDLE_RIGHT 0
#define TURRET_DISTANT_X 0
#define TURRET_DISTANT_Y 0

#define BODY_DISTANT_X 9
#define BODY_DISTANT_Y 9

#define TANKWHEEL_LEFT_WHEEL 0
#define TANKWHEEL_RIGHT_WHEEL 1
#define TANKWHEEL_TURRET 2

#define WHEELING_ANI_RIGHT 0
#define WHEELING_ANI_LEFT 1
#define WHEELING_ANI_IDLE 2

#define TANKBODY_ANI_IDLE 0

#define TURRET_ANI_IDLE_LEFT 1
#define TURRET_ANI_IDLE_RIGHT 0

#define TANKWHEEL_TURRET_DISTANT_X 9
#define TANKWHEEL_TURRET_DISTANT_Y 10
////////////////////////////JASON
#define JASON_WALKING_SPEED		0.15f
//0.1f
#define JASON_JUMP_SPEED_Y		0.5f
#define JASON_JUMP_DEFLECT_SPEED 0.2f
#define JASON_GRAVITY			0.002f
#define JASON_DIE_DEFLECT_SPEED	 0.5f

#define JASON_STATE_IDLE			0
#define JASON_STATE_WALKING_RIGHT	100
#define JASON_STATE_WALKING_LEFT	200
#define JASON_STATE_WALKING_UP	500
#define JASON_STATE_WALKING_DOWN	600
#define JASON_STATE_JUMP			300
#define JASON_STATE_DIE				400

#define JASON_ANI_WALK_UP		2
#define JASON_ANI_WALK_LEFT			1
#define JASON_ANI_WALK_DOWN		0
#define JASON_ANI_WALK_RIGHT		3
#define JASON_ANI_IDLE_UP		4
#define JASON_ANI_IDLE_LEFT			5
#define JASON_ANI_IDLE_DOWN		6
#define JASON_ANI_IDLE_RIGHT		7


#define JASON_ANI_DIE				8

#define JASON_BIG_BBOX_WIDTH  26
#define JASON_BIG_BBOX_HEIGHT 18


#define JASON_SMALL_BBOX_WIDTH  13
#define JASON_SMALL_BBOX_HEIGHT 15

#define JASON_UNTOUCHABLE_TIME 5000
#define JASON_FIRING_DELAY_TIME 120

///////////////////////////
#define SOPHIA_WALKING_SPEED		0.15f
//0.1f
#define SOPHIA_JUMP_SPEED_Y		0.5f
#define SOPHIA_JUMP_DEFLECT_SPEED 0.2f
#define SOPHIA_GRAVITY			0.002f
#define SOPHIA_DIE_DEFLECT_SPEED	 0.5f

#define SOPHIA_STATE_IDLE			0
#define SOPHIA_STATE_WALKING_RIGHT	100
#define SOPHIA_STATE_WALKING_LEFT	200
#define SOPHIA_STATE_WALKING_UP	500
#define SOPHIA_STATE_WALKING_DOWN	600
#define SOPHIA_STATE_JUMP			300
#define SOPHIA_STATE_DIE				400

#define SOPHIA_ANI_BIG_IDLE_RIGHT		0
#define SOPHIA_ANI_BIG_IDLE_LEFT			1
#define SOPHIA_ANI_SMALL_IDLE_RIGHT		2
#define SOPHIA_ANI_SMALL_IDLE_LEFT			3

#define SOPHIA_ANI_BIG_WALKING_RIGHT			4
#define SOPHIA_ANI_BIG_WALKING_LEFT			5
#define SOPHIA_ANI_SMALL_WALKING_RIGHT		6
#define SOPHIA_ANI_SMALL_WALKING_LEFT		7

#define SOPHIA_ANI_DIE				8

#define	SOPHIA_LEVEL_SMALL	1
#define	SOPHIA_LEVEL_BIG		2

#define SOPHIA_BIG_BBOX_WIDTH  26
#define SOPHIA_BIG_BBOX_HEIGHT 18


#define SOPHIA_SMALL_BBOX_WIDTH  13
#define SOPHIA_SMALL_BBOX_HEIGHT 15

#define SOPHIA_UNTOUCHABLE_TIME 5000
#define SOPHIA_FIRING_DELAY_TIME 500

/////////////

#define MINI_JASON_WALKING_SPEED		0.15f
#define MINI_JASON_JUMP_SPEED_Y		0.5f
#define MINI_JASON_JUMP_DEFLECT_SPEED 0.2f
#define MINI_JASON_GRAVITY			0.002f
#define MINI_JASON_DIE_DEFLECT_SPEED	 0.5f

#define MINI_JASON_STATE_IDLE			0
#define MINI_JASON_STATE_WALKING_RIGHT	100
#define MINI_JASON_STATE_WALKING_LEFT	200
#define MINI_JASON_STATE_JUMP			300
#define MINI_JASON_STATE_DIE				400


#define MINI_JASON_ANI_WALKING_LEFT			0
#define MINI_JASON_ANI_WALKING_RIGHT		1
#define MINI_JASON_ANI_IDLE_LEFT			2
#define MINI_JASON_ANI_IDLE_RIGHT		3
#define MINI_JASON_ANI_JUMP_LEFT			4
#define MINI_JASON_ANI_JUMP_RIGHT		5

#define MINI_JASON_ANI_DIE				8

#define MINI_JASON_BBOX_WIDTH  8
#define MINI_JASON_BBOX_HEIGHT 16

#define MINI_JASON_UNTOUCHABLE_TIME 5000
#define MINI_JASON_FIRING_DELAY_TIME 500