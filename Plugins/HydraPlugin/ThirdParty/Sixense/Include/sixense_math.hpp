/*
 *
 * SIXENSE CONFIDENTIAL
 *
 * Copyright (C) 2011 Sixense Entertainment Inc.
 * All Rights Reserved
 *
 */

#ifndef SIXENSE_MATH_HPP
#define SIXENSE_MATH_HPP

#include <math.h>
#include <iostream>
#include <string>

namespace sixenseMath {

	class Vector2 {
	public:
		Vector2();
		Vector2( const float x, const float y );
		float& operator [](const int idx);
		Vector2 operator -(const Vector2 rhs);
		Vector2 operator +(const Vector2 rhs);

		Vector2& operator +=(const Vector2& rhs);
		Vector2& operator *=(const float& rhs);
		Vector2& operator -=(const Vector2& rhs);
		float operator *(const Vector2 rhs); // dot product
		Vector2 operator *(const float rhs);
		Vector2 operator /(const float rhs);

		bool operator ==(const Vector2& rhs);

		void normalize();
		float length();
		void print( const std::string name=std::string() );
		void fill( float vec[2] );

	protected:
		float _vec[2];
	};

	class Vector3 {
	public:
		Vector3();
		Vector3( const Vector3& );
		Vector3( const float vec[3] );
		Vector3( const float x, const float y, const float z );
		float& operator [](const int idx);
		Vector3 operator -(const Vector3 rhs);
		Vector3 operator +(const Vector3 rhs);
		Vector3 operator ^(const Vector3 rhs); // cross product
		Vector3& operator +=(const Vector3& rhs);
		Vector3& operator *=(const float& rhs);
		Vector3& operator -=(const Vector3& rhs);
		float operator *(const Vector3 rhs); // dot product
		Vector3 operator *(const float rhs);
		Vector3 operator /(const float rhs);
		void normalize();
		float length();
		void print( const std::string name=std::string() );
		void fill( float vec[3] );

		bool operator ==(const Vector3& rhs);

		// Construction helpers
		static Vector3 normalize( const Vector3 );

	protected:
		float _vec[3];
	};

	class Vector4 {
	public:
		Vector4();
		Vector4( const Vector4& );
		Vector4( const Vector3&, float w );
		Vector4( const float vec[4] );
		Vector4( const float x, const float y, const float z, const float w );
		float& operator [](const int idx);
		Vector4 operator -(const Vector4 rhs) const;
		Vector4 operator ^(const Vector4 rhs) const; // cross product
		Vector4 operator +(const Vector4 rhs) const;
		float operator *(const Vector4 rhs); // dot product
		Vector4 operator *(const float rhs) const;
		Vector4 operator -(const float rhs) const;
		Vector4 operator /(const float rhs) const;

		bool operator ==(const Vector4& rhs);

		void normalize();
		float length();
		void print( const std::string name=std::string() );
		Vector4 operator *(const class Matrix4 rhs) const;
		void fill( float vec[4] );

	protected:
		float _vec[4];
	};

	class Quat : public Vector4 {
	public:
		Quat();
		Quat( const float x, const float y, const float z, const float w );
		Quat( const class Matrix3& );
		Quat( const Vector4& );
		Quat( const Vector3 xyz, float w );
		void print( const std::string name=std::string() );
		Vector3 operator *(const Vector3 rhs) const;
		Quat operator *(const Quat rhs) const;
		float dot(const Quat rhs) const; // dot product (operator * is already used for quat...)
		void invert(); // invert in place
		Quat inverse(); // leave this alone and return inverted copy
		Vector3 getEulerAngles();

		// Construction helpers
		static Quat rotation( const Vector3 from_vec, const Vector3 to_vec );
		static Quat rotation( const float angle_in_rad, const Vector3 axis );
		static Quat rotation( const Vector3 hpr_in_rad );
		static Quat rotation( const float heading, const float pitch, const float roll );
		static Quat slerp( const float t, const Quat a, const Quat b);
	};

