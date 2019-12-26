#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

#include <gsl/gsl_matrix.h>
#include <gsl/gsl_linalg.h>
// you need to add the following libraries to your project : gsl, gslcblas

class Vec3 {
private:
    float mVals[3];
public:
    Vec3() {}
    Vec3( float x , float y , float z ) {
       mVals[0] = x; mVals[1] = y; mVals[2] = z;
    }
    float & operator [] (unsigned int c) { return mVals[c]; }
    float operator [] (unsigned int c) const { return mVals[c]; }
    void operator = (Vec3 const & other) {
       mVals[0] = other[0] ; mVals[1] = other[1]; mVals[2] = other[2];
    }
    float squareLength() const {
       return mVals[0]*mVals[0] + mVals[1]*mVals[1] + mVals[2]*mVals[2];
    }
    float length() const { return sqrt( squareLength() ); }
    void normalize() { float L = length(); mVals[0] /= L; mVals[1] /= L; mVals[2] /= L; }
    static float dot( Vec3 const & a , Vec3 const & b ) {
       return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
    }
    static Vec3 cross( Vec3 const & a , Vec3 const & b ) {
       return Vec3( a[1]*b[2] - a[2]*b[1] ,
                    a[2]*b[0] - a[0]*b[2] ,
                    a[0]*b[1] - a[1]*b[0] );
    }
    void operator += (Vec3 const & other) {
        mVals[0] += other[0];
        mVals[1] += other[1];
        mVals[2] += other[2];
    }
    void operator -= (Vec3 const & other) {
        mVals[0] -= other[0];
        mVals[1] -= other[1];
        mVals[2] -= other[2];
    }
    void operator *= (float s) {
        mVals[0] *= s;
        mVals[1] *= s;
        mVals[2] *= s;
    }
    void operator /= (float s) {
        mVals[0] /= s;
        mVals[1] /= s;
        mVals[2] /= s;
    }

    static Vec3 Rand(float magnitude = 1.f) {
        return Vec3( magnitude * (-1.f + 2.f * rand() / (float)(RAND_MAX)) , magnitude * (-1.f + 2.f * rand() / (float)(RAND_MAX)) , magnitude * (-1.f + 2.f * rand() / (float)(RAND_MAX)) );
    }

    Vec3 getOrthogonal() const {
        // CAREFUL ! THE NORM IS NOT PRESERVED !!!!
        if( mVals[0] == 0 ) {
            return Vec3( 0 , mVals[2] , -mVals[1] );
        }
        else if( mVals[1] == 0 ) {
            return Vec3( mVals[2] , 0 , -mVals[0] );
        }
        return Vec3( mVals[1] , -mVals[0] , 0 );
    }
};

static inline Vec3 operator + (Vec3 const & a , Vec3 const & b) {
   return Vec3(a[0]+b[0] , a[1]+b[1] , a[2]+b[2]);
}
static inline Vec3 operator - (Vec3 const & a , Vec3 const & b) {
   return Vec3(a[0]-b[0] , a[1]-b[1] , a[2]-b[2]);
}
static inline Vec3 operator * (float a , Vec3 const & b) {
   return Vec3(a*b[0] , a*b[1] , a*b[2]);
}
static inline Vec3 operator / (Vec3 const &  a , float b) {
   return Vec3(a[0]/b , a[1]/b , a[2]/b);
}
static inline std::ostream & operator << (std::ostream & s , Vec3 const & p) {
    s << p[0] << " " << p[1] << " " << p[2];
    return s;
}
static inline std::istream & operator >> (std::istream & s , Vec3 & p) {
    s >> p[0] >> p[1] >> p[2];
    return s;
}













