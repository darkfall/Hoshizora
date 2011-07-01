#ifndef SORA_IMAGE_EFFECT_H
#define SORA_IMAGE_EFFECT_H

#include "SoraException.h"
#include "SoraPlatform.h"
#include "SoraColor.h"
#include "hgevector.h"
#include "SoraObject.h"

#include "CoreTransform.h"
#include "CoreTransformer.h"

#include "SoraModifier.h"

namespace sora {
	
#define IMAGE_EFFECT_TYPE 0x00001000
	
	class SoraSprite;

	enum {
		IMAGE_EFFECT_NONE = 0,

		IMAGE_EFFECT_FADEIN = 1,
		IMAGE_EFFECT_FADEOUT = 2,

		IMAGE_EFFECT_TRANSITIONS = 4,
		IMAGE_EFFECT_TRANSITIONS_Z = 8,

		IMAGE_EFFECT_SCALEIN = 16,
		IMAGE_EFFECT_SCALEOUT = 32,
		IMAGE_EFFECT_TENSILE = 64,

		IMAGE_EFFECT_SHAKE = 128,
		
		IMAGE_EFFECT_COLOR_TRANSITION = 256,
		
		IMAGE_EFFECT_ROTATE = 512,
		IMAGE_EFFECT_ROTATE_Z = 1024,
	};

	enum {
		IMAGE_EFFECT_END,
		IMAGE_EFFECT_NOTSTART,
		IMAGE_EFFECT_PLAYING,
		IMAGE_EFFECT_PAUSED,
	};

	enum IMAGE_EFFECT_MODE {
		IMAGE_EFFECT_NULL = 0,
		IMAGE_EFFECT_ONCE = 1,
		IMAGE_EFFECT_PINGPONG = 2,
		IMAGE_EFFECT_REPEAT = 3,
		
		// submodes
		IMAGE_EFFECT_PINGPONG_INVERSE = 10,
		IMAGE_EFFECT_PINGPONG_REVERSE = 11,
	};
	
	class SoraImageEffect: public SoraModifier<SoraSprite> {
	public:
		SoraImageEffect();
		SoraImageEffect(CoreTransformer<CoreTransform>* transformer);
		SoraImageEffect(IMAGE_EFFECT_MODE _mode);
		virtual ~SoraImageEffect();

		void stop();

		void pause();
		void pauseForTime(float32 t);
		void resume();
        
        bool finished() const ;

		float32 getTime();

		uint8 getState() const { return states; }
		uint16 getType() const  { return etype; }
		IMAGE_EFFECT_MODE getMode() const { return mode; }

		virtual void start(IMAGE_EFFECT_MODE mode, float32 time);
		virtual uint32 update(float32 delta);
		
		virtual void modify(SoraSprite* sprite) = 0;
		
		void restart();
		
		void setTransformer(CoreTransformer<CoreTransform>* transformer);

		float32 get1st() const { return t_curr.Get1st(); }
		float32 get2nd() const { return t_curr.Get2nd(); }
		float32 get3rd() const { return t_curr.Get3rd(); }
		float32 get4th() const { return t_curr.Get4th(); }
		
		SoraImageEffect& add(SoraImageEffect* next);
        
        float32 getEffectTime() const { return effectTime; }
		
		void setRepeatTimes(int32 times);
		uint32 getRepeatTimes() const;
		
		void swap();
				
	protected:
		void checkRepeatTimes();

		uint16 etype;
		uint8 states;
		IMAGE_EFFECT_MODE mode;

		float32 startTime;
		float32 pauseTime;
		float32 topauseTime;
		float32 effectTime;
		
		int32 currRepeatTimes;
		int32 repeatTimes;
		
		uint8 pingpongState;
		
		uint8 started;
		uint8 paused;

		CoreTransform t_src;
		CoreTransform t_dst;
		CoreTransform t_curr;
		
		CoreTransformer<CoreTransform>* t_transformer;
		bool bInternalTransformer;
	};
	
	class SoraImageEffectList: public SoraImageEffect {
	public:
		SoraImageEffectList();
		SoraImageEffectList(IMAGE_EFFECT_MODE mode);
		virtual ~SoraImageEffectList();
		
		SoraImageEffectList* add(SoraImageEffect* effect);

		SoraImageEffect* getListHead() const;
		SoraImageEffect* getListTail() const;
		
		void setListMode(IMAGE_EFFECT_MODE mode);
		IMAGE_EFFECT_MODE getListMode() const;
		
