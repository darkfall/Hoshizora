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
        mTime(0.f),
        mFPS(0.f),
        mFrameCount(0),
        mRTime(0.f),
        mRFPS(60),
        mTFPS(0) {
            mTimestamp.update();
        }
        
		void setFPS(int32 fps) {
            //
        }
        
		float32 getFPS() {
            return mRFPS;
        }
	
        float32 getDelta() {
            return mDt;
        }
		
        float32 getTime() {
            return mTime;
        }
        
		int32 getFrameCount() {
            return mFrameCount;
        }
		
        uint64 getCurrentSystemTime() {
            return SoraTimestamp::currentTime();
        }
		
		bool update() {
            SoraTimestamp::TimeDiff diff = mTimestamp.elapsed();
            
            mDt = (float)diff / SoraTimestamp::resolution();
            if(mDt != 0.f) {
                mFPS = 1.f / mDt;
            }
            mTime += mDt;
            ++mFrameCount;
            
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
        float mTime;
        float mRTime;
        float mFPS;
        int32 mTFPS;
        int32 mRFPS;
        int32 mFrameCount;
    };
    
    
} // namespace sora



#endif
