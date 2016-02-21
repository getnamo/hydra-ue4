/*
 *
 * SIXENSE CONFIDENTIAL
 *
 * Copyright (C) 2011 Sixense Entertainment Inc.
 * All Rights Reserved
 *
 */


// Vector2

inline Vector2::Vector2() { 
	_vec[0] = 0.0f; 
	_vec[1] = 0.0f; 
}

inline Vector2::Vector2( float x, float y ) { 
	_vec[0] = x; 
	_vec[1] = y; 
}

inline float& Vector2::operator [](const int idx) {
	return _vec[idx]; 
}

inline Vector2 Vector2::operator -(const Vector2 rhs) {
	Vector2 rhs_const_copy(rhs);
	return Vector2( _vec[0] - rhs_const_copy[0], _vec[1] - rhs_const_copy[1] );
}

inline Vector2 Vector2::operator +(const Vector2 rhs) {
	Vector2 rhs_const_copy(rhs);
	return Vector2( _vec[0] + rhs_const_copy[0], _vec[1] + rhs_const_copy[1] );
}

inline Vector2& Vector2::operator +=(const Vector2& rhs) {
	Vector2 rhs_const_copy( rhs );
	_vec[0] += rhs_const_copy[0];
	_vec[1] += rhs_const_copy[1];
	return *this;
}

inline Vector2& Vector2::operator -=(const Vector2& rhs) {
	Vector2 rhs_const_copy( rhs );
	_vec[0] -= rhs_const_copy[0];
	_vec[1] -= rhs_const_copy[1];
	return *this;
}

inline float Vector2::operator *(const Vector2 rhs) {
	Vector2 rhs_const_copy( rhs );
	return _vec[0]*rhs_const_copy[0] + _vec[1]*rhs_const_copy[1];
}

inline Vector2 Vector2::operator *(const float rhs) {
	float rhs_const_copy( rhs );
	return Vector2( _vec[0]*rhs_const_copy, _vec[1]*rhs_const_copy );
}

inline Vector2& Vector2::operator *=(const float& rhs) {
	float rhs_const_copy( rhs );
	_vec[0]*=rhs_const_copy;
	_vec[1]*=rhs_const_copy;
	return *this;
}

inline Vector2 Vector2::operator /(const float rhs) {
	float rhs_const_copy( rhs );
	return Vector2( _vec[0]/rhs_const_copy, _vec[1]/rhs_const_copy);
}




inline bool Vector2::operator ==(const Vector2& rhs) {
	Vector2 rhs_const_copy( rhs );
	if( _vec[0] == rhs_const_copy._vec[0] && _vec[1] == rhs_const_copy._vec[1] ) return true;
	else return false;
}



inline void Vector2::normalize() {
	float len = sqrtf( _vec[0]*_vec[0] + _vec[1]*_vec[1] );
	_vec[0]/=len;
	_vec[1]/=len;
}

inline float Vector2::length() {
	return sqrtf( _vec[0]*_vec[0] + _vec[1]*_vec[1] );
}

inline void Vector2::print( std::string name ) {
	if( name != "" ) {
		std::cout << "Vector2 " << name << " = { ";
	} else {
		std::cout << "Vector2 = { ";
	}

	std::cout << _vec[0] << ", " << _vec[1] << " }" << std::endl << std::endl;

}

inline void Vector2::fill( float vec[2] ) {
	vec[0] = _vec[0];
	vec[1] = _vec[1];
}


// Vector3

inline Vector3::Vector3() { 
	_vec[0] = 0.0f; 
	_vec[1] = 0.0f; 
	_vec[2] = 0.0f; 

}

inline Vector3::Vector3( const Vector3& rhs ) {
	_vec[0] = rhs._vec[0]; 
	_vec[1] = rhs._vec[1]; 
	_vec[2] = rhs._vec[2]; 

}


inline Vector3::Vector3( float x, float y, float z ) { 
	_vec[0] = x; 
	_vec[1] = y; 
	_vec[2] = z; 

}

inline Vector3::Vector3( const float vec[3] ) {
	_vec[0] = vec[0]; 
	_vec[1] = vec[1]; 
	_vec[2] = vec[2]; 

}

inline float& Vector3::operator [](const int idx) { 
	return _vec[idx]; 
}

inline Vector3 Vector3::operator -(const Vector3 rhs) {
	Vector3 rhs_const_copy( rhs );
	return Vector3( _vec[0] - rhs_const_copy[0], _vec[1] - rhs_const_copy[1], _vec[2] - rhs_const_copy[2] );
}

inline Vector3 Vector3::operator +(const Vector3 rhs) {
	Vector3 rhs_const_copy( rhs );
	return Vector3( _vec[0] + rhs_const_copy[0], _vec[1] + rhs_const_copy[1], _vec[2] + rhs_const_copy[2] );
}

inline Vector3& Vector3::operator +=(const Vector3& rhs) {
	Vector3 rhs_const_copy( rhs );
	_vec[0] += rhs_const_copy[0];
	_vec[1] += rhs_const_copy[1];
	_vec[2] += rhs_const_copy[2];
	return *this;
}

inline Vector3& Vector3::operator -=(const Vector3& rhs) {
	Vector3 rhs_const_copy( rhs );
	_vec[0] -= rhs_const_copy[0];
	_vec[1] -= rhs_const_copy[1];
	_vec[2] -= rhs_const_copy[2];
	return *this;
}

inline Vector3 Vector3::operator ^(const Vector3 rhs) {
	Vector3 rhs_const_copy( rhs );
	return Vector3( _vec[1]*rhs_const_copy[2]-_vec[2]*rhs_const_copy[1], _vec[2]*rhs_const_copy[0]-_vec[0]*rhs_const_copy[2], _vec[0]*rhs_const_copy[1]-_vec[1]*rhs_const_copy[0] );
}


inline float Vector3::operator *(const Vector3 rhs) {
	Vector3 rhs_const_copy( rhs );
	return _vec[0]*rhs_const_copy[0] + _vec[1]*rhs_const_copy[1] + _vec[2]*rhs_const_copy[2];
}

inline Vector3 Vector3::operator *(const float rhs) {
	float rhs_const_copy( rhs );
	return Vector3( _vec[0]*rhs_const_copy, _vec[1]*rhs_const_copy, _vec[2]*rhs_const_copy);
}

