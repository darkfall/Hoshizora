//
//  SoraiOSTimer.h
//  Sora-iPhone
//
//  Created by Ruiwei Bu on 9/8/11.
//  Copyright 2011 Studio GameMaster. All rights reserved.
//

#ifndef Sora_iPhone_SoraiOSTimer_h
#define Sora_iPhone_SoraiOSTimer_h

/**
 * Since we don't control the frame rate by ourself
 * The timer only records the duration time between animation presents
 * Use the AnimationInterval in your ViewController to control the framerate
 **/
#include "SoraTimer.h"
#include "SoraTimestamp.h"


namespace sora {
    
    class SoraiOSTimer: public SoraTimer {
    public:
        SoraiOSTimer():
        mDt(0.f),
        mFPS(0.f),
        mRTime(0.f),
        mRFPS(60),
        mTFPS(0) {
            mTimestamp.update();
        }
        
		void setFPS(int32 fps) {
            //
        }
        
		float getFPS() {
            return mRFPS;
        }
	
        float getDelta() {
            return mDt;
        }
		
		bool update() {
            SoraTimestamp::TimeDiff diff = mTimestamp.elapsed();
            
            mDt = (float)diff / SoraTimestamp::Resolution();
            if(mDt != 0.f) {
                mFPS = 1.f / mDt;
            }
            
            mRTime += mDt;
            mTFPS++;
            if(mRTime >= 1.0f) {
                mRFPS = mTFPS;
                mTFPS = 0;
                mRTime = 0.f;
            }
            
            mTimestamp.update();
			return true;
		}
        
    private:
        SoraTimestamp mTimestamp;

        float mDt;
        float mRTime;
        float mFPS;
        int32 mTFPS;
        int32 mRFPS;
    };
    
    
} // namespace sora



#endif
