//-------------------------------------------------------------------------------------
//
// JGE++ is a hardware accelerated 2D game SDK for PSP/Windows.
//
// Licensed under the BSD license, see LICENSE in JGE root for details.
// 
// Copyright (c) 2007 James Hui (a.k.a. Dr.Watson) <jhkhui@gmail.com>
// 
//-------------------------------------------------------------------------------------

#ifndef _JGAME_OBJECT_H_
#define _JGAME_OBJECT_H_

#include "JTypes.h"
#include "JRenderer.h"
#include "JSprite.h"


#define FLASH_TIME			0.10f
#define FLASHING_COUNT		6

#define RENDER_FLAG_ANGLE		0x0001
#define RENDER_FLAG_SIZE		0x0002
#define RENDER_FLAG_ROTATION	0x0004


//////////////////////////////////////////////////////////////////////////
/// A super Sprite class for in-game entities. Extra functions are added
/// in addition to a normal Sprite for easier control of in-game
/// activities.
/// 
//////////////////////////////////////////////////////////////////////////
class JGameObject: public JSprite
{


public:

	//////////////////////////////////////////////////////////////////////////
	/// Constructor.
	/// 
	/// @param tex - Texture for first frame. NULL for no starting frame.
	/// @param x - X position of texture for the frame.
	/// @param y - Y position of texture for the frame.
	/// @param width - Width of the frame.
	/// @param height - Height of the frame.
	/// 
	//////////////////////////////////////////////////////////////////////////
	JGameObject(JTexture *tex, float x, float y, float width, float height);

	virtual ~JGameObject();
	
	//////////////////////////////////////////////////////////////////////////
	/// Update function.
	/// 
	/// @param dt - Delta time since last update (in second).
	/// 
	//////////////////////////////////////////////////////////////////////////
	virtual void Update(float dt);

	//////////////////////////////////////////////////////////////////////////
	/// Render current frame.
	/// 
	//////////////////////////////////////////////////////////////////////////
	virtual void Render();

	//////////////////////////////////////////////////////////////////////////
	/// Function to handle collision.
	/// 
	//////////////////////////////////////////////////////////////////////////
	virtual void OnCollide();

	//////////////////////////////////////////////////////////////////////////
	/// Set bounding box for collision detection. All the following collision
	/// detections will be done using bounding box.
	/// 
	/// @param x - X position of the bounding box (relative to this sprite).
	/// @param y - Y position of the bounding box (relative to this sprite).
	/// @param width - Width of the bounding box.
	/// @param height - Height of the bounding box.
	/// 
	//////////////////////////////////////////////////////////////////////////
	void SetBBox(float x, float y, float width, float height);

	//////////////////////////////////////////////////////////////////////////
	/// Get bounding box relative to screen position.
	/// 
	/// @param x - X of screen position.
	/// @param y - Y of screen position.
	/// 
	/// @return xNow - X position of the bounding box (relative to screen).
	/// @return yNow - Y position of the bounding box (relative to screen).
	/// @return width - Width of the bounding box.
	/// @return height - Height of the bounding box.
	/// 
	//////////////////////////////////////////////////////////////////////////
	void GetBBox(float x, float y, float* xNow, float* yNow, float* width, float *height);

	//////////////////////////////////////////////////////////////////////////
	/// Set up a circle for collision detection. All the following collision
	/// detections will be done using circle to circle collision detection.
	/// 
	/// @param cx - X of the circle center.
	/// @param cy - Y of the circle center.
	/// @param radius - Radius of the circle.
	/// 
	//////////////////////////////////////////////////////////////////////////
	void SetCollisionCircle(float cx, float cy, float radius);

	//////////////////////////////////////////////////////////////////////////
	/// Check for collision. Either bounding box or circle to circle 
	/// collision detection will be used depending on which one is setup last.
	/// 
	/// @param target - Target to check for collision.
	/// 
	//////////////////////////////////////////////////////////////////////////
	bool Collide(JGameObject *target);