class Mat3
{
public:
    ////////////         CONSTRUCTORS          //////////////
    Mat3()
    {
        vals[0] = 0;
        vals[1] = 0;
        vals[2] = 0;
        vals[3] = 0;
        vals[4] = 0;
        vals[5] = 0;
        vals[6] = 0;
        vals[7] = 0;
        vals[8] = 0;
    }
    Mat3( float v1 , float v2 , float v3 , float v4 , float v5 , float v6 , float v7 , float v8 , float v9)
    {
        vals[0] = v1;
        vals[1] = v2;
        vals[2] = v3;
        vals[3] = v4;
        vals[4] = v5;
        vals[5] = v6;
        vals[6] = v7;
        vals[7] = v8;
        vals[8] = v9;
    }
    Mat3( const Mat3 & m )
    {
        for(int i = 0 ; i < 3 ; ++i )
            for(int j = 0 ; j < 3 ; ++j )
                (*this)(i,j) = m(i,j);
    }


    bool isnan() const {
        return std::isnan(vals[0]) || std::isnan(vals[1]) || std::isnan(vals[2])
                 || std::isnan(vals[3]) || std::isnan(vals[4]) || std::isnan(vals[5])
                 || std::isnan(vals[6]) || std::isnan(vals[7]) || std::isnan(vals[8]);
    }


    void operator = (const Mat3 & m)
    {
        for(int i = 0 ; i < 3 ; ++i )
            for(int j = 0 ; j < 3 ; ++j )
                (*this)(i,j) = m(i,j);
    }




    void operator += (const Mat3 & m)
    {
        for(int i = 0 ; i < 3 ; ++i )
            for(int j = 0 ; j < 3 ; ++j )
                (*this)(i,j) += m(i,j);
    }
    void operator -= (const Mat3 & m)
    {
        for(int i = 0 ; i < 3 ; ++i )
            for(int j = 0 ; j < 3 ; ++j )
                (*this)(i,j) -= m(i,j);
    }
    void operator /= (double s)
    {
        for( unsigned int c = 0 ; c < 9 ; ++c )
            vals[c] /= s;
    }



    Mat3 operator - (const Mat3 & m2)
    {
        return Mat3( (*this)(0,0)-m2(0,0) , (*this)(0,1)-m2(0,1) , (*this)(0,2)-m2(0,2) , (*this)(1,0)-m2(1,0) , (*this)(1,1)-m2(1,1) , (*this)(1,2)-m2(1,2) , (*this)(2,0)-m2(2,0) , (*this)(2,1)-m2(2,1) , (*this)(2,2)-m2(2,2) );
    }
    Mat3 operator + (const Mat3 & m2)
    {
        return Mat3( (*this)(0,0)+m2(0,0) , (*this)(0,1)+m2(0,1) , (*this)(0,2)+m2(0,2) , (*this)(1,0)+m2(1,0) , (*this)(1,1)+m2(1,1) , (*this)(1,2)+m2(1,2) , (*this)(2,0)+m2(2,0) , (*this)(2,1)+m2(2,1) , (*this)(2,2)+m2(2,2) );
    }




    Mat3 operator / (float s)
    {
        return Mat3( (*this)(0,0)/s , (*this)(0,1)/s , (*this)(0,2)/s , (*this)(1,0)/s , (*this)(1,1)/s , (*this)(1,2)/s , (*this)(2,0)/s , (*this)(2,1)/s , (*this)(2,2)/s );
    }
    Mat3 operator * (float s)
    {
        return Mat3( (*this)(0,0)*s , (*this)(0,1)*s , (*this)(0,2)*s , (*this)(1,0)*s , (*this)(1,1)*s , (*this)(1,2)*s , (*this)(2,0)*s , (*this)(2,1)*s , (*this)(2,2)*s );
    }


    Vec3 operator * (const Vec3 & p) // computes m.p
    {
        return Vec3(
                    (*this)(0,0)*p[0] + (*this)(0,1)*p[1] + (*this)(0,2)*p[2],
                    (*this)(1,0)*p[0] + (*this)(1,1)*p[1] + (*this)(1,2)*p[2],
                    (*this)(2,0)*p[0] + (*this)(2,1)*p[1] + (*this)(2,2)*p[2]);
    }