		virtual uint32 update(float32 delta);
		virtual void start(IMAGE_EFFECT_MODE mode, float32 time);
		
		virtual void modify(SoraSprite* spr);
		
	private:
		typedef std::list<SoraImageEffect*> IMAGE_EFFECT_LIST;
		IMAGE_EFFECT_LIST mImageEffects;
		
		SoraImageEffect* mCurrEffect;
		
		bool mReverse;
		
		IMAGE_EFFECT_MODE mListMode;
	};

	class SoraImageEffectFade: public SoraImageEffect {
	public:
		SoraImageEffectFade(float32 src, float32 dst, float32 time, 
							IMAGE_EFFECT_MODE mode=IMAGE_EFFECT_ONCE,
							CoreTransformer<CoreTransform>* transformer=0);
		
		virtual void modify(SoraSprite* sprite);
	};

	class SoraImageEffectShake: public SoraImageEffect {
	public:
		SoraImageEffectShake(float32 mV, float32 mH, float32 time, 
							 IMAGE_EFFECT_MODE mode=IMAGE_EFFECT_ONCE,
							 CoreTransformer<CoreTransform>* transformer=0);
		
		virtual void modify(SoraSprite* sprite);
	};

	class SoraImageEffectScale: public SoraImageEffect {
	public:
		SoraImageEffectScale(float32 src, float32 dst, float32 time, 
							 IMAGE_EFFECT_MODE mode=IMAGE_EFFECT_ONCE,
							 CoreTransformer<CoreTransform>* transformer=0);
		SoraImageEffectScale(float32 srcV, float32 dstV, float32 srcH, float32 dstH, float32 time, 
							 IMAGE_EFFECT_MODE mode=IMAGE_EFFECT_ONCE,
							 CoreTransformer<CoreTransform>* transformer=0);
		
		virtual void modify(SoraSprite* sprite);
	};

	class SoraImageEffectTransitions: public SoraImageEffect {
	public:
		SoraImageEffectTransitions(float32 sx, float32 sy, float32 sz, float32 dx, float32 dy, float32 dz, float32 time, 
								   IMAGE_EFFECT_MODE mode=IMAGE_EFFECT_ONCE,
								   CoreTransformer<CoreTransform>* transformer=0);
		SoraImageEffectTransitions(float32 sx, float32 sy, float32 dx, float32 dy, float32 time, 
								   IMAGE_EFFECT_MODE mode=IMAGE_EFFECT_ONCE,
								   CoreTransformer<CoreTransform>* transformer=0);
		
		virtual void modify(SoraSprite* sprite);
	};
	
	class SoraImageEffectColorTransitions: public SoraImageEffect {
	public:
		SoraImageEffectColorTransitions(const SoraColorRGBA& start, const SoraColorRGBA& end, float32 time, 
										IMAGE_EFFECT_MODE mode=IMAGE_EFFECT_ONCE,
										CoreTransformer<CoreTransform>* transformer=0);
		SoraImageEffectColorTransitions(ulong32 start, ulong32 end, float32 time, 
										IMAGE_EFFECT_MODE mode=IMAGE_EFFECT_ONCE,
										CoreTransformer<CoreTransform>* transformer=0);
		
		virtual void modify(SoraSprite* sprite);
	};
	
	class SoraImageEffectRotation: public SoraImageEffect {
	public:
		SoraImageEffectRotation(float32 start, float32 end, float32 time,
								IMAGE_EFFECT_MODE mode=IMAGE_EFFECT_ONCE,
								CoreTransformer<CoreTransform>* transformer=0);
		SoraImageEffectRotation(float32 start, float32 startz, float32 end, float32 endz, float32 time,
								IMAGE_EFFECT_MODE mode=IMAGE_EFFECT_ONCE,
								CoreTransformer<CoreTransform>* transformer=0);
		
		virtual void modify(SoraSprite* sprite);
	};
	
	typedef SoraImageEffectFade IEFade;
	typedef SoraImageEffectShake IEShake;
	typedef SoraImageEffectScale IEScale;
	typedef SoraImageEffectTransitions IETransitions;
	typedef SoraImageEffectColorTransitions IEColorTransitions;
	typedef SoraImageEffectRotation IERotation;
	
	typedef CoreTransformer<CoreTransform> IMAGE_EFFECT_TRANSFORMER;

} // namespace sora

#endif