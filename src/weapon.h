#pragma once

struct Bullet
{
	typedef enum BulletType {
		BULLET_STREAM, BULLET_SPHERE,
		BULLET_SPLASH,
	} BulletType;
	
	float radius; // hitbox radius
	float splashRadius;
	float chipRadius;
	
	float range; // how far shots travel before gravity takes effect
	float speed; // units per frame
	float gravity; // change per frame in downward velocity
	float gravityMax; // maximum downward velocity
	
	float damage; // damage dealt on direct hit
	float splashDamage;
	float chipDamage;
	
	float paintRadius; // radius of painted area
	int rainFrames; // frames per rain drop
	
	float distanceTravelled;
	int rainFramesElapsed;
	
	vec3 position;
	vec3 direction; // must be normalized!
	int falling = false;
	
	// determines how the bullet is rendered
	BulletType type;
};

//
// Main weapon
//

struct MainWeapon
{
	int type; // weapon class
	int weight; // weight class
	
	int delayFrames; // frames between trigger press and action
	int releaseFrames; // frames between trigger release and action
	int bufferFrames; // frames between trigger release and tank refill
	
	float cost; // tank units consumed per shot fired
	
	float spreadAngle; // maximum horizontal angle of deviation of shots
	int spreadInterval; // interval after which deviation multiplies
	float spreadFactor; // multiplier of deviation per interval
	
	Bullet bullet;
	
	int fps;
};

struct MainShooter: public MainWeapon
{
	int shotFrames; // frames per shot
};

struct MainSemi: MainWeapon
{
	int shotFrames; // frames per shot
	int fireFrames; // frames of continuous shots
	int cycleFrames; // frames between each fire
};

struct MainGatling: MainWeapon
{
	int shotFrames; // frames per shot
	int fireFrames; // frames of continuous shots
	
	int chargeFrames; // base charge
	int superFrames; // extra charge
};

struct MainBlaster: MainWeapon
{
	int shotFrames; // frames per shot
	
	float splashDamage;
	float splashRadius;
	float chipDamage;
	float chipRadius;
};

struct MainCharger: MainWeapon
{
	int hasScope;
	int canStore;
	
	int chargeFrames; // total frames of charge
	int storeFrames; // frames of held charge
	
	float scopeFov; // zoom value in fov (degrees)
};

struct MainBrush: MainWeapon
{
	int shotCt;
	
	float paintWidth;
	float rollSpeedMultiplier;
};

struct MainRoller: MainWeapon
{
	int flickShotCt;
	int flingShotCt;
	
	float flingSpread;
	float flingRange;
	
	float paintWidth;
	float rollSpeedMultiplier;
};

struct MainBucket: MainWeapon
{
	int shotCt;
};

/* __INGAME__ */

struct Weapon
{
	int triggerHeld;
	
	int delayF;
	int releaseF;
	int bufferF;
	
	float tank;
	
	Array<Bullet>* bullets;
	
	void shoot(Bullet bullet); // most bullets
	void swing(); // brush melee, roller melee, etc
	
	virtual void update() {}
	virtual void trigger() {}
	virtual void release() {}
};

void Weapon::shoot(Bullet bullet)
{
	bullets->append(bullet);
}

struct Shooter: Weapon
{
	MainShooter stat;
	int isFiring;
	int shotF;

void trigger()
{
	if (!isFiring)
	{
		delayF = delayFrames;
		shotF = 0;
	}
	triggerHeld = true;
}

void release()
{
	releaseF = releaseFrames;	
	triggerHeld = false;
}

void update()
{
	if (triggerHeld)
	{
		if (delayF) { delayF--; return; }
		isFiring = true;
		
		if (shotF) { shotF--; return; }
		
		shoot(bullet);
		
		shotF = shotFrames;
	}
	else
	{
		if (releaseF) { releaseF--; return; }
		isFiring = false;
		
		if (bufferF) { bufferF--; return; }
	}
}

};