    Mat3 operator * (const Mat3 & m2)
    {
        return Mat3(
                    (*this)(0,0)*m2(0,0) + (*this)(0,1)*m2(1,0) + (*this)(0,2)*m2(2,0) ,
                    (*this)(0,0)*m2(0,1) + (*this)(0,1)*m2(1,1) + (*this)(0,2)*m2(2,1) ,
                    (*this)(0,0)*m2(0,2) + (*this)(0,1)*m2(1,2) + (*this)(0,2)*m2(2,2) ,
                    (*this)(1,0)*m2(0,0) + (*this)(1,1)*m2(1,0) + (*this)(1,2)*m2(2,0) ,
                    (*this)(1,0)*m2(0,1) + (*this)(1,1)*m2(1,1) + (*this)(1,2)*m2(2,1) ,
                    (*this)(1,0)*m2(0,2) + (*this)(1,1)*m2(1,2) + (*this)(1,2)*m2(2,2) ,
                    (*this)(2,0)*m2(0,0) + (*this)(2,1)*m2(1,0) + (*this)(2,2)*m2(2,0) ,
                    (*this)(2,0)*m2(0,1) + (*this)(2,1)*m2(1,1) + (*this)(2,2)*m2(2,1) ,
                    (*this)(2,0)*m2(0,2) + (*this)(2,1)*m2(1,2) + (*this)(2,2)*m2(2,2)
                    );
    }





    ////////        ACCESS TO COORDINATES      /////////
    float operator () (unsigned int i , unsigned int j) const
    { return vals[3*i + j]; }
    float & operator () (unsigned int i , unsigned int j)
    { return vals[3*i + j]; }









    ////////        BASICS       /////////
    inline float sqrnorm()
    {
        return vals[0]*vals[0] + vals[1]*vals[1] + vals[2]*vals[2]
                + vals[3]*vals[3] + vals[4]*vals[4] + vals[5]*vals[5]
                + vals[6]*vals[6] +  vals[7]*vals[7] + vals[8]*vals[8];
    }

    inline float norm()
    { return sqrt( sqrnorm() ); }

    inline float determinant() const
    {
        return vals[0] * ( vals[4] * vals[8] - vals[7] * vals[5] )
                - vals[1] * ( vals[3] * vals[8] - vals[6] * vals[5] )
                + vals[2] * ( vals[3] * vals[7] - vals[6] * vals[4] );
    }


    static
    Mat3 pseudoInverse( Mat3 const & m , bool & isRealInverse , double defaultValueForInverseSingularValue = 0.0 )
    {
        float det = m.determinant();
        if( fabs(det) != 0.0 )
        {
            isRealInverse = true;
            return Mat3( m(1,1)*m(2,2) - m(2,1)*m(1,2) , m(0,2)*m(2,1) - m(0,1)*m(2,2) , m(0,1)*m(1,2) - m(0,2)*m(1,1) ,
                             m(1,2)*m(2,0) - m(1,0)*m(2,2) , m(0,0)*m(2,2) - m(0,2)*m(2,0) , m(0,2)*m(1,0) - m(0,0)*m(1,2) ,
                             m(1,0)*m(2,1) - m(1,1)*m(2,0) , m(0,1)*m(2,0) - m(0,0)*m(2,1) , m(0,0)*m(1,1) - m(0,1)*m(1,0) ) / det ;
        }

        // otherwise:
        isRealInverse = false;
        Mat3 U ; float sx ; float sy ; float sz ; Mat3 Vt;
        m.SVD(U,sx,sy,sz,Vt);
        float sxInv = sx == 0.0 ? 1.0 / sx : defaultValueForInverseSingularValue;
        float syInv = sy == 0.0 ? 1.0 / sy : defaultValueForInverseSingularValue;
        float szInv = sz == 0.0 ? 1.0 / sz : defaultValueForInverseSingularValue;
        return Vt.getTranspose() * Mat3::diag(sxInv , syInv , szInv) * U.getTranspose();
    }


    inline float trace() const
    { return vals[0] + vals[4] + vals[8]; }




