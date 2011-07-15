#include "SoraImageEffect.h"

#include "CoreTransformer.h"
#include "SoraSprite.h"

namespace sora {
		
	SoraImageEffect::SoraImageEffect(): 
		etype(IMAGE_EFFECT_NONE), states(IMAGE_EFFECT_NOTSTART), t_transformer(NULL),
		currRepeatTimes(0), repeatTimes(0) {
	}
	SoraImageEffect::SoraImageEffect(CoreTransformer<CoreTransform>* transformer): 
		currRepeatTimes(0), repeatTimes(0) { 
            if(transformer) {
                t_transformer = transformer->clone();
            } else 
                t_transformer = NULL;
	}
	SoraImageEffect::SoraImageEffect(IMAGE_EFFECT_MODE _mode): 
		mode(_mode), etype(IMAGE_EFFECT_NONE), states(IMAGE_EFFECT_NOTSTART), t_transformer(NULL), 
		currRepeatTimes(0), repeatTimes(-1) { 
	}
	
	SoraImageEffect::~SoraImageEffect() {
		if(t_transformer) {
            delete t_transformer;
            t_transformer = 0;
        }
	}
	
	void SoraImageEffect::setRepeatTimes(int32 times) {
		repeatTimes = times;
		currRepeatTimes = 0;
	}
	
	uint32 SoraImageEffect::getRepeatTimes() const {
		return repeatTimes;
	}
    
    bool SoraImageEffect::finished() const {
        return (states == IMAGE_EFFECT_END || states == IMAGE_EFFECT_NOTSTART);
    }
	
	void SoraImageEffect::start(IMAGE_EFFECT_MODE _mode, float32 time) {
		effectTime = time;
		if(effectTime == 0.f) {
			states = IMAGE_EFFECT_END;
			t_curr = t_dst;
			return;
		}
		started = 1;
		paused = 0;
		mode = _mode;
	
		states = IMAGE_EFFECT_PLAYING;
		pingpongState = 0;
		startTime = pauseTime = topauseTime = 0.f;
		t_curr = t_src;
		
		if(!t_transformer) {
			t_transformer = new CoreLinearTransformer<CoreTransform>;
		}
	}
	
	void SoraImageEffect::restart() {
		started = 1;
		paused = 0;
		states = IMAGE_EFFECT_PLAYING;
		startTime = pauseTime = topauseTime = 0.f;
		t_curr = t_src;
	}
	
	void SoraImageEffect::swap() {
		std::swap(t_src, t_dst);
		t_curr = t_src;
	}
	
	void SoraImageEffect::stop() {
		started = 0;
		states = IMAGE_EFFECT_END;
	}
	
	void SoraImageEffect::pause() {
		paused = 1;
		pauseTime = 0.f;
		states = IMAGE_EFFECT_PAUSED;
	}
	
	void SoraImageEffect::pauseForTime(float32 t) {
		assert(t != 0.f);
		pause();
		topauseTime = t;
	}
	
	void SoraImageEffect::resume() {
		paused = 0;
		topauseTime = 0.f;
		states = IMAGE_EFFECT_PLAYING;
	}
	
	float32 SoraImageEffect::getTime() {
		if(started == 1) {
			if(paused == 1) {
				return pauseTime;
			}
			return startTime;
		}
		return 0.f;
	}
	
	void SoraImageEffect::setTransformer(CoreTransformer<CoreTransform>* transformer) {
		assert(transformer != 0);
		if(t_transformer) delete t_transformer;
		t_transformer = transformer;
	}
	
