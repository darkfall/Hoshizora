#include "SoraImageEffect.h"

#include "CoreTransformer.h"

namespace sora {
		
	SoraImageEffect::~SoraImageEffect() {
		if(bInternalTransformer) {
			if(t_transformer) {
				delete t_transformer;
				t_transformer = 0;
			}
		}
		
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
		startTime = pauseTime = topauseTime = 0.f;
		t_curr = t_src;
		
		if(!t_transformer) {
			if(mode == IMAGE_EFFECT_PINGPONG) {
				t_transformer = new CoreTrigTransformerFull<CoreTransform>;
				effectTime *= 2;
			}
			else
				t_transformer = new CoreLinearTransformer<CoreTransform>;
			bInternalTransformer = true;
		} else bInternalTransformer = false;
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
						break;
					case IMAGE_EFFECT_REPEAT:
					case IMAGE_EFFECT_PINGPONG:
						startTime = 0.f;
						t_curr = t_src;
						break;
				}
			} else
				if(t_transformer) t_curr = t_transformer->slerp(t_src, t_dst, startTime/effectTime);
			
		}
		return states;
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