	class Matrix3 {
	public:
		Matrix3();
		Matrix3( const Matrix3& );
		Matrix3( const float mat[3][3] );
		Matrix3( const float m00, const float m10, const float m20, const float m01, const float m11, const float m21, const float m02, const float m12, const float m22 );
		void fill( float mat[3][3] );
		Matrix3( const Vector3 col0, const Vector3 col1, const Vector3 col2 );
		Vector3& operator [](const int idx);
		Matrix3 operator *(const Matrix3 rhs);
		Matrix3 operator *(const float rhs);
		Matrix3 operator /(const float rhs);
		Matrix3 operator +(const Matrix3 rhs);
		Matrix3 operator -(const Matrix3 rhs);
		Matrix3 operator *(const Quat rhs);
		Vector3 operator *(const Vector3 rhs);
		Vector3 col( const int );
		Vector3 row( const int );
		void set_col( const int which, const Vector3 col );
		float trace();
		bool is_identity();
		void transpose();
		void print( const std::string name=std::string() );
		Vector3 getEulerAngles();

		// Construction helpers
		static Matrix3 rotation( const float angle_in_rad, const Vector3 axis );
		static Matrix3 rotation( const Vector3 hpr_in_rad );
		static Matrix3 rotation( const Quat rot );
		static Matrix3 rotation( const Vector3 from, const Vector3 to );
		static Matrix3 translation( const Vector3 trans );
		static Matrix3 scale( const float, const float, const float );
		static Matrix3 scale( const float );
		static Matrix3 transpose( const Matrix3 );

	protected:
		Vector3 _cols[3];
	};

	class Matrix4 {
	public:
		Matrix4();
		Matrix4( const Matrix4& );
		Matrix4( const Matrix3& );
		Matrix4( const float mat[4][4] );
		Matrix4( const float m00, const float m10, const float m20, const float m30, const float m01, const float m11, const float m21, const float m31, const float m02, const float m12, const float m22, const float m32, const float m03, const float m13, const float m23, const float m33 );
		void fill( float mat[4][4] );
		Matrix4( const Vector4 col0, const Vector4 col1, const Vector4 col2, const Vector4 col3 );
		Vector4& operator [](const int idx);
		Matrix4 operator *(const Matrix4 rhs);
		Matrix4 operator *(const float rhs);
		Matrix4 operator /(const float rhs);
		Matrix4 operator +(const Matrix4 rhs);
		Matrix4 operator -(const Matrix4 rhs);
		Matrix4 operator *(const Quat rhs);
		Vector4 operator *(const Vector4 rhs);
		Vector4 col( const int );
		Vector4 row( const int );
		void set_col( const int which, const Vector4 col );
		float trace();
		bool is_identity();
		void transpose();
		void print( const std::string name=std::string() );
		Vector3 getEulerAngles();

		// Construction helpers
		static Matrix4 rotation( const float angle_in_rad, const Vector3 axis );
		static Matrix4 rotation( const Quat rot );
		static Matrix4 rotation( const Vector3 from, const Vector3 to );
		static Matrix4 rotation( const Vector3 hpr_in_rad );
		static Matrix4 translation( const Vector3 trans );
		static Matrix4 scale( const float, const float, const float );
		static Matrix4 scale( const float );
		static Matrix4 transpose( const Matrix4 );

	protected:
		Vector4 _cols[4];
	};

	class Line {
		friend class Plane;
		public:
			Line( const Line& );
			Line( const Vector3& dir, const Vector3& pos ); 

			Vector3 getClosestPoint( const Vector3& );
			
		private:
			Vector3 _dir;
			Vector3 _pos1;
			Vector3 _pos2;
	};


	class Plane	{
	public:
		Plane(); 
		Plane( const Plane&	);
		Plane( Vector3 p0, Vector3 p1, Vector3 p2 );
		Plane( Vector3 point, Vector3 normal );

		void init();
		double whichSide( Vector3 p );
		Vector3 getClosestPoint( Vector3 in );
		Vector3 intersect( const Line line );
		Vector3 getNormal();

	private:
		double _a, _b, _c, _d;
		Vector3	_norm;
		Vector3	_p0, _p1, _p2;

	};




	#include "sixense_math.cpp"

}


#endif