	int32 SoraImageEffect::update(float32 delta) {
		if(started == 1) {
			if(paused == 1) {
				pauseTime += delta;
				if(topauseTime != 0.f) 
					if(pauseTime > topauseTime)
						resume();
				
				return states;
			}
			
			startTime += delta;
			if(startTime >= effectTime) {
				t_curr = t_dst;
				switch(mode) {
					case IMAGE_EFFECT_ONCE:
						states = IMAGE_EFFECT_END;
						t_curr = t_dst;
						break;
						
					case IMAGE_EFFECT_REPEAT:
						checkRepeatTimes();
						
						startTime = 0.f;
						t_curr = t_src;
						break;
						
					case IMAGE_EFFECT_PINGPONG:
						startTime = 0.f;
						std::swap(t_src, t_dst);
						t_curr = t_src;
						++pingpongState;
						
						if(pingpongState == 2) {
							checkRepeatTimes();
							pingpongState = 0;
						}
						break;
				}
			} else
				if(t_transformer) t_curr = t_transformer->slerp(t_src, t_dst, startTime/effectTime);
			
		}
		return states;
	}
	
	bool SoraImageEffect::checkRepeatTimes() {
		if(repeatTimes == 0)
            return false;
        else if(repeatTimes != 0 ) {
			++currRepeatTimes;
			if(currRepeatTimes >= repeatTimes) {
				currRepeatTimes = 0;
				states = IMAGE_EFFECT_END;
				started = 0;
                return true;
			}
		}
		return false;
	}
	
	SoraImageEffectList::SoraImageEffectList(IMAGE_EFFECT_MODE mode) {
		mListMode = mode;
		mCurrEffect = NULL;
		mReverse = false;
		
		started = 1;
		paused = 0;
		states = IMAGE_EFFECT_PLAYING;
	}
        
    SoraImageEffect& SoraImageEffect::operator =(const SoraImageEffect& rhs) {
        if(&rhs != this) {
            etype = rhs.etype;
            states = rhs.states;
            mode = rhs.mode;
            startTime = rhs.startTime;
            pauseTime = rhs.pauseTime;
            topauseTime = rhs.topauseTime;
            effectTime = rhs.effectTime;
            currRepeatTimes = rhs.currRepeatTimes;
            repeatTimes = rhs.repeatTimes;
            pingpongState = rhs.pingpongState;
            started = rhs.started;
            paused = rhs.paused;
            t_src = rhs.t_src;
            t_dst = rhs.t_dst;
            t_curr = rhs.t_curr;
            t_transformer = rhs.t_transformer->clone();
        }
		return *this;
    }
											 
	SoraImageEffectList::SoraImageEffectList() {
		mListMode = IMAGE_EFFECT_ONCE;
		mCurrEffect = NULL;
		mReverse = false;
		
		started = 1;
		paused = 0;
		states = IMAGE_EFFECT_PLAYING;
	}
	
	SoraImageEffectList::~SoraImageEffectList() {
		ImageEffectList::iterator itEffect = mImageEffects.begin();
		while(itEffect != mImageEffects.end()) {
			delete *itEffect;
			*itEffect = NULL;
			++itEffect;
		}
	}
	
	SoraImageEffectList* SoraImageEffectList::add(SoraImageEffect* effect) {
		mImageEffects.push_back(effect);
		effect->start(IMAGE_EFFECT_ONCE, effect->getEffectTime());
		mCurrEffect	= getListHead();
		return this;
	}
	
	SoraImageEffect* SoraImageEffectList::getListHead() const {
		return mImageEffects.front();
	}
	
	SoraImageEffect* SoraImageEffectList::getListTail() const {
		return mImageEffects.back();
	}
	
	void SoraImageEffectList::setListMode(IMAGE_EFFECT_MODE mode) {
		mListMode = mode;
	}
	
	IMAGE_EFFECT_MODE SoraImageEffectList::getListMode() const {
		return mListMode;
	}
	
	void SoraImageEffectList::start(IMAGE_EFFECT_MODE mode, float32 time) {
		mCurrEffect	= getListHead();
	}
	
	void SoraImageEffectList::modify(SoraSprite* spr) {
		if(mCurrEffect)
			mCurrEffect->modify(spr);
	}
    
    SoraModifier<SoraSprite>* SoraImageEffectList::clone() {
        SoraImageEffectList* newlist = new SoraImageEffectList(*this);
        newlist->mImageEffects = this->mImageEffects;
        newlist->mReverse = this->mReverse;
        newlist->mListMode = this->mListMode;
        newlist->mCurrEffect = this->mCurrEffect;
        return newlist;
    }