inline Vector3& Vector3::operator *=(const float& rhs) {
	float rhs_const_copy( rhs );
	_vec[0]*=rhs_const_copy;
	_vec[1]*=rhs_const_copy;
	_vec[2]*=rhs_const_copy;
	return *this;
}

inline Vector3 Vector3::operator /(const float rhs) {
	float rhs_const_copy( rhs );
	return Vector3( _vec[0]/rhs_const_copy, _vec[1]/rhs_const_copy, _vec[2]/rhs_const_copy);
}


inline bool Vector3::operator ==(const Vector3& rhs) {
	Vector3 rhs_const_copy( rhs );
	if( _vec[0] == rhs_const_copy._vec[0] && _vec[1] == rhs_const_copy._vec[1] && _vec[2] == rhs_const_copy._vec[2] ) return true;
	else return false;
}





inline void Vector3::normalize() {
	float len = length();
	_vec[0]/=len;
	_vec[1]/=len;
	_vec[2]/=len;
}


inline float Vector3::length() {
	return sqrt( _vec[0]*_vec[0] + _vec[1]*_vec[1] + _vec[2]*_vec[2] );
}

inline void Vector3::print( std::string name ) {
	if( name != "" ) {
		std::cout << "Vector3 " << name << " = { ";
	} else {
		std::cout << "Vector3 = { ";
	}

	std::cout << _vec[0] << ", " << _vec[1] << ", " << _vec[2] << " }" << std::endl << std::endl;

}

inline Vector3 Vector3::normalize( Vector3 m ) {
	Vector3 norm(m);
	norm.normalize();
	return norm;
}

inline void Vector3::fill( float vec[3] ) {
	vec[0] = _vec[0];
	vec[1] = _vec[1];
	vec[2] = _vec[2];
}



// Vector4

inline Vector4::Vector4() { 
	_vec[0] = 0.0f; 
	_vec[1] = 0.0f; 
	_vec[2] = 0.0f; 
	_vec[3] = 1.0f;

}

inline Vector4::Vector4( const Vector4& rhs ) {
	_vec[0] = rhs._vec[0]; 
	_vec[1] = rhs._vec[1]; 
	_vec[2] = rhs._vec[2]; 
	_vec[3] = rhs._vec[3];

}

inline Vector4::Vector4( const Vector3& rhs, float w ) {
	Vector3 rhs_c(rhs);
	_vec[0] = rhs_c[0]; 
	_vec[1] = rhs_c[1]; 
	_vec[2] = rhs_c[2]; 
	_vec[3] = w;
}


inline Vector4::Vector4( const float vec[4] ) {
	_vec[0] = vec[0]; 
	_vec[1] = vec[1]; 
	_vec[2] = vec[2]; 
	_vec[3] = vec[3]; 

}

inline Vector4::Vector4( const float a, const float b, const float c, const float d ) {
	_vec[0] = a; 
	_vec[1] = b; 
	_vec[2] = c; 
	_vec[3] = d;

}

inline float& Vector4::operator [](const int idx) { 
	return _vec[idx]; 
}

inline Vector4 Vector4::operator +(const Vector4 rhs) const {
	Vector4 rhs_const_copy( rhs );
	return Vector4( _vec[0] + rhs_const_copy[0], _vec[1] + rhs_const_copy[1], _vec[2] + rhs_const_copy[2], _vec[3] + rhs_const_copy[3] );
}

inline Vector4 Vector4::operator -(const Vector4 rhs) const {
	Vector4 rhs_const_copy( rhs );
	return Vector4( _vec[0] - rhs_const_copy[0], _vec[1] - rhs_const_copy[1], _vec[2] - rhs_const_copy[2], _vec[3] - rhs_const_copy[3] );
}

inline Vector4 Vector4::operator *(const float rhs) const {
	float rhs_const_copy( rhs );
	return Vector4( _vec[0]*rhs_const_copy, _vec[1]*rhs_const_copy, _vec[2]*rhs_const_copy, _vec[3]*rhs_const_copy);
}

inline Vector4 Vector4::operator /(const float rhs) const {
	float rhs_const_copy( rhs );
	return Vector4( _vec[0]/rhs_const_copy, _vec[1]/rhs_const_copy, _vec[2]/rhs_const_copy, _vec[3]/rhs_const_copy);
}


inline Vector4 Vector4::operator -(const float rhs) const {
	float rhs_const_copy( rhs );
	return Vector4( _vec[0]-rhs_const_copy, _vec[1]-rhs_const_copy, _vec[2]-rhs_const_copy, _vec[3]-rhs_const_copy);
}

inline float Vector4::operator *(const Vector4 rhs) { // dot product
	Vector4 rhs_const_copy( rhs );

	float dot = _vec[0]*rhs_const_copy[0] + _vec[1]*rhs_const_copy[1] + _vec[2]*rhs_const_copy[2] + _vec[3]*rhs_const_copy[3];

	if( dot > 1.0f ) dot = 1.0f;
	if( dot < -1.0f ) dot = -1.0f;
	return dot;
}


inline Vector4 Vector4::operator *(const class Matrix4 rhs_in) const {
	Matrix4 rhs(rhs_in);
	return Vector4( rhs[0][0]*_vec[0] + rhs[0][1]*_vec[1] + rhs[0][2]*_vec[2] + rhs[0][3]*_vec[3],
		              rhs[1][0]*_vec[0] + rhs[1][1]*_vec[1] + rhs[1][2]*_vec[2] + rhs[1][3]*_vec[3], 
		              rhs[2][0]*_vec[0] + rhs[2][1]*_vec[1] + rhs[2][2]*_vec[2] + rhs[2][3]*_vec[3],
		              rhs[3][0]*_vec[0] + rhs[3][1]*_vec[1] + rhs[3][2]*_vec[2] + rhs[3][3]*_vec[3] );
}

inline bool Vector4::operator ==(const Vector4& rhs) {
	Vector4 rhs_const_copy( rhs );
	if( 
		_vec[0] == rhs_const_copy._vec[0] && 
		_vec[1] == rhs_const_copy._vec[1] && 
		_vec[2] == rhs_const_copy._vec[2] && 
		_vec[3] == rhs_const_copy._vec[3] ) return true;
	else return false;
}






inline void Vector4::normalize() {
	float len = length();
	_vec[0]/=len;
	_vec[1]/=len;
	_vec[2]/=len;
	_vec[3]/=len;
}


inline float Vector4::length() {
	return sqrt( _vec[0]*_vec[0] + _vec[1]*_vec[1] + _vec[2]*_vec[2] + _vec[3]*_vec[3] );
}

