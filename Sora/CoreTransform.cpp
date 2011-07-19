#include "CoreTransform.h"

namespace sora {
    
    CoreTransform::CoreTransform(float32 _f): 
    f1(_f), 
    bClean(false), 
    iKind(TRANSFORM_1F) {
    }
    
    CoreTransform::CoreTransform(float32 _f1, float32 _f2): 
    f1(_f1), 
    f2(_f2), 
    bClean(false), 
    iKind(TRANSFORM_2F) {
    }
    
    CoreTransform::CoreTransform(float32 _f1, float32 _f2, float32 _f3): 
    f1(_f1), 
    f2(_f2), 
    f3(_f3), 
    bClean(false), 
    iKind(TRANSFORM_3F) {
    }
    
    CoreTransform::CoreTransform(float32 _f1, float32 _f2, float32 _f3, float32 _f4):
    f1(_f1), 
    f2(_f2), 
    f3(_f3), 
    f4(_f4), 
    bClean(false), 
    iKind(TRANSFORM_4F) {
    }
	
    CoreTransform::CoreTransform():
    bClean(true), 
    iKind(0) {
    }
    
    void CoreTransform::Set(float32 _f) { 
        f1 = _f; 
        bClean = false;
        iKind = TRANSFORM_1F;
    }
    
    void CoreTransform::Set(float32 _f1, float32 _f2) {
        f1 = _f1; 
        f2 = _f2;
        bClean = false;
        iKind = TRANSFORM_2F;
    }
    
    void CoreTransform::Set(float32 _f1, float32 _f2, float32 _f3) { 
        f1 = _f1;
        f2 = _f2; 
        f3 = _f3;
        bClean = false; 
        iKind = TRANSFORM_3F;
    }
    
    void CoreTransform::Set(float32 _f1, float32 _f2, float32 _f3, float32 _f4) {
        f1 = _f1;
        f2 = _f2;
        f3 = _f3;
        f4 = _f4;
        bClean = false;
        iKind = TRANSFORM_4F; 
    }
	
	CoreTransform CoreTransform::GetDist(const CoreTransform& t2, int divider) {
		if(divider == 0)
			 return *this;
	
		if(bClean) return *this;
		
		if(iKind != t2.GetKind()) {
			//THROW_SORA_EXCEPTION("CoreTransform::GetDist: Different Transform Type");
			return *this;
		}
	
		switch (iKind) {
			case TRANSFORM_1F: return CoreTransform((f1-t2.f1)/divider); break;
			case TRANSFORM_2F: return CoreTransform((f1-t2.f1)/divider, (f2-t2.f2)/divider); break;
			case TRANSFORM_3F: return CoreTransform((f1-t2.f1)/divider, (f2-t2.f2)/divider, (f3-t2.f3)/divider); break;
			case TRANSFORM_4F: return CoreTransform((f1-t2.f1)/divider, (f2-t2.f2)/divider, (f3-t2.f3)/divider, (f4-t2.f4)/divider); break;
		}
		return *this;
	}

	CoreTransform CoreTransform::operator / (float32 divider) const {
		if(bClean) return *this;
	
		switch (iKind) {
			case TRANSFORM_1F: return CoreTransform(f1/divider); break;
			case TRANSFORM_2F: return CoreTransform(f1/divider, f2/divider); break;
			case TRANSFORM_3F: return CoreTransform(f1/divider, f2/divider, f3/divider); break;
			case TRANSFORM_4F: return CoreTransform(f1/divider, f2/divider, f3/divider, f4/divider); break;
		}
		return *this;
	}

	CoreTransform CoreTransform::operator + (const CoreTransform& rhs) const {
		if(bClean) return rhs;
		
		if(iKind != rhs.GetKind()) {
			//THROW_SORA_EXCEPTION("CoreTransform::Operator +: Different Transform Type");
			return *this;
		}
	
		switch (iKind) {
			case TRANSFORM_1F: return CoreTransform(f1+rhs.f1); break;
			case TRANSFORM_2F: return CoreTransform(f1+rhs.f1, f2+rhs.f2); break;
			case TRANSFORM_3F: return CoreTransform(f1+rhs.f1, f2+rhs.f2, f3+rhs.f3); break;
			case TRANSFORM_4F: return CoreTransform(f1+rhs.f1, f2+rhs.f2, f3+rhs.f3, f4+rhs.f4); break;
		}
		return *this;
	}

	
	CoreTransform CoreTransform::operator - (const CoreTransform& rhs) const {
		if(bClean) return rhs;
		
		if(iKind != rhs.GetKind()) {
			//THROW_SORA_EXCEPTION("CoreTransform::Operator +: Different Transform Type");
			return *this;
		}
	
		switch (iKind) {
			case TRANSFORM_1F: return CoreTransform(f1-rhs.f1); break;
			case TRANSFORM_2F: return CoreTransform(f1-rhs.f1, f2-rhs.f2); break;
			case TRANSFORM_3F: return CoreTransform(f1-rhs.f1, f2-rhs.f2, f3-rhs.f3); break;
			case TRANSFORM_4F: return CoreTransform(f1-rhs.f1, f2-rhs.f2, f3-rhs.f3, f4-rhs.f4); break;
		}
		return *this;
	}	