	int32 SoraImageEffectList::update(float32 delta) {
		if(mCurrEffect) {
			int32 result = mCurrEffect->update(delta);
			if(result == IMAGE_EFFECT_END) {
				ImageEffectList::iterator next = std::find(mImageEffects.begin(), mImageEffects.end(), mCurrEffect);
				if(next != mImageEffects.end()) {
					if(!mReverse) {
						++next; 
						if(next != mImageEffects.end()) {
							mCurrEffect = *next;
							mCurrEffect->start(IMAGE_EFFECT_ONCE, mCurrEffect->getEffectTime());
						}
						else {
							if(mListMode == IMAGE_EFFECT_PINGPONG) {
								mReverse = true;
								mCurrEffect->swap();
								mCurrEffect->start(IMAGE_EFFECT_ONCE, mCurrEffect->getEffectTime());
								
								return IMAGE_EFFECT_PLAYING;
							} else if(mListMode == IMAGE_EFFECT_REPEAT) {
								mCurrEffect = getListHead();
								mCurrEffect->start(IMAGE_EFFECT_ONCE, mCurrEffect->getEffectTime());
								
								return IMAGE_EFFECT_PLAYING;
							} else
								return IMAGE_EFFECT_END;
						}
					} else {
						if(next == mImageEffects.begin()) {
							if(mListMode == IMAGE_EFFECT_PINGPONG) {
								mReverse = false;
								mCurrEffect->swap();
								mCurrEffect->start(IMAGE_EFFECT_ONCE, mCurrEffect->getEffectTime());
								
								return IMAGE_EFFECT_PLAYING;
							} else
								return IMAGE_EFFECT_END;
						}
						
						--next;
						mCurrEffect->swap();
						mCurrEffect = *next;
						mCurrEffect->swap();
						mCurrEffect->start(IMAGE_EFFECT_ONCE, mCurrEffect->getEffectTime());
					}
				}
			}
			return IMAGE_EFFECT_PLAYING;
		}
		return IMAGE_EFFECT_END;
	}
	
	SoraImageEffectFade::SoraImageEffectFade(float32 src, float32 dst, float32 _time, IMAGE_EFFECT_MODE _mode,
											 CoreTransformer<CoreTransform>* transformer):
	SoraImageEffect(transformer) {
		src<dst?etype = IMAGE_EFFECT_FADEIN:etype = IMAGE_EFFECT_FADEOUT;
		t_src.Set(src);
		t_dst.Set(dst);

		start(_mode, _time);
	}
	
	void SoraImageEffectFade::modify(SoraSprite* spr) {
		spr->setColor(CSETA(spr->getColor(), get1st()*255));
	}
    
    SoraModifier<SoraSprite>* SoraImageEffectFade::clone() {
        return new SoraImageEffectFade(*this);
    }

	SoraImageEffectScale::SoraImageEffectScale(float32 src, float32 dst, float32 _time, IMAGE_EFFECT_MODE _mode,
											   CoreTransformer<CoreTransform>* transformer):
	SoraImageEffect(transformer) {
		src<dst?etype = IMAGE_EFFECT_SCALEIN:etype = IMAGE_EFFECT_SCALEOUT;
		t_src.Set(src, src);
		t_dst.Set(dst, dst);

		start(_mode, _time);
	}
	
	SoraImageEffectScale::SoraImageEffectScale(float32 srcV, float32 dstV, float32 srcH, float32 dstH, float32 _time, IMAGE_EFFECT_MODE _mode,
											   CoreTransformer<CoreTransform>* transformer):
	SoraImageEffect(transformer) {
		etype = IMAGE_EFFECT_TENSILE;
		t_src.Set(srcV, srcH);
		t_dst.Set(dstV, dstH);
		
		start(_mode, _time);
	}
	
	void SoraImageEffectScale::modify(SoraSprite* spr) {
        spr->setScale(get1st(), get2nd());
	}
    
    SoraModifier<SoraSprite>* SoraImageEffectScale::clone() {
        return new SoraImageEffectScale(*this);
    }