inline void Vector4::print( std::string name ) {
	if( name != "" ) {
		std::cout << "Vector4 " << name << " = { ";
	} else {
		std::cout << "Vector4 = { ";
	}

	std::cout << _vec[0] << ", " << _vec[1] << ", " << _vec[2] << ", " << _vec[3] << " }" << std::endl << std::endl;

}

inline void Vector4::fill( float vec[4] ) {
	vec[0] = _vec[0];
	vec[1] = _vec[1];
	vec[2] = _vec[2];
	vec[3] = _vec[3];
}




// Quat

inline Quat::Quat() { 
	_vec[0] = 0.0f; 
	_vec[1] = 0.0f; 
	_vec[2] = 0.0f; 
	_vec[3] = 1.0f;

}

inline Quat::Quat( const float a, const float b, const float c, const float d ) {
	_vec[0] = a; 
	_vec[1] = b; 
	_vec[2] = c; 
	_vec[3] = d;

}

inline Quat::Quat( const Vector4& rhs ) : Vector4( rhs ) {
	
}

inline Quat::Quat( const Vector3 xyz, float w ) {
	Vector3 deconst( xyz );
	_vec[0] = deconst[0]; 
	_vec[1] = deconst[1]; 
	_vec[2] = deconst[2];
	_vec[3] = w; 
	
}

inline Quat::Quat( const Matrix3& mat_c ) {
	
	Matrix3 mat(mat_c);

	float T = 1.0f + mat[0][0] + mat[1][1] + mat [2][2];

	float S, X, Y, Z, W;

#if 1
	if( T > 0.000001f ) {
		S = sqrt( T ) * 2.0f;
		X = ( mat[1][2] - mat[2][1] ) / S;
		Y = ( mat[2][0] - mat[0][2] ) / S;
		Z = ( mat[0][1] - mat[1][0] ) / S;
		W = 0.25f * S;
	} else {
		if ( mat[0][0] > mat[1][1] && mat[0][0] > mat[2][2] )  {	// Column 0: 
			S  = sqrt( 1.0f + mat[0][0] - mat[1][1] - mat[2][2] ) * 2.0f;
			X = 0.25f * S;
			Y = (mat[0][1] + mat[1][0] ) / S;
			Z = (mat[2][0] + mat[0][2] ) / S;
			W = (mat[1][2] - mat[2][1] ) / S;
		} else if ( mat[1][1] > mat[2][2] ) {			// Column 1: 
			S  = sqrt( 1.0f + mat[1][1] - mat[0][0] - mat[2][2] ) * 2.0f;
			X = (mat[0][1] + mat[1][0] ) / S;
			Y = 0.25f * S;
			Z = (mat[1][2] + mat[2][1] ) / S;
			W = (mat[2][0] - mat[0][2] ) / S;
		} else {						// Column 2:
			S  = sqrt( 1.0f + mat[2][2] - mat[0][0] - mat[1][1] ) * 2.0f;
			X = (mat[2][0] + mat[0][2] ) / S;
			Y = (mat[1][2] + mat[2][1] ) / S;
			Z = 0.25f * S;
			W = (mat[0][1] - mat[1][0] ) / S;
		}
	}
#else
	if( T > 0.000001f ) {
		S = sqrt( T ) * 2;
		X = ( mat[2][1] - mat[1][2] ) / S;
		Y = ( mat[0][2] - mat[2][0] ) / S;
		Z = ( mat[1][0] - mat[0][1] ) / S;
		W = 0.25 * S;
	} else {
		if ( mat[0][0] > mat[1][1] && mat[0][0] > mat[2][2] )  {	// Column 0: 
			S  = sqrt( 1.0 + mat[0][0] - mat[1][1] - mat[2][2] ) * 2;
			X = 0.25 * S;
			Y = (mat[1][0] + mat[0][1] ) / S;
			Z = (mat[0][2] + mat[2][0] ) / S;
			W = (mat[2][1] - mat[1][2] ) / S;
		} else if ( mat[1][1] > mat[2][2] ) {			// Column 1: 
			S  = sqrt( 1.0 + mat[1][1] - mat[0][0] - mat[2][2] ) * 2;
			X = (mat[1][0] + mat[0][1] ) / S;
			Y = 0.25 * S;
			Z = (mat[2][1] + mat[1][2] ) / S;
			W = (mat[0][2] - mat[2][0] ) / S;
		} else {						// Column 2:
			S  = sqrt( 1.0 + mat[2][2] - mat[0][0] - mat[1][1] ) * 2;
			X = (mat[0][2] + mat[2][0] ) / S;
			Y = (mat[2][1] + mat[1][2] ) / S;
			Z = 0.25 * S;
			W = (mat[1][0] - mat[0][1] ) / S;
		}
	}
#endif

	_vec[0] = X;
	_vec[1] = Y;
	_vec[2] = Z;
	_vec[3] = W;

}

inline Vector3 Quat::operator *(const Vector3 rhs) const {
	return Matrix3::rotation( *this ) * rhs;
}

inline float Quat::dot(const Quat rhs) const {
	Vector4 t(*this), that(rhs);

	float dot = t * that;

	return dot;
}

inline Quat Quat::operator *(const Quat rhs) const {
	Quat a(rhs), b(*this);

	Vector3 vec_a(a[0],a[1],a[2]);
	Vector3 vec_b(b[0],b[1],b[2]);
	float scalar_a(a[3]), scalar_b(b[3]);

	Quat retval( vec_b*scalar_a + vec_a*scalar_b + (vec_a^vec_b), scalar_a*scalar_b - vec_a*vec_b );
	retval.normalize();
	return retval;

}


inline Quat Quat::rotation( const float angle_in_rad, const Vector3 axis_in ) {

	// from http://www.j3d.org/matrix_faq/matrfaq_latest.html
	Vector3 axis( axis_in );
	axis.normalize();

	float c = cos( angle_in_rad/2.0f );
	float s = sin( angle_in_rad/2.0f );

	Quat result( axis[0] * s, axis[1] * s, axis[2] * s, c );
	result.normalize();

	return result;
}

inline Quat Quat::rotation( const float heading_in_rad, const float pitch_in_rad, const float roll_in_rad ) {

	Matrix3 mat = 
		Matrix3::rotation( heading_in_rad, Vector3( 0, 1, 0 ) ) *
		Matrix3::rotation( pitch_in_rad, Vector3( 1, 0, 0 ) ) *
		Matrix3::rotation( roll_in_rad, Vector3( 0, 0, -1 ) );
	return Quat(mat);
	
}