	CoreTransform CoreTransform::operator * (float32 m) const {
		if(bClean) return *this;
	
		switch (iKind) {
			case TRANSFORM_1F: return CoreTransform(f1*m); break;
			case TRANSFORM_2F: return CoreTransform(f1*m, f2*m); break;
			case TRANSFORM_3F: return CoreTransform(f1*m, f2*m, f3*m); break;
			case TRANSFORM_4F: return CoreTransform(f1*m, f2*m, f3*m, f4*m); break;
		}
		return *this;
	}

	void CoreTransform::operator = (const CoreTransform& rhs) {
		iKind = rhs.iKind;
		bClean = rhs.bClean;
	
		switch (iKind) {
			case TRANSFORM_1F: f1 = rhs.f1; break;
			case TRANSFORM_2F: f1 = rhs.f1; f2 = rhs.f2; break;
			case TRANSFORM_3F: f1 = rhs.f1; f2 = rhs.f2; f3 = rhs.f3; break;
			case TRANSFORM_4F: f1 = rhs.f1; f2 = rhs.f2; f3 = rhs.f3; f4 = rhs.f4; break;
		}
	}

	CoreTransform& CoreTransform::operator += (const CoreTransform& rhs) {
		if(bClean) return *this;
	
		if(iKind != rhs.GetKind()) {
			//THROW_SORA_EXCEPTION("CoreTransform::Operator +: Different Transform Type");
			return *this;
		}
	
		switch (iKind) {
			case TRANSFORM_1F: f1 += rhs.f1; break;
			case TRANSFORM_2F: f1 += rhs.f1; f2 += rhs.f2; break;
			case TRANSFORM_3F: f1 += rhs.f1; f2 += rhs.f2; f3 += rhs.f3; break;
			case TRANSFORM_4F: f1 += rhs.f1; f2 += rhs.f2; f3 += rhs.f3; f4 += rhs.f4; break;
		}
		return *this;
	}
	
	CoreTransform& CoreTransform::operator -= (const CoreTransform& rhs) {
		if(bClean) return *this;
		
		if(iKind != rhs.GetKind()) {
			//THROW_SORA_EXCEPTION("CoreTransform::Operator +: Different Transform Type");
			return *this;
		}
		
		switch (iKind) {
			case TRANSFORM_1F: f1 -= rhs.f1; break;
			case TRANSFORM_2F: f1 -= rhs.f1; f2 -= rhs.f2; break;
			case TRANSFORM_3F: f1 -= rhs.f1; f2 -= rhs.f2; f3 -= rhs.f3; break;
			case TRANSFORM_4F: f1 -= rhs.f1; f2 -= rhs.f2; f3 -= rhs.f3; f4 -= rhs.f4; break;
		}
		return *this;
	}

	float32 CoreTransform::Get1st() const {
		if(bClean) return 0.f;
		
		return f1;
	}

	float32 CoreTransform::Get2nd() const {
		if(bClean) return 0.f;
	
		if(iKind >= TRANSFORM_2F) return f2;
		return 0.f;
	}

	float32 CoreTransform::Get3rd() const {
		if(bClean) return 0.f;
	
		if(iKind >= TRANSFORM_3F) return f3;
		return 0.f;
	}

	float32 CoreTransform::Get4th() const {
		if(bClean) return 0.f;
	
		if(iKind >= TRANSFORM_4F) return f4;
		return 0.f;
	}

	bool CoreTransform::operator == (const CoreTransform& rhs) const {
		if(bClean) return false;;
	
		if(iKind != rhs.GetKind()) return false;
	
		switch (iKind) {
			case TRANSFORM_1F: if(f1 != rhs.f1) return false; break;
			case TRANSFORM_2F: if(f1 != rhs.f1 || f2 != rhs.f2) return false; break;
			case TRANSFORM_3F: if(f1 != rhs.f1 || f2 != rhs.f2 || f3 != rhs.f3) return false; break;
			case TRANSFORM_4F: if(f1 != rhs.f1 || f2 != rhs.f2 || f3 != rhs.f3 || f4 != rhs.f4) return false; break;
		}
		return true;
	}

} // namespace sora