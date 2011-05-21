#include "SoraImageEffect.h"

#include "CoreTransformer.h"

namespace sora {
		
	SoraImageEffect::SoraImageEffect(): 
		etype(IMAGE_EFFECT_NONE), states(IMAGE_EFFECT_NOTSTART), t_transformer(NULL), pnext(NULL), pprev(NULL),
		currRepeatTimes(0), repeatTimes(0), listMode(IMAGE_EFFECT_NULL) {
	}
	SoraImageEffect::SoraImageEffect(CoreTransformer<CoreTransform>* transformer): 
		t_transformer(transformer), pnext(NULL), pprev(NULL),
		currRepeatTimes(0), repeatTimes(0), listMode(IMAGE_EFFECT_NULL) { 
	}
	SoraImageEffect::SoraImageEffect(IMAGE_EFFECT_MODE _mode): 
		mode(_mode), etype(IMAGE_EFFECT_NONE), states(IMAGE_EFFECT_NOTSTART), t_transformer(NULL), pnext(NULL), pprev(NULL),
		currRepeatTimes(0), repeatTimes(0), listMode(IMAGE_EFFECT_NULL) { 
	}
	
	SoraImageEffect::~SoraImageEffect() {
		if(bInternalTransformer) {
			if(t_transformer) {
				delete t_transformer;
				t_transformer = 0;
			}
		}
	}
	
	void SoraImageEffect::setRepeatTimes(uint32 times) {
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
		listMode = mode;
		if(listMode == IMAGE_EFFECT_PINGPONG)
			listMode = IMAGE_EFFECT_PINGPONG_INVERSE;
		states = IMAGE_EFFECT_PLAYING;
		pingpongState = 0;
		startTime = pauseTime = topauseTime = 0.f;
		t_curr = t_src;
		
		if(!t_transformer) {
			t_transformer = new CoreLinearTransformer<CoreTransform>;
			bInternalTransformer = true;
		} else bInternalTransformer = false;
	}
	
	void SoraImageEffect::restart() {
		started = 1;
		paused = 0;
		states = IMAGE_EFFECT_PLAYING;
		startTime = pauseTime = topauseTime = 0.f;
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
		bInternalTransformer = false;
	}
	
	uint32 SoraImageEffect::update(float32 delta) {
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
						
						checkRepeatTimes();
						break;
						
					case IMAGE_EFFECT_REPEAT:
						startTime = 0.f;
						t_curr = t_src;
						
						checkRepeatTimes();
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
	
	void SoraImageEffect::checkRepeatTimes() {
		// inverse and reverse
		if(repeatTimes != 0) {
			++currRepeatTimes;
			if(currRepeatTimes >= mode == IMAGE_EFFECT_PINGPONG ? repeatTimes*2 : repeatTimes) {
				currRepeatTimes = 0;
				states = IMAGE_EFFECT_END;
				started = 0;
				
				if(isInList())
					checkList();
			}
		} else {
			if(isInList())
				checkList();
		}
	}
	
	SoraImageEffect* SoraImageEffect::getListHead() const {
		const SoraImageEffect* phead = this;
		while(phead->pprev != NULL)
			phead = phead->pprev;
		return (SoraImageEffect*)phead;
	}
	
	SoraImageEffect* SoraImageEffect::getListTail() const {
		const SoraImageEffect* ptail = this;
		while(ptail->pnext != NULL)
			ptail = ptail->pnext;
		return (SoraImageEffect*)ptail;
	}
	
	void SoraImageEffect::checkList() {
		if(pnext != NULL) {
			switch (listMode) {
				case IMAGE_EFFECT_ONCE:
					states = IMAGE_EFFECT_END;
					break;
					
				case IMAGE_EFFECT_REPEAT:
					states = IMAGE_EFFECT_TONEXT;
					break;
					
				case IMAGE_EFFECT_PINGPONG_INVERSE:
					states = IMAGE_EFFECT_TONEXT;
					break;
					
				case IMAGE_EFFECT_PINGPONG_REVERSE:
					states = IMAGE_EFFECT_TOPREV;
					
					if(pprev == NULL) {
						SoraImageEffect* peff = getListHead();
						while(peff != NULL) {
							peff->listMode = IMAGE_EFFECT_PINGPONG_INVERSE;
							peff = peff->pnext;
						}
						listMode = IMAGE_EFFECT_PINGPONG_INVERSE;
						states = IMAGE_EFFECT_TONEXT;
					}
					break;
			}
		} else {
			if(pprev != NULL) {
				if(listMode == IMAGE_EFFECT_PINGPONG_INVERSE) {
					SoraImageEffect* peff = getListHead();
					while(peff != NULL) {
						peff->listMode = IMAGE_EFFECT_PINGPONG_REVERSE;
						peff = peff->pnext;
					}
					listMode = IMAGE_EFFECT_PINGPONG_REVERSE;
					states = IMAGE_EFFECT_TOPREV;
				} else if(listMode == IMAGE_EFFECT_REPEAT)
					states = IMAGE_EFFECT_END_OF_LIST;
			}
		}
		
	}
	
	bool SoraImageEffect::isInList() const {
		return pprev != NULL || pnext != NULL;
	}
	
	IMAGE_EFFECT_MODE SoraImageEffect::getListMode() const {
		return listMode;
	}
	
	void SoraImageEffect::clearList() {
		SoraImageEffect* peff = getListHead();
		while(peff != NULL) {
			SoraImageEffect* eff = peff;
			peff = eff->getNext();
			
			delete eff;
			eff = NULL;
		}
	}
	
	SoraImageEffect* SoraImageEffect::setNext(SoraImageEffect* nextEffect) {
		if(nextEffect == NULL) {
			pnext = NULL;
			pprev = NULL;
		}
			
		SoraImageEffect* tmpNext = pnext;
		if(tmpNext != NULL) {
			while(tmpNext->pnext != NULL) {
				tmpNext = tmpNext->pnext;
			}
			tmpNext->pnext = nextEffect;
			nextEffect->pprev = tmpNext;
		} else {
			pnext = nextEffect;
			pnext->pprev = this;
		}
		nextEffect->listMode = this->listMode;
		if(nextEffect->listMode == IMAGE_EFFECT_PINGPONG)
			nextEffect->listMode = IMAGE_EFFECT_PINGPONG_INVERSE;
		
		return pnext;
	}
	
	SoraImageEffect* SoraImageEffect::getNext() const {
		return pnext;
	}
	
	SoraImageEffect* SoraImageEffect::getPrev() const {
		return pprev;
	}
	
	SoraImageEffectFade::SoraImageEffectFade(float32 src, float32 dst, float32 _time, IMAGE_EFFECT_MODE _mode,
											 CoreTransformer<CoreTransform>* transformer):
	SoraImageEffect(transformer) {
		src<dst?etype = IMAGE_EFFECT_FADEIN:etype = IMAGE_EFFECT_FADEOUT;
		t_src.Set(src);
		t_dst.Set(dst);

		start(_mode, _time);
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

} // namespace sora