	//////////////////////////////////////////////////////////////////////////
	/// Set object that has collided with this object.
	/// 
	/// @param target - Object that has collided with this object.
	//////////////////////////////////////////////////////////////////////////
	void SetCollisionTarget(JGameObject *target);

	//////////////////////////////////////////////////////////////////////////
	/// Get object that has collided with this object.
	/// 
	/// @return Object that has collided with this object.
	//////////////////////////////////////////////////////////////////////////
	JGameObject *GetCollisionTarget();
	
	//////////////////////////////////////////////////////////////////////////
	/// Set damage point of this object.
	/// 
	/// @param pt - Damage point.
	//////////////////////////////////////////////////////////////////////////
	void SetHitPoint(int pt);

	//////////////////////////////////////////////////////////////////////////
	/// Get damage point of this object.
	/// 
	/// @return Damage point.
	//////////////////////////////////////////////////////////////////////////
	int GetHitPoint();

	//////////////////////////////////////////////////////////////////////////
	/// Set blood of this object.
	/// 
	/// @param pt - Blood value.
	//////////////////////////////////////////////////////////////////////////
	void SetBlood(int pt);

	//////////////////////////////////////////////////////////////////////////
	/// Get blood of this object.
	/// 
	/// @return Blood value.
	//////////////////////////////////////////////////////////////////////////
	int GetBlood();
	
	//////////////////////////////////////////////////////////////////////////
	/// Enable alpha animation during update.
	/// 
	/// @param flag - Enable flag.
	/// @param delta - Rate of changing the alpha value.
	/// 
	//////////////////////////////////////////////////////////////////////////
	void EnableAlpha(bool flag, float delta=0.0f);

	//////////////////////////////////////////////////////////////////////////
	/// Enable scaling during update.
	/// 
	/// @param flag - Enable flag.
	/// @param delta - Rate of changing the scaling value.
	/// 
	//////////////////////////////////////////////////////////////////////////
	void EnableScaling(bool flag, float delta=0.0f);

	//////////////////////////////////////////////////////////////////////////
	/// Enable rotation during update.
	/// 
	/// @param flag - Enable flag.
	/// @param delta - Rate of rotation.
	/// 
	//////////////////////////////////////////////////////////////////////////
	void EnableRotation(bool flag, float delta=0.0f);

	//////////////////////////////////////////////////////////////////////////
	/// Set rendering flags.
	/// 
	/// @param flags - Rendering flags encoded in bits.
	/// 
	//////////////////////////////////////////////////////////////////////////
	void SetRenderFlags(int flags);

	//////////////////////////////////////////////////////////////////////////
	/// Start flashing during render.
	/// 
	//////////////////////////////////////////////////////////////////////////
	void StartFlashing();

	//////////////////////////////////////////////////////////////////////////
	/// Start flashing during render.
	/// 
	//////////////////////////////////////////////////////////////////////////
	void StopFlashing();

	//////////////////////////////////////////////////////////////////////////
	/// Check if object is flashing.
	/// 
	/// @return Flashing status.
	/// 
	//////////////////////////////////////////////////////////////////////////
	bool IsFlashing();

	

private:

	u32 mRenderFlags;
	

	bool mUseBoundingBox;

	float mBBoxX;
	float mBBoxY;
	float mBBoxWidth;
	float mBBoxHeight;

	float mCenterX;
	float mCenterY;
	float mRadius;

	bool mDoAlpha;
	float mAlphaDelta;
	bool mDoScaling;
	float mScaleDelta;
	bool mDoRotation;
	float mRotationDelta;

	bool mFlashing;
	float mFlashTimer;
	int mFlashCounter;
	

protected:
	JGameObject *mCollisionTarget;
	bool mCollided;

	int mHitPoint;
	int mBlood;
	int mOriginalBlood;


};


#endif