inline Quat Quat::rotation( const Vector3 hpr_in_rad ) {
	Vector3 deconst_hpr_in_rad( hpr_in_rad );
	return Quat::rotation( deconst_hpr_in_rad[0], deconst_hpr_in_rad[1], deconst_hpr_in_rad[2] );

}


inline Quat Quat::rotation( const Vector3 from_vec_in, const Vector3 to_vec_in ) {

	Vector3 to_vec(to_vec_in);
	Vector3 from_vec(from_vec_in);

	from_vec.normalize();
	to_vec.normalize();

	Vector3 axis = from_vec ^ to_vec;
	float dot = from_vec * to_vec;

	float angle = acosf( dot );

	return rotation( angle, axis );
}

inline void Quat::print( const std::string name ) {
	if( name != "" ) {
		std::cout << "Quat " << name << " = { ";
	} else {
		std::cout << "Quat = { ";
	}

	std::cout << _vec[0] << ", " << _vec[1] << ", " << _vec[2] << ", " << _vec[3] << " }" << std::endl << std::endl;

}

inline void Quat::invert() { // invert in place
	float norm = _vec[0]*_vec[0] + _vec[1]*_vec[1] + _vec[2]*_vec[2] + _vec[3]*_vec[3];
	_vec[0] *= -1.0f/norm;
	_vec[1] *= -1.0f/norm;
	_vec[2] *= -1.0f/norm;
	_vec[2] *= 1.0f/norm;
}

inline Quat Quat::inverse() { // leave this alone and return inverted copy
	Quat copy( *this );

	copy.invert();

	return copy;
}



inline Quat Quat::slerp( const float t, const Quat a, const Quat b) {


	// Cant just dot quats, compiler doesnt see Vector4::operator* for some reason...
	Quat nca(a), ncb(b);

	Vector4 va(nca[0], nca[1], nca[2], nca[3]);
	Vector4 vb(ncb[0], ncb[1], ncb[2], ncb[3]);


	// via google via Graphics Gems

	double beta;
	double theta;
	double sin_t, cos_t;
	double alpha = t;
	bool bflip=false;

	cos_t = va*vb;

	if( cos_t < 0.0 ) {
		cos_t = -cos_t;
		bflip = true;
	}

	if( 1.0 - cos_t < 1e-7 ) {
		beta = 1.0 - alpha;
	} else {
		theta = acos( cos_t );
		sin_t = sin( theta );
		beta = sin( theta * (1.0 - alpha)) / sin_t;
		alpha = sin( theta * alpha ) / sin_t;
	}

	if( bflip ) {
		alpha = -alpha;
	}

	Quat retval( va*(float)beta + vb*(float)alpha );

	retval.normalize();

	return retval;
}


inline Vector3 Quat::getEulerAngles() {
	
	const Quat const_this(*this);
	Matrix3 mat = Matrix3::rotation( const_this );

	return mat.getEulerAngles();
}



// Matrix3

inline Matrix3::Matrix3() {
	_cols[0] = Vector3( 1, 0 ,0 );
	_cols[1] = Vector3( 0, 1 ,0 );
	_cols[2] = Vector3( 0, 0 ,1 );
}


inline Matrix3::Matrix3( const float mat[3][3] ) {
	_cols[0] = Vector3( mat[0][0], mat[0][1], mat[0][2] );
	_cols[1] = Vector3( mat[1][0], mat[1][1], mat[1][2] );
	_cols[2] = Vector3( mat[2][0], mat[2][1], mat[2][2] );

}

inline Matrix3::Matrix3( const Matrix3& rhs ) {
	_cols[0] = rhs._cols[0];
	_cols[1] = rhs._cols[1];
	_cols[2] = rhs._cols[2];

}

inline Matrix3::Matrix3( const float m00, const float m10, const float m20, const float m01, const float m11, const float m21, const float m02, const float m12, const float m22 ) {
	
	_cols[0] = Vector3( m00, m01, m02 );
	_cols[1] = Vector3( m10, m11, m12 );
	_cols[2] = Vector3( m20, m21, m22 );

}


inline void Matrix3::fill( float mat[3][3] ) {
	for( unsigned int i=0; i<3; i++ ) {
		for( int j=0; j<3; j++ ) {
			mat[i][j] = _cols[i][j];
		}
	}
}


inline Matrix3::Matrix3( const Vector3 col0, const Vector3 col1, const Vector3 col2 ) {
	_cols[0] = col0;
	_cols[1] = col1;
	_cols[2] = col2;

}


inline Vector3& Matrix3::operator [](const int idx) {
	return _cols[idx]; 
}


inline Matrix3 Matrix3::operator *(const Matrix3 rhs_in) {
	Matrix3 rhs(rhs_in);
	Matrix3 retmat( 
		Vector3( rhs[0][0]*_cols[0][0] + rhs[1][0]*_cols[0][1] + rhs[2][0]*_cols[0][2],
		rhs[0][1]*_cols[0][0] + rhs[1][1]*_cols[0][1] + rhs[2][1]*_cols[0][2], 
		rhs[0][2]*_cols[0][0] + rhs[1][2]*_cols[0][1] + rhs[2][2]*_cols[0][2] ),

		Vector3( rhs[0][0]*_cols[1][0] + rhs[1][0]*_cols[1][1] + rhs[2][0]*_cols[1][2],
		rhs[0][1]*_cols[1][0] + rhs[1][1]*_cols[1][1] + rhs[2][1]*_cols[1][2], 
		rhs[0][2]*_cols[1][0] + rhs[1][2]*_cols[1][1] + rhs[2][2]*_cols[1][2] ),

		Vector3( rhs[0][0]*_cols[2][0] + rhs[1][0]*_cols[2][1] + rhs[2][0]*_cols[2][2],
		rhs[0][1]*_cols[2][0] + rhs[1][1]*_cols[2][1] + rhs[2][1]*_cols[2][2], 
		rhs[0][2]*_cols[2][0] + rhs[1][2]*_cols[2][1] + rhs[2][2]*_cols[2][2] ) );

	return retmat;
}

inline Matrix3 Matrix3::operator *(const Quat rhs) {
	return *this * Matrix3::rotation( rhs );
}


