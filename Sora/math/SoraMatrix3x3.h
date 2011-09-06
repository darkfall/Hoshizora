/*
 *  SoraMatrix3x3.h
 *  Sora
 *
 *  Created by GriffinBu on 1/20/11.
 *  Copyright 2011 Griffin Bu(Project L). All rights reserved.
 *
 */

#ifndef SORA_2D_MATRIX_H_
#define SORA_2D_MATRIX_H_

#include "SoraPlatform.h"
#include "SoraVector.h"

#include <vector>

namespace sora {
	
	/*
	 just another 2d matrix implemention
	 */
	
	class SORA_API SoraMatrix3x3 {
	private:
		struct _2DMatrix {
			double _11, _12, _13;
			double _21, _22, _23;
			double _31, _32, _33;
		};
		
		_2DMatrix matrix;
		
		inline void matrixMul(_2DMatrix& rhs);
		
	public:
		SoraMatrix3x3() {
			createIdentity();
		}
		
		inline void createIdentity();
		inline void translate(double x, double y);
		inline void scale(double x, double y);
		inline void rotate(double rotation);
		inline void rotate(const SoraVector& fwd, const SoraVector& side);
		inline void transformVector2Ds(std::vector<SoraVector>& vPoints);
		inline void transformVector2D(SoraVector& vPoint);
		
		void _11(double val) { matrix._11 = val; }
		void _12(double val) { matrix._12 = val; }
		void _13(double val) { matrix._13 = val; }
		void _21(double val) { matrix._21 = val; }
		void _22(double val) { matrix._22 = val; }
		void _23(double val) { matrix._23 = val; }
		void _31(double val) { matrix._31 = val; }
		void _32(double val) { matrix._32 = val; }
		void _33(double val) { matrix._33 = val; }
		
		double _11() const { return matrix._11; }
		double _12() const { return matrix._12; }
		double _13() const { return matrix._13; }
		double _21() const { return matrix._21; }
		double _22() const { return matrix._22; }
		double _23() const { return matrix._23; }
		double _31() const { return matrix._31; }
		double _32() const { return matrix._32; }
		double _33() const { return matrix._33; }
	};
	
	inline void SoraMatrix3x3::matrixMul(_2DMatrix& rhs) {
		SoraMatrix3x3::_2DMatrix mat_tmp;
		
		mat_tmp._11 = (matrix._11*rhs._11) + (matrix._12*rhs._21) + (matrix._13*rhs._31);
		mat_tmp._12 = (matrix._11*rhs._12) + (matrix._12*rhs._22) + (matrix._13*rhs._32);
		mat_tmp._13 = (matrix._11*rhs._13) + (matrix._12*rhs._22) + (matrix._13*rhs._33);
		
		mat_tmp._21 = (matrix._21*rhs._11) + (matrix._22*rhs._21) + (matrix._23*rhs._31);
		mat_tmp._22 = (matrix._21*rhs._12) + (matrix._22*rhs._22) + (matrix._23*rhs._32);
		mat_tmp._23 = (matrix._21*rhs._13) + (matrix._22*rhs._22) + (matrix._23*rhs._33);
		
		mat_tmp._31 = (matrix._31*rhs._11) + (matrix._32*rhs._21) + (matrix._33*rhs._31);
		mat_tmp._32 = (matrix._31*rhs._12) + (matrix._32*rhs._22) + (matrix._33*rhs._32);
		mat_tmp._33 = (matrix._31*rhs._13) + (matrix._32*rhs._22) + (matrix._33*rhs._33);
		
		matrix = mat_tmp;
	}
	
	inline void SoraMatrix3x3::transformVector2Ds(std::vector<SoraVector>& vPoints) {
		for(uint32 i=0; i<vPoints.size(); ++i) {
			double tempX = (matrix._11*vPoints[i].x) + (matrix._21*vPoints[i].y) + matrix._31;
			double tempY = (matrix._12*vPoints[i].x) + (matrix._22*vPoints[i].y) + matrix._32;
			
			vPoints[i].x = (float)tempX;
			vPoints[i].y = (float)tempY;
		}
	}
	
	inline void SoraMatrix3x3::transformVector2D(SoraVector& vPoint) {
		double tempX = (matrix._11*vPoint.x) + (matrix._21*vPoint.y) + matrix._31;
		double tempY = (matrix._12*vPoint.x) + (matrix._22*vPoint.y) + matrix._32;
		
		vPoint.x = (float)tempX;
		vPoint.y = (float)tempY;
	}
	
	inline void SoraMatrix3x3::createIdentity() {
		matrix._11 = 1; matrix._12 = 0; matrix._13 = 0;
		matrix._21 = 0; matrix._22 = 1; matrix._23 = 0;
		matrix._31 = 0; matrix._33 = 0; matrix._33 = 1;
	}
	
	inline void SoraMatrix3x3::translate(double x, double y) {
		SoraMatrix3x3::_2DMatrix mat;
		
		mat._11 = 1; mat._12 = 0; mat._13 = 0;
		mat._21 = 0; mat._22 = 1; mat._23 = 0;
		mat._31 = x; mat._33 = y; mat._33 = 1;
		
		matrixMul(mat);
	}
	
	inline void SoraMatrix3x3::scale(double xscale, double yscale) {
		SoraMatrix3x3::_2DMatrix mat;
		
		mat._11 = xscale; mat._12 = 0; mat._13 = 0;
		mat._21 = 0; mat._22 = yscale; mat._23 = 0;
		mat._31 = 0; mat._33 = 0; mat._33 = 1;
		
		matrixMul(mat);
	}
	
	inline void SoraMatrix3x3::rotate(double rot) {
		SoraMatrix3x3::_2DMatrix mat;
		
		double sinv = sin(rot);
		double cosv = cos(rot);
		
		mat._11 = cosv; mat._12 = sinv; mat._13 = 0;
		mat._21 = -sinv; mat._22 = cosv; mat._23 = 0;
		mat._31 = 0; mat._33 = 0; mat._33 = 1;
		
		matrixMul(mat);
	}
	
	inline void SoraMatrix3x3::rotate(const SoraVector& fwd, const SoraVector& side) {
		SoraMatrix3x3::_2DMatrix mat;
		
		mat._11 = fwd.x; mat._12 = fwd.y; mat._13 = 0;
		mat._21 = side.x; mat._22 = side.y; mat._23 = 0;
		mat._31 = 0; mat._33 = 0; mat._33 = 1;
		
		matrixMul(mat);
	}
    
    typedef SoraMatrix3x3 Matrix3x3;
    
} // namepsace sora

#endif // SORA_2D_MATRIX_H_