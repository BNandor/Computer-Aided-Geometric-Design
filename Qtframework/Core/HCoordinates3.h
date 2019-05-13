#pragma once

#include <cmath>
#include <GL/glew.h>
#include <iostream>

namespace cagd
{
    //--------------------------------------
    // 3-dimensional homogeneous coordinates
    //--------------------------------------
    class HCoordinate3
    {
    protected:
        GLfloat _data[4]; // x, y, z, w;

    public:
        // default constructor
        HCoordinate3();

        // special constructor
        HCoordinate3(GLfloat x, GLfloat y, GLfloat z = 0.0, GLfloat w = 1.0);

        // homework: get components by value
        GLfloat operator[](GLuint rhs) const{return _data[rhs];}
        GLfloat x() const{return _data[0];}
        GLfloat y() const{return _data[1];}
        GLfloat z() const{return _data[2];}
        GLfloat w() const{return _data[3];}

        // homework: get components by reference
        GLfloat& operator[](GLuint rhs){return _data[rhs];}
        GLfloat& x(){return _data[0];}
        GLfloat& y(){return _data[1];}
        GLfloat& z(){return _data[2];}
        GLfloat& w(){return _data[3];}
        //mine
          void setx(GLfloat x){_data[0]=x;}
          void sety(GLfloat y){_data[1]=y;}
          void setz(GLfloat z){_data[2]=z;}
          void setw(GLfloat w){_data[3]=w;}
        //eof mine
        // add
        const HCoordinate3 operator +(const HCoordinate3& rhs) const;

        // homework: add to this
        HCoordinate3& operator +=(const HCoordinate3& rhs);

        // homework: subtract
        const HCoordinate3 operator -(const HCoordinate3& rhs) const;

        // homework: subtract from this
        HCoordinate3& operator -=(const HCoordinate3& rhs);

        // homework: dot product
        GLfloat operator *(const HCoordinate3& rhs) const;

        // homework: cross product
        const HCoordinate3 operator ^(const HCoordinate3& rhs) const;

        // homework: cross product with this
        HCoordinate3& operator ^=(const HCoordinate3& rhs);

        // homework: multiplicate with scalar from right
        const HCoordinate3 operator *(GLfloat rhs) const;

        // homework: multiplicate this with a scalar
        HCoordinate3& operator *=(GLfloat rhs);

        // homework: divide with scalar
        const HCoordinate3 operator /(GLfloat rhs) const;

        // homework: divide this with a scalar
        HCoordinate3& operator /=(GLfloat rhs);

        // homework: length of vector represented by this homogeneous coordinate
        GLfloat length() const;

        // homework: normalize
        HCoordinate3& normalize();
    };

    // default constructor
    inline HCoordinate3::HCoordinate3()
    {
        _data[0] = _data[1] = _data[2] = 0.0;
        _data[3] = 1.0;
    }

    // special constructor
    inline HCoordinate3::HCoordinate3(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
    {
        _data[0] = x;
        _data[1] = y;
        _data[2] = z;
        _data[3] = w;
    }

    // add
    inline const HCoordinate3 HCoordinate3::operator +(const HCoordinate3& rhs) const
    {
        return HCoordinate3(
                rhs.w() * x() + w() * rhs.x(),
                rhs.w() * y() + w() * rhs.y(),
                rhs.w() * z() + w() * rhs.z(),
                w() * rhs.w());
    }

    //mine
    HCoordinate3& HCoordinate3::operator +=(const HCoordinate3& rhs){
      GLfloat x = _data[0];
      GLfloat y = _data[1];
      GLfloat z = _data[2];
      GLfloat w = _data[3];

      _data[0]=rhs.w() * x + w * rhs.x();
      _data[1]=rhs.w() * y + w * rhs.y();
      _data[2]=rhs.w() * z + w * rhs.z();
      _data[3]=w * rhs.w();
      return *this;
    }
    inline const HCoordinate3 HCoordinate3::operator -(const HCoordinate3& rhs) const
    {
        return HCoordinate3(
                rhs.w() * x() - w() * rhs.x(),
                rhs.w() * y() - w() * rhs.y(),
                rhs.w() * z() - w() * rhs.z(),
                w() * rhs.w());
    }
    HCoordinate3& HCoordinate3::operator -=(const HCoordinate3& rhs){
      GLfloat x = _data[0];
      GLfloat y = _data[1];
      GLfloat z = _data[2];
      GLfloat w = _data[3];

      _data[0]=rhs.w() * x - w * rhs.x();
      _data[1]=rhs.w() * y - w * rhs.y();
      _data[2]=rhs.w() * z - w * rhs.z();
      _data[3]=w * rhs.w();
      return *this;
    }
    GLfloat HCoordinate3::operator *(const HCoordinate3& rhs) const{
      return (x()*rhs.x() + y()*rhs.y() + z()*rhs.z())/(w()*rhs.w());
    }
    const HCoordinate3  HCoordinate3::operator ^(const HCoordinate3& rhs) const{
      return HCoordinate3(
            y()*rhs.z() - z()*rhs.y(),
            z()*rhs.x() - x()*rhs.z(),
            x()*rhs.y() - y()*rhs.x(),
            w()*rhs.w()
            );
    }
    HCoordinate3& HCoordinate3::operator ^=(const HCoordinate3& rhs){
      GLfloat x = _data[0];
      GLfloat y = _data[1];
      GLfloat z = _data[2];
      GLfloat w = _data[3];
      _data[0]=y*rhs.z() - z*rhs.y();
      _data[1]=z*rhs.x() - x*rhs.z();
      _data[2]=x*rhs.y() - y*rhs.x();
      _data[3]=w*rhs.w();
      return *this;
    }

    const HCoordinate3 HCoordinate3::operator *(GLfloat rhs) const{
      return HCoordinate3(
            rhs*x(),
            rhs*y(),
            rhs*z(),
            w()
            );
    }
    HCoordinate3& HCoordinate3::operator *=(GLfloat rhs) {
            _data[0]*=rhs;
            _data[1]*=rhs;
            _data[2]*=rhs;
            return *this;
    }
    const HCoordinate3 HCoordinate3::operator /(GLfloat rhs) const{
      return HCoordinate3(
            x()/rhs,
            y()/rhs,
            z()/rhs,
            w()
            );
    }
    HCoordinate3& HCoordinate3::operator /=(GLfloat rhs) {
            _data[0]/=rhs;
            _data[1]/=rhs;
            _data[2]/=rhs;
            return *this;
    }
    GLfloat HCoordinate3::length() const{
      return sqrt((x()/w())*(x()/w()) + (y()/w())*(y()/w()) + (z()/w())*(z()/w()));
    }
    HCoordinate3& HCoordinate3::normalize(){
      _data[0]/=_data[3];
      _data[1]/=_data[3];
      _data[2]/=_data[3];
      _data[3]=1;
      return *this;
    }
    //eof mine
    // homework: scale from left with a scalar
    inline const HCoordinate3 operator *(GLfloat lhs, const HCoordinate3& rhs){return rhs*lhs;}

    // homework: output to stream
    inline std::ostream& operator <<(std::ostream& lhs, const HCoordinate3& rhs){
      lhs<<rhs.x()<<" "<<rhs.y()<<" "<<rhs.z()<<" "<<rhs.w();
      return lhs;
    }

    // homework: input from stream
    inline std::istream& operator >>(std::istream& lhs, HCoordinate3& rhs){
      GLfloat num;
      lhs>>num;rhs.setx(num);
      lhs>>num;rhs.sety(num);
      lhs>>num;rhs.setz(num);
      lhs>>num;rhs.setw(num);
      return lhs;
    }
}