inline Vector3 Matrix3::operator *(const Vector3 rhs_in) {
	Vector3 rhs(rhs_in);
	return Vector3( _cols[0][0]*rhs[0] + _cols[1][0]*rhs[1] + _cols[2][0]*rhs[2],
		_cols[0][1]*rhs[0] + _cols[1][1]*rhs[1] + _cols[2][1]*rhs[2], 
		_cols[0][2]*rhs[0] + _cols[1][2]*rhs[1] + _cols[2][2]*rhs[2] );
}

inline Matrix3 Matrix3::operator *(const float rhs) {
	return Matrix3( _cols[0] * rhs, _cols[1] * rhs, _cols[2] * rhs );
}

inline Matrix3 Matrix3::operator /(const float rhs) {
	return Matrix3( _cols[0] / rhs, _cols[1] / rhs, _cols[2] / rhs );
}

inline Matrix3 Matrix3:: operator +(const Matrix3 rhs) {
	return Matrix3( _cols[0] + rhs._cols[0], _cols[1] + rhs._cols[1], _cols[2] + rhs._cols[2] );

}

inline Matrix3 Matrix3:: operator -(const Matrix3 rhs) {
	return Matrix3( _cols[0] - rhs._cols[0], _cols[1] - rhs._cols[1], _cols[2] - rhs._cols[2] );

}


inline void Matrix3::transpose() {
	Matrix3 mat( 
		Vector3( _cols[0][0], _cols[1][0], _cols[2][0] ),
		Vector3( _cols[0][1], _cols[1][1], _cols[2][1] ),
		Vector3( _cols[0][2], _cols[1][2], _cols[2][2] )
		);

	_cols[0] = mat._cols[0];
	_cols[1] = mat._cols[1];
	_cols[2] = mat._cols[2];
}

inline float Matrix3::trace() {
	return( _cols[0][0] + _cols[1][1] + _cols[2][2] );
}

inline Vector3 Matrix3::col( const int i ) {
	return _cols[i];
}

inline Vector3 Matrix3::row( const int i ) {
	return Vector3( _cols[0][i], _cols[1][i], _cols[2][i] );
}

inline void Matrix3::set_col( const int which, const Vector3 col ) {
	_cols[which] = col;
}

inline bool Matrix3::is_identity() {
	if( _cols[0][0] == 1.0f &&
		  _cols[0][1] == 0.0f &&
			_cols[0][2] == 0.0f &&
			_cols[1][0] == 0.0f &&
			_cols[1][1] == 1.0f &&
			_cols[1][2] == 0.0f &&
			_cols[2][0] == 0.0f &&
			_cols[2][1] == 0.0f &&
			_cols[2][2] == 1.0f ) {
		return true;
	} else {
		return false;
	}
}

inline Matrix3 Matrix3::rotation( const Vector3 hpr_in_deg ) {
	return Matrix3::rotation( Quat::rotation( hpr_in_deg ) );
}

inline Matrix3 Matrix3::rotation( const float angle_in_rad, const Vector3 axis_in ) {
	Vector3 axis(axis_in);

	// from http://en.wikipedia.org/wiki/Rotation_matrix#Axis_and_angle

	float c = cos(angle_in_rad); 
	float s = sin(angle_in_rad); 
	float C = 1-c;
	float x = axis[0], y = axis[1], z = axis[2];

	float xs = x*s;   
	float ys = y*s;   
	float zs = z*s;
	float xC = x*C;   
	float yC = y*C;   
	float zC = z*C;
	float xyC = x*yC; 
	float yzC = y*zC; 
	float zxC = z*xC;

	return Matrix3(	
		Vector3( x*xC+c, xyC+zs, zxC-ys ), 
		Vector3( xyC-zs, y*yC+c, yzC+xs ), 
		Vector3( zxC+ys, yzC-xs, z*zC+c ) );
}


inline Matrix3 Matrix3::rotation( const Quat rot_in ) {
	Quat rot(rot_in);

	// from http://www.j3d.org/matrix_faq/matrfaq_latest.html

	float xx, xy, xz, xw, yy, yz, yw, zz, zw;
	float X = rot[0], Y = rot[1], Z = rot[2], W = rot[3];

	xx      = X * X;
	xy      = X * Y;
	xz      = X * Z;
	xw      = X * W;
	yy      = Y * Y;
	yz      = Y * Z;
	yw      = Y * W;
	zz      = Z * Z;
	zw      = Z * W;

	Matrix3 mat;

	mat[0][0]  = 1.0f - 2.0f * ( yy + zz );
	mat[1][0]  =     2.0f * ( xy - zw );
	mat[2][0]  =     2.0f * ( xz + yw );
	mat[0][1]  =     2.0f * ( xy + zw );
	mat[1][1]  = 1.0f - 2.0f * ( xx + zz );
	mat[2][1]  =     2.0f * ( yz - xw );
	mat[0][2]  =     2.0f * ( xz - yw );
	mat[1][2]  =     2.0f * ( yz + xw );
	mat[2][2]  = 1.0f - 2.0f * ( xx + yy );

	return mat;

}

inline Matrix3 Matrix3::translation( const Vector3 trans_in ) {
	Vector3 trans(trans_in);

	Matrix3 mat;
	mat._cols[2][0] = trans[0];
	mat._cols[2][1] = trans[1];
	mat._cols[2][2] = trans[2];

	return mat;
}

inline void Matrix3::print( const std::string name ) {
	if( name != "" ) {
		std::cout << "Matrix3 " << name << " = { " << std::endl;
	} else {
		std::cout << "Matrix3 = { " << std::endl;
	}

	std::cout << _cols[0][0] << ", " << _cols[1][0] << ", " << _cols[2][0] << std::endl;
	std::cout << _cols[0][1] << ", " << _cols[1][1] << ", " << _cols[2][1] << std::endl;
	std::cout << _cols[0][2] << ", " << _cols[1][2] << ", " << _cols[2][2] << std::endl;
	std::cout << "}" << std::endl << std::endl;
}

inline Matrix3 Matrix3::rotation( const Vector3 from, const Vector3 to ) {
	return Matrix3( Matrix3::rotation( Quat::rotation( from, to ) ) );
}

inline Matrix3 Matrix3::scale( const float xs, const float ys, const float zs ) {
	Matrix3 scale_mat;
	scale_mat[0][0] = xs;
	scale_mat[1][1] = ys;
	scale_mat[2][2] = zs;
	return scale_mat;
}