	SoraImageEffectTransitions::SoraImageEffectTransitions(float32 sx, float32 sy, float32 sz, float32 dx, float32 dy, float32 dz, float32 _time, IMAGE_EFFECT_MODE _mode,
														   CoreTransformer<CoreTransform>* transformer):
	SoraImageEffect(transformer) {
		etype = IMAGE_EFFECT_TRANSITIONS_Z;
		t_src.Set(sx, sy, sz);
		t_dst.Set(dx, dy, dz);

		start(_mode, _time);
	}

	SoraImageEffectTransitions::SoraImageEffectTransitions(float32 sx, float32 sy, float32 dx, float32 dy, float32 _time, IMAGE_EFFECT_MODE _mode,
														   CoreTransformer<CoreTransform>* transformer):
	SoraImageEffect(transformer) {
		etype = IMAGE_EFFECT_TRANSITIONS;
		t_src.Set(sx, sy, 0.f);
		t_dst.Set(dx, dy, 0.f);

		start(_mode, _time);
	}
    
    SoraModifier<SoraSprite>* SoraImageEffectTransitions::clone() {
        return new SoraImageEffectTransitions(*this);
    }
	
	void SoraImageEffectTransitions::modify(SoraSprite* spr) {
        spr->setPosition(get1st(), get2nd());
	}
	
	SoraImageEffectColorTransitions::SoraImageEffectColorTransitions(const SoraColorRGBA& s, const SoraColorRGBA& end, float32 _time, IMAGE_EFFECT_MODE _mode,
																	 CoreTransformer<CoreTransform>* transformer):
	SoraImageEffect(transformer) {
		etype = IMAGE_EFFECT_COLOR_TRANSITION;
		t_src.Set(s.r, s.g, s.b, s.a);
		t_dst.Set(end.r, end.g, end.b, end.a);
		
		start(_mode, _time);
	}
	
	SoraImageEffectColorTransitions::SoraImageEffectColorTransitions(ulong32 s, ulong32 end, float32 _time, IMAGE_EFFECT_MODE _mode,
																	 CoreTransformer<CoreTransform>* transformer):
	SoraImageEffect(transformer) {
		etype = IMAGE_EFFECT_COLOR_TRANSITION;
		t_src.Set((float)CGETR(s)/255.f, (float)CGETG(s)/255.f, (float)CGETB(s)/255.f, (float)CGETA(s)/255.f);
		t_dst.Set((float)CGETR(end)/255.f, (float)CGETG(end)/255.f, (float)CGETB(end)/255.f, (float)CGETA(end)/255.f);
		
		start(_mode, _time);
	}
	
	void SoraImageEffectColorTransitions::modify(SoraSprite* spr) {
		spr->setColor(SoraColorRGBA::GetHWColor(get1st(), get2nd(), get3rd(), get4th()), -1);
	}
    
    SoraModifier<SoraSprite>* SoraImageEffectColorTransitions::clone() {
        return new SoraImageEffectColorTransitions(*this);
    }
	
	SoraImageEffectRotation::SoraImageEffectRotation(float32 s, float32 e, float32 _time, IMAGE_EFFECT_MODE _mode,
													 CoreTransformer<CoreTransform>* transformer):
	SoraImageEffect(transformer) {
		etype = IMAGE_EFFECT_ROTATE;
		t_src.Set(s);
		t_dst.Set(e);
		
		start(_mode, _time);
	}
	
	SoraImageEffectRotation::SoraImageEffectRotation(float32 s, float32 sz, float32 e, float32 ez, float32 _time, IMAGE_EFFECT_MODE _mode,
													 CoreTransformer<CoreTransform>* transformer):
	SoraImageEffect(transformer) {
		etype = IMAGE_EFFECT_ROTATE_Z;
		t_src.Set(s, sz);
		t_dst.Set(e, ez);
		
		start(_mode, _time);
	}
	
	void SoraImageEffectRotation::modify(SoraSprite* spr) {
		spr->setRotation(get1st());
	}
    
    SoraModifier<SoraSprite>* SoraImageEffectRotation::clone() {
        return new SoraImageEffectRotation(*this);
    }

} // namespace sora