    ////////        TRANSPOSE       /////////
    inline
    void transpose()
    {
        float xy = vals[1] , xz = vals[2] , yz = vals[5];
        vals[1] = vals[3];
        vals[3] = xy;
        vals[2] = vals[6];
        vals[6] = xz;
        vals[5] = vals[7];
        vals[7] = yz;
    }
    Mat3 getTranspose() const
    {
        return Mat3(vals[0],vals[3],vals[6],vals[1],vals[4],vals[7],vals[2],vals[5],vals[8]);
    }







    // ---------- ROTATION <-> AXIS/ANGLE ---------- //
    template< class point_t >
    void getAxisAndAngleFromRotationMatrix( point_t & axis , float & angle )
    {
        angle = acos( (trace() - 1.f) / 2.f );
        axis[0] = vals[7] - vals[5];
        axis[1] = vals[2] - vals[6];
        axis[2] = vals[3] - vals[1];
        axis.normalize();
    }

    template< class point_t >
    inline static
    Mat3 getRotationMatrixFromAxisAndAngle( const point_t & axis , float angle )
    {
        Mat3 w = vectorial(axis);
        return Identity() +  w * std::sin(angle) +  w * w * ((1.0) - std::cos(angle));
    }



    // ---------- STATIC STANDARD MATRICES ---------- //
    inline static Mat3 Identity()
    {  return Mat3(1,0,0  ,  0,1,0  ,  0,0,1);  }

    inline static Mat3 Zero()
    {  return Mat3(0,0,0  ,  0,0,0  ,  0,0,0);  }

    template< typename T2 >
    inline static Mat3 diag( T2 x , T2 y ,T2 z )
    {  return Mat3(x,0,0  ,  0,y,0  ,  0,0,z);  }


    template< class point_t >
    inline static Mat3 getFromCols(const point_t & c1 , const point_t & c2 , const point_t & c3)
    {
        // 0 1 2
        // 3 4 5
        // 6 7 8
        return Mat3( c1[0] , c2[0] , c3[0] ,
                         c1[1] , c2[1] , c3[1] ,
                         c1[2] , c2[2] , c3[2] );
    }
    template< class point_t >
    inline static Mat3 getFromRows(const point_t & r1 , const point_t & r2 , const point_t & r3)
    {
        // 0 1 2
        // 3 4 5
        // 6 7 8
        return Mat3( r1[0] , r1[1] , r1[2] ,
                         r2[0] , r2[1] , r2[2] ,
                         r3[0] , r3[1] , r3[2] );
    }


    inline static Mat3 RandRotation()
    {
        Vec3 axis(-1.0 + 2.0* (float)(rand()) / (float)( RAND_MAX )  ,
                       -1.0 + 2.0* (float)(rand()) / (float)( RAND_MAX )  ,
                       -1.0 + 2.0* (float)(rand()) / (float)( RAND_MAX )  );
        axis.normalize();
        float angle = 2.0 * M_PI * ((float)(rand()) / (float)( RAND_MAX )   - 0.5 );

        return Mat3::getRotationMatrixFromAxisAndAngle( axis , angle );
    }

    inline static Mat3 RandRotation( float maxAngle )
    {
        Vec3 axis(-1.0 + 2.0* (float)(rand()) / (float)( RAND_MAX )  ,
                       -1.0 + 2.0* (float)(rand()) / (float)( RAND_MAX )  ,
                       -1.0 + 2.0* (float)(rand()) / (float)( RAND_MAX )  );
        axis.normalize();
        float angle =  maxAngle * ((float)(rand()) / (float)( RAND_MAX )   - 0.5 );

        return Mat3::getRotationMatrixFromAxisAndAngle( axis , angle );
    }


    inline static Mat3 RandRotation( Vec3 twistAxis , double maxTwist , double maxRotation )
    {
        twistAxis.normalize();
        Vec3 u = twistAxis.getOrthogonal();
        u.normalize();
        const Vec3 & v = Vec3::cross(u,twistAxis);

        double uv_axis_angle = 2.0*M_PI * (double)(rand()) / (double)(RAND_MAX);
        const Vec3 & uv = cos(uv_axis_angle)*u + sin(uv_axis_angle)*v;

        double rotation_angle = maxRotation * ( ( 2.0 *(double)(rand()) / (double)(RAND_MAX) ) - 1.0 );
        double twist_angle = maxTwist * ( ( 2.0 *(double)(rand()) / (double)(RAND_MAX) ) - 1.0 );

        return Mat3::getRotationMatrixFromAxisAndAngle(uv , rotation_angle) * Mat3::getRotationMatrixFromAxisAndAngle(twistAxis , twist_angle);
    }