inline Matrix3 Matrix3::scale( const float s ) {
	Matrix3 scale_mat;
	scale_mat[0][0] = s;
	scale_mat[1][1] = s;
	scale_mat[2][2] = s;
	return scale_mat;
}

inline Matrix3 Matrix3::transpose( const Matrix3 m ) {
	Matrix3 transp(m);
	transp.transpose();
	return transp;
}


inline Vector3 Matrix3::getEulerAngles() {
#if 1

	Vector3 retval;

	float h, p, r;
	float A, B;

	B = _cols[1][2];
	p = asinf( B );
	A = cosf( p );

	
	if( fabs( A ) > 0.005f ) {
		h = atan2f( -_cols[0][2]/A, _cols[2][2]/A ); // atan2( D, C )
		r = atan2f( -_cols[1][0]/A, _cols[1][1]/A ); // atan2( F, E )
	} else {
		h = 0;
		r = atan2f( _cols[2][1], _cols[2][0] ); // atan2( F, E ) when B=0, D=1
	}

	retval[0] = h;
	retval[1] = p;
	retval[2] = r;

	return retval;

#endif

#if 0


	Vector3 retval;

#if 0

	float sin_az, sin_el, sin_rl;
	float cos_az, cos_el, cos_rl;

	sin_el = - _cols[0][2];
	cos_el = (float)sqrt( 1.0 - (sin_el * sin_el) );

	if (cos_el == 0.0) {
		sin_az = 0.0;
		cos_az = 1.0;	
	}
	else {
		sin_az = _cols[0][1] / cos_el;
		cos_az = _cols[0][0] / cos_el;
	}

	sin_rl =   sin_az * _cols[2][0] - cos_az * _cols[2][1];
	cos_rl = - sin_az * _cols[1][0] + cos_az * _cols[1][1];

	retval[0] = (float)atan2 (sin_az , cos_az);
	retval[1] = (float)atan2 (sin_el , cos_el);
	retval[2] = (float)atan2 (sin_rl , cos_rl);

#else

	float h, p, r;
	float A, B;

	B = _cols[2][1];
	p = asinf( B );
	A = cosf( p );

	
	if( fabs( A ) > 0.005 ) {
		h = atan2f( -_cols[2][0]/A, _cols[2][2]/A ); // atan2( D, C )
		r = atan2f( -_cols[0][1]/A, _cols[1][1]/A ); // atan2( F, E )
	} else {
		h = 0;
		r = atan2f( _cols[1][2], _cols[0][2] ); // atan2( F, E ) when B=0, D=1
	}

	retval[0] = -h;
	retval[1] = p;
	retval[2] = r;

#endif

#endif

	return retval;

}









// Matrix4

inline Matrix4::Matrix4() {
	_cols[0] = Vector4( 1, 0, 0, 0 );
	_cols[1] = Vector4( 0, 1, 0, 0 );
	_cols[2] = Vector4( 0, 0, 1, 0 );
	_cols[3] = Vector4( 0, 0, 0, 1 );
}


inline Matrix4::Matrix4( const float mat[4][4] ) {
	_cols[0] = Vector4( mat[0][0], mat[0][1], mat[0][2], mat[0][3] );
	_cols[1] = Vector4( mat[1][0], mat[1][1], mat[1][2], mat[1][3] );
	_cols[2] = Vector4( mat[2][0], mat[2][1], mat[2][2], mat[2][3] );
	_cols[3] = Vector4( mat[3][0], mat[3][1], mat[3][2], mat[3][3] );

}

inline Matrix4::Matrix4( const Matrix4& rhs ) {
	_cols[0] = rhs._cols[0];
	_cols[1] = rhs._cols[1];
	_cols[2] = rhs._cols[2];
	_cols[3] = rhs._cols[3];

}

inline Matrix4::Matrix4( const Matrix3& rhs ) {
	Matrix3 rhs_c(rhs);
	_cols[0] = Vector4( rhs_c[0], 0.0f );
	_cols[1] = Vector4( rhs_c[1], 0.0f );
	_cols[2] = Vector4( rhs_c[2], 0.0f );
	_cols[3] = Vector4( 0.0f, 0.0f, 0.0f, 1.0f );

}

inline Matrix4::Matrix4( const float m00, const float m10, const float m20, const float m30, const float m01, const float m11, const float m21, const float m31, const float m02, const float m12, const float m22, const float m32, const float m03, const float m13, const float m23, const float m33 ) {
	
	_cols[0] = Vector4( m00, m01, m02, m03 );
	_cols[1] = Vector4( m10, m11, m12, m13 );
	_cols[2] = Vector4( m20, m21, m22, m23 );
	_cols[3] = Vector4( m30, m31, m32, m33 );

}


inline void Matrix4::fill( float mat[4][4] ) {
	for( unsigned int i=0; i<4; i++ ) {
		for( int j=0; j<4; j++ ) {
			mat[i][j] = _cols[i][j];
		}
	}
}


inline Matrix4::Matrix4( const Vector4 col0, const Vector4 col1, const Vector4 col2, const Vector4 col3 ) {
	_cols[0] = col0;
	_cols[1] = col1;
	_cols[2] = col2;
	_cols[3] = col3;

}


inline Vector4& Matrix4::operator [](const int idx) {
	return _cols[idx]; 
}


inline Matrix4 Matrix4::operator *(const Matrix4 rhs_in) {
	Matrix4 rhs(rhs_in);
	Matrix4 retmat( 
		Vector4( rhs[0][0]*_cols[0][0] + rhs[1][0]*_cols[0][1] + rhs[2][0]*_cols[0][2] + rhs[3][0]*_cols[0][3],
		         rhs[0][1]*_cols[0][0] + rhs[1][1]*_cols[0][1] + rhs[2][1]*_cols[0][2] + rhs[3][1]*_cols[0][3],
		         rhs[0][2]*_cols[0][0] + rhs[1][2]*_cols[0][1] + rhs[2][2]*_cols[0][2] + rhs[3][2]*_cols[0][3],
		         rhs[0][3]*_cols[0][0] + rhs[1][3]*_cols[0][1] + rhs[2][3]*_cols[0][2] + rhs[3][3]*_cols[0][3] ),

		Vector4( rhs[0][0]*_cols[1][0] + rhs[1][0]*_cols[1][1] + rhs[2][0]*_cols[1][2] + rhs[3][0]*_cols[1][3],
		         rhs[0][1]*_cols[1][0] + rhs[1][1]*_cols[1][1] + rhs[2][1]*_cols[1][2] + rhs[3][1]*_cols[1][3], 
		         rhs[0][2]*_cols[1][0] + rhs[1][2]*_cols[1][1] + rhs[2][2]*_cols[1][2] + rhs[3][2]*_cols[1][3],
		         rhs[0][3]*_cols[1][0] + rhs[1][3]*_cols[1][1] + rhs[2][3]*_cols[1][2] + rhs[3][3]*_cols[1][3] ),

		Vector4( rhs[0][0]*_cols[2][0] + rhs[1][0]*_cols[2][1] + rhs[2][0]*_cols[2][2] + rhs[3][0]*_cols[2][3],
		         rhs[0][1]*_cols[2][0] + rhs[1][1]*_cols[2][1] + rhs[2][1]*_cols[2][2] + rhs[3][1]*_cols[2][3], 
		         rhs[0][2]*_cols[2][0] + rhs[1][2]*_cols[2][1] + rhs[2][2]*_cols[2][2] + rhs[3][2]*_cols[2][3],
		         rhs[0][3]*_cols[2][0] + rhs[1][3]*_cols[2][1] + rhs[2][3]*_cols[2][2] + rhs[3][3]*_cols[2][3] ),

		Vector4( rhs[0][0]*_cols[3][0] + rhs[1][0]*_cols[3][1] + rhs[2][0]*_cols[3][2] + rhs[3][0]*_cols[3][3],
		         rhs[0][1]*_cols[3][0] + rhs[1][1]*_cols[3][1] + rhs[2][1]*_cols[3][2] + rhs[3][1]*_cols[3][3], 
		         rhs[0][2]*_cols[3][0] + rhs[1][2]*_cols[3][1] + rhs[2][2]*_cols[3][2] + rhs[3][2]*_cols[3][3],
		         rhs[0][3]*_cols[3][0] + rhs[1][3]*_cols[3][1] + rhs[2][3]*_cols[3][2] + rhs[3][3]*_cols[3][3] ) );

	return retmat;
}

inline Matrix4 Matrix4::operator *(const Quat rhs) {
	return *this * Matrix4::rotation( rhs );
}


inline Vector4 Matrix4::operator *(const Vector4 rhs_in) {
	Vector4 rhs(rhs_in);
	return Vector4( _cols[0][0]*rhs[0] + _cols[1][0]*rhs[1] + _cols[2][0]*rhs[2] + _cols[3][0]*rhs[3],
		              _cols[0][1]*rhs[0] + _cols[1][1]*rhs[1] + _cols[2][1]*rhs[2] + _cols[3][1]*rhs[3], 
		              _cols[0][2]*rhs[0] + _cols[1][2]*rhs[1] + _cols[2][2]*rhs[2] + _cols[3][2]*rhs[3],
		              _cols[0][3]*rhs[0] + _cols[1][3]*rhs[1] + _cols[2][3]*rhs[2] + _cols[3][3]*rhs[3] );
}

inline Matrix4 Matrix4::operator *(const float rhs) {
	return Matrix4( _cols[0] * rhs, _cols[1] * rhs, _cols[2] * rhs, _cols[3] * rhs );
}

inline Matrix4 Matrix4::operator /(const float rhs) {
	return Matrix4( _cols[0] / rhs, _cols[1] / rhs, _cols[2] / rhs, _cols[3] / rhs );
}

inline Matrix4 Matrix4:: operator +(const Matrix4 rhs) {
	return Matrix4( _cols[0] + rhs._cols[0], _cols[1] + rhs._cols[1], _cols[2] + rhs._cols[2], _cols[3] + rhs._cols[3] );

}

inline Matrix4 Matrix4:: operator -(const Matrix4 rhs) {
	return Matrix4( _cols[0] - rhs._cols[0], _cols[1] - rhs._cols[1], _cols[2] - rhs._cols[2], _cols[3] - rhs._cols[3] );

}


inline void Matrix4::transpose() {
	Matrix4 mat( 
		Vector4( _cols[0][0], _cols[1][0], _cols[2][0], _cols[3][0] ),
		Vector4( _cols[0][1], _cols[1][1], _cols[2][1], _cols[3][1] ),
		Vector4( _cols[0][2], _cols[1][2], _cols[2][2], _cols[3][2] ),
		Vector4( _cols[0][3], _cols[1][3], _cols[2][3], _cols[3][3] )
		);

	_cols[0] = mat._cols[0];
	_cols[1] = mat._cols[1];
	_cols[2] = mat._cols[2];
	_cols[3] = mat._cols[3];
}

inline Vector4 Matrix4::col( const int i ) {
	return _cols[i];
}

inline Vector4 Matrix4::row( const int i ) {
	return Vector4( _cols[0][i], _cols[1][i], _cols[2][i], _cols[3][i] );
}

inline void Matrix4::set_col( const int which, const Vector4 col ) {
	_cols[which] = col;
}

inline float Matrix4::trace() {
	return _cols[0][0] + _cols[1][1] + _cols[2][2] + _cols[3][3];
}

inline bool Matrix4::is_identity() {
	if( _cols[0][0] == 1.0f &&
		  _cols[0][1] == 0.0f &&
			_cols[0][2] == 0.0f &&
			_cols[0][3] == 0.0f &&

			_cols[1][0] == 0.0f &&
			_cols[1][1] == 1.0f &&
			_cols[1][2] == 0.0f &&
			_cols[1][3] == 0.0f &&

			_cols[2][0] == 0.0f &&
			_cols[2][1] == 0.0f &&
			_cols[2][2] == 1.0f &&
			_cols[2][3] == 0.0f &&

			_cols[3][0] == 0.0f &&
			_cols[3][1] == 0.0f &&
			_cols[3][2] == 0.0f &&
			_cols[3][3] == 1.0f ) {
		return true;
	} else {
		return false;
	}
}


inline Matrix4 Matrix4::rotation( const float angle_in_rad, const Vector3 axis_in ) {
	return Matrix4( Matrix3::rotation( angle_in_rad, axis_in ) );
}


inline Matrix4 Matrix4::rotation( const Quat rot_in ) {
	return Matrix4( Matrix3::rotation( rot_in ) );
}

inline Matrix4 Matrix4::rotation( const Vector3 hpr_in_deg ) {
	return Matrix4( Matrix3::rotation( hpr_in_deg ) );
}

inline Matrix4 Matrix4::translation( const Vector3 trans_in ) {
	return Matrix4( Matrix3::translation( trans_in ) );
}

inline void Matrix4::print( const std::string name ) {
	if( name != "" ) {
		std::cout << "Matrix4 " << name << " = { " << std::endl;
	} else {
		std::cout << "Matrix4 = { " << std::endl;
	}

	std::cout << _cols[0][0] << ", " << _cols[1][0] << ", " << _cols[2][0] << ", " << _cols[3][0] << std::endl;
	std::cout << _cols[0][1] << ", " << _cols[1][1] << ", " << _cols[2][1] << ", " << _cols[3][1] << std::endl;
	std::cout << _cols[0][2] << ", " << _cols[1][2] << ", " << _cols[2][2] << ", " << _cols[3][2] << std::endl;
	std::cout << _cols[0][3] << ", " << _cols[1][3] << ", " << _cols[2][3] << ", " << _cols[3][3] << std::endl;
	std::cout << "}" << std::endl << std::endl;


}

inline Matrix4 Matrix4::rotation( const Vector3 from, const Vector3 to ) {
	return Matrix4( Matrix3::rotation( from, to ) );
}

inline Matrix4 Matrix4::scale( const float xs, const float ys, const float zs ) {
	return Matrix4( Matrix3::scale( xs, ys, zs ) );
}

inline Matrix4 Matrix4::scale( const float s ) {
	return Matrix4( Matrix3::scale( s ) );
}

inline Matrix4 Matrix4::transpose( const Matrix4 m ) {
	Matrix4 transp(m);
	transp.transpose();
	return transp;
}

inline Vector3 Matrix4::getEulerAngles() {
	Vector3 retval;

	float h, p, r;
	float A, B;

	B = _cols[1][2];
	p = asinf( B );
	A = cosf( p );

	
	if( fabs( A ) > 0.005f ) {
		h = atan2f( -_cols[0][2]/A, _cols[2][2]/A ); // atan2( D, C )
		r = atan2f( -_cols[1][0]/A, _cols[1][1]/A ); // atan2( F, E )
	} else {
		h = 0;
		r = atan2f( _cols[2][1], _cols[2][0] ); // atan2( F, E ) when B=0, D=1
	}

	retval[0] = h;
	retval[1] = p;
	retval[2] = r;

	return retval;

}





// Line

inline Line::Line ( const Vector3& d, const Vector3& p ) : _dir( d ),  _pos1( p ) {
	_dir.normalize();
	_pos2 = _pos1 + _dir;
}

inline Line::Line ( const Line& line ) {
	_dir = line._dir;
	_pos1 = line._pos1;
}

inline Vector3 Line::getClosestPoint( const Vector3& pos ) {

	Vector3 ncp( pos );

	Vector3 u = _pos2 - _pos1;
	Vector3 v = ncp - _pos1;
	Vector3 closest = _pos1 + u * ( (u*v) / (u*u) );
	
	return closest;
}





// Plane


inline Plane::Plane () : _p0( 0.0f, 0.0f, 0.0f ), _p1( 1.0f, 0.0f, 0.0f ), _p2( 1.0f, 1.0f, 0.0f ) {
	init();
}

inline Plane::Plane ( const Plane& p ) : _p0(p._p0), _p1(p._p1), _p2(p._p2) {
	init();
}

inline Plane::Plane ( Vector3 p0, Vector3 p1, Vector3 p2 ) : _p0( p0 ), _p1( p1 ), _p2( p2 ) {
	init();
}

inline Plane::Plane ( Vector3 p, Vector3 norm ) : _p0( p ), _norm( norm ) {
	
	_a = _norm[0];
	_b = _norm[1];
	_c = _norm[2];
	_d = -( _norm[0] * _p0[0] + _norm[1] * _p0[1] +  _norm[2] * _p0[2] );

	_norm.normalize();

	// Make up some valid points for pos2 and pos3
	_p1 = Vector3( _norm[1], _norm[2], _norm[0] ) ^ _norm;
	_p2 = _p1 ^ _norm;

	_p1+=_p0;
	_p2+=_p0;
} 

inline void Plane::init() {
	_a = ( _p1[1] - _p0[1] ) * ( _p2[2] - _p0[2] ) - ( _p1[2] - _p0[2] ) * ( _p2[1] - _p0[1] );
	_b = ( _p1[2] - _p0[2] ) * ( _p2[0] - _p0[0] ) - ( _p1[0] - _p0[0] ) * ( _p2[2] - _p0[2] );
	_c = ( _p1[0] - _p0[0] ) * ( _p2[1] - _p0[1] ) - ( _p1[1] - _p0[1] ) * ( _p2[0] - _p0[0] );
	_d = -( _p1[0] * _a + _p1[1] * _b + _p1[2] * _c );
	
	double denom = sqrt( _a * _a + _b * _b + _c * _c );
	_a /= denom;
	_b /= denom;
	_c /= denom;
	_d /= denom;
	
	Vector3 dir1 = _p1 - _p0;
	dir1.normalize();
	
	Vector3 dir2 = _p2 - _p0;
	dir2.normalize();
	
	_norm = dir1^dir2;
	_norm.normalize();
}

inline Vector3 Plane::getClosestPoint( Vector3 in ) {
	
	// Create a vector from the surface to  point src
	Vector3 w = in - _p0;
	
	// The closest point in the plane to point p is in the negative normal
	// direction a distance w dot p
	Vector3 out = in - _norm * (w * _norm); // (w dot normal)

	return out;
}

inline Vector3 Plane::getNormal() {
	return _norm;
}
 
 
inline Vector3 Plane::intersect( const Line line ) {
	Vector3 nc_dir( line._dir );
	Vector3 nc_pos1( line._pos1 );

	double denom = ( _a * nc_dir[0] + _b * nc_dir[1] + _c * nc_dir[2] );
	if ( fabs( denom ) < .001f ) {
		return Vector3();
	}

	double t = -( _a * nc_pos1[0] + _b * nc_pos1[1] + _c * nc_pos1[2] + _d ) / denom;
			
	Vector3 pos;

	pos[0] = nc_pos1[0] + nc_dir[0] * (float)t;
	pos[1] = nc_pos1[1] + nc_dir[1] * (float)t;
	pos[2] = nc_pos1[2] + nc_dir[2] * (float)t;

	return pos;							
}

inline double	Plane::whichSide	(	Vector3 p )	{
	return _a * p[0] + _b * p[1] + _c * p[2] + _d;
}