    void SVD( Mat3 & U , float & sx , float & sy , float & sz , Mat3 & Vt ) const
    {
        gsl_matrix * u = gsl_matrix_alloc(3,3);
        for(unsigned int i = 0 ; i < 3; ++i)
            for(unsigned int j = 0 ; j < 3; ++j)
                gsl_matrix_set( u , i , j , (*this)(i,j) );

        gsl_matrix * v = gsl_matrix_alloc(3,3);
        gsl_vector * s = gsl_vector_alloc(3);
        gsl_vector * work = gsl_vector_alloc(3);

        gsl_linalg_SV_decomp (u,
                              v,
                              s,
                              work);

        sx = s->data[0];
        sy = s->data[1];
        sz = s->data[2];
        for(unsigned int i = 0 ; i < 3; ++i)
        {
            for(unsigned int j = 0 ; j < 3; ++j)
            {
                U(i,j) = gsl_matrix_get( u , i , j );
                Vt(i,j) = gsl_matrix_get( v , j , i );
            }
        }

        gsl_matrix_free(u);
        gsl_matrix_free(v);
        gsl_vector_free(s);
        gsl_vector_free(work);

        // a transformation float is given as R.B.S.Bt, R = rotation , B = local basis (rotation matrix), S = scales in the basis B
        // it can be obtained from the svd decomposition of float = U Sigma Vt :
        // B = V
        // S = Sigma
        // R = U.Vt
    }







    // ---------- Projections onto Rotations ----------- //


    void setRotation()
    {
        Mat3 U,Vt;
        float sx,sy,sz;
        SVD(U,sx,sy,sz,Vt);
        const Mat3 & res = U*Vt;
        if( res.determinant() < 0 )
        {
            U(0,2) = -U(0,2);
            U(1,2) = -U(1,2);
            U(2,2) = -U(2,2);
            *this = (U*Vt);
            return;
        }
        // else
        *this = (res);
    }






    template< class point_t >
    inline static
    Mat3 tensor( const point_t & p1 , const point_t & p2 )
    {
        return Mat3(
                    p1[0]*p2[0] , p1[0]*p2[1] , p1[0]*p2[2],
                    p1[1]*p2[0] , p1[1]*p2[1] , p1[1]*p2[2],
                    p1[2]*p2[0] , p1[2]*p2[1] , p1[2]*p2[2]);
    }

    template< class point_t >
    inline static
    Mat3 vectorial( const point_t & p )
    {
        return Mat3(
                    0      , -p[2]  , p[1]     ,
                    p[2]   , 0      , - p[0]   ,
                    - p[1] , p[0]   , 0
                    );
    }



    Mat3 operator - () const
    {
        return Mat3( - vals[0],- vals[1],- vals[2],- vals[3],- vals[4],- vals[5],- vals[6],- vals[7],- vals[8] );
    }


private:
    float vals[9];
    // will be noted as :
    // 0 1 2
    // 3 4 5
    // 6 7 8
};



inline static
Mat3 operator * (float s , const Mat3 & m)
{
    return Mat3( m(0,0)*s , m(0,1)*s , m(0,2)*s , m(1,0)*s , m(1,1)*s , m(1,2)*s , m(2,0)*s , m(2,1)*s , m(2,2)*s );
}


inline static std::ostream & operator << (std::ostream & s , Mat3 const & m)
{
    s << m(0,0) << " \t" << m(0,1) << " \t" << m(0,2) << std::endl << m(1,0) << " \t" << m(1,1) << " \t" << m(1,2) << std::endl << m(2,0) << " \t" << m(2,1) << " \t" << m(2,2) << std::endl;
    return s;
}


#endif
