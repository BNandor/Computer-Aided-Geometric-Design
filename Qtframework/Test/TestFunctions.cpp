#include <cmath>
#include "TestFunctions.h"
#include "../Core/Constants.h"

using namespace cagd;
using namespace std;

GLdouble spiral_on_cone::u_min = -TWO_PI;
GLdouble spiral_on_cone::u_max = +TWO_PI;

DCoordinate3 spiral_on_cone::d0(GLdouble u)
{
    return DCoordinate3(u * cos(u), u * sin(u), u);
}

DCoordinate3 spiral_on_cone::d1(GLdouble u)
{
    GLdouble c = cos(u), s = sin(u);
    return DCoordinate3(c - u * s, s + u * c, 1.0);
}

DCoordinate3 spiral_on_cone::d2(GLdouble u)
{
    GLdouble c = cos(u), s = sin(u);
    return DCoordinate3(-2.0 * s - u * c, 2.0 * c - u * s, 0);
}

GLdouble helix::u_min = -TWO_PI;
GLdouble helix::u_max = +TWO_PI;

DCoordinate3 helix::d0(GLdouble u)
{
    return DCoordinate3( cos(u),  sin(u), 2*u);
}

DCoordinate3 helix::d1(GLdouble u)
{
    GLdouble c = cos(u), s = sin(u);
    return DCoordinate3(-s, c , 2.0);
}

DCoordinate3 helix::d2(GLdouble u)
{
    GLdouble c = cos(u), s = sin(u);
    return DCoordinate3(-c, - s, 0);
}

GLdouble sphere::u_min = -TWO_PI;
GLdouble sphere::u_max = +TWO_PI;

DCoordinate3 sphere::d0(GLdouble u)
{
    return DCoordinate3( sin(u)*sin(u),  sin(u)*cos(u), cos(u));
}

DCoordinate3 sphere::d1(GLdouble u)
{
    GLdouble c = cos(u), s = sin(u);
    return DCoordinate3(2*s*c, c*c-s*s , -s);
}

DCoordinate3 sphere::d2(GLdouble u)
{
    GLdouble c = cos(u), s = sin(u);
    return DCoordinate3(2*(c*c-s*s),-4*s*c, -c);
}

GLdouble lissajous::u_min = -3*TWO_PI;
GLdouble lissajous::u_max = +3*TWO_PI;
GLdouble lissajous::a = 2;
GLdouble lissajous::b = 3;
GLdouble lissajous::omega = 1.3;
GLdouble lissajous::delta = 0.75;

DCoordinate3 lissajous::d0(GLdouble u)
{
    return DCoordinate3( a*sin(omega*u+delta),  b*sin(u), cos(u));
}

DCoordinate3 lissajous::d1(GLdouble u)
{
    GLdouble c = cos(u), s = sin(u);
    return DCoordinate3(a*omega*cos(omega*u+delta), b*c, -s);
}

DCoordinate3 lissajous::d2(GLdouble u)
{
    GLdouble c = cos(u), s = sin(u);
    return DCoordinate3(-a*omega*omega*sin(omega*u+delta), -b*s, -c);
}

GLdouble rose::u_min = -2*TWO_PI;
GLdouble rose::u_max = +2*TWO_PI;
GLdouble rose::k = 1.7;

DCoordinate3 rose::d0(GLdouble u)
{
    return DCoordinate3( cos(k*u)*cos(u),  sin(u)*cos(k*u), sin(u));
}

DCoordinate3 rose::d1(GLdouble u)
{
    return DCoordinate3(-k*cos(u)*sin(k*u) -sin(u)*cos(k*u), cos(u)*cos(k*u) - sin(u)*k*sin(k*u) , cos(u));
}

DCoordinate3 rose::d2(GLdouble u)
{
    GLdouble c = cos(u), s = sin(u), ck = cos(u*k),sk=sin(u*k);
    return DCoordinate3(2*k*s*sk+(-k*k-1)*c*ck,(-k*k-1)*s*ck-2*k*c*sk, -s);
}

GLdouble ellipse::u_min = -2*TWO_PI;
GLdouble ellipse::u_max = +2*TWO_PI;

GLdouble ellipse::a = 1;
GLdouble ellipse::b = 2;

DCoordinate3 ellipse::d0(GLdouble u)
{
    return DCoordinate3( a*cos(u),  b*sin(u), sin(u));
}

DCoordinate3 ellipse::d1(GLdouble u)
{
    return DCoordinate3(-a*sin(u),b*cos(u),cos(u));
}

DCoordinate3 ellipse::d2(GLdouble u)
{
    return DCoordinate3(-a*cos(u),-b*sin(u),-sin(u));
}
//parametric surfaces
GLdouble surface1::u_min = 0;
GLdouble surface1::u_max = TWO_PI;
GLdouble surface1::v_min = 0;
GLdouble surface1::v_max = TWO_PI;
GLdouble surface1::r = 4;

DCoordinate3 surface1::d00(GLdouble u, GLdouble v){
      return DCoordinate3(r*cos(v),r*sin(v),u);
}
DCoordinate3 surface1::d01(GLdouble u, GLdouble v){
      return DCoordinate3(0,0,1);
}

DCoordinate3 surface1::d10(GLdouble u, GLdouble v){
      return DCoordinate3(-r*sin(v),r*cos(v),0);
}

GLdouble surface2::u_min = 0;
GLdouble surface2::u_max = 30;
GLdouble surface2::v_min = 0;
GLdouble surface2::v_max = 30;

DCoordinate3 surface2::d00(GLdouble u, GLdouble v){
      return DCoordinate3(u,v,cos(u+v));
}
DCoordinate3 surface2::d10(GLdouble u, GLdouble v){
      return DCoordinate3(1,0,-sin(u+v));
}

DCoordinate3 surface2::d01(GLdouble u, GLdouble v){
      return DCoordinate3(0,1,-sin(u+v));
}
GLdouble surface3::u_min = 0;
GLdouble surface3::u_max = TWO_PI;
GLdouble surface3::v_min = 0;
GLdouble surface3::v_max = PI;
GLdouble surface3::a = 2.5;

DCoordinate3 surface3::d00(GLdouble u, GLdouble v){
      return DCoordinate3(a*sin(v)*cos(u),a*sin(u)*sin(v),a*cos(v));
}
DCoordinate3 surface3::d01(GLdouble u, GLdouble v){
      return DCoordinate3(-sin(u)*sin(v)*a,a*cos(u)*sin(v),0);
}
DCoordinate3 surface3::d10(GLdouble u, GLdouble v){
      return DCoordinate3(a*cos(v)*cos(u),a*cos(v)*sin(u),-a*sin(v));
}
GLdouble surface4::u_min = 1;
GLdouble surface4::u_max = PI;
GLdouble surface4::v_min = 1;
GLdouble surface4::v_max = TWO_PI;

DCoordinate3 surface4::d00(GLdouble u, GLdouble v){
      return DCoordinate3(sin(u),sin(v),sin(u+v));
}
DCoordinate3 surface4::d01(GLdouble u, GLdouble v){
      return DCoordinate3(cos(u),0,cos(u+v));
}

DCoordinate3 surface4::d10(GLdouble u, GLdouble v){
      return DCoordinate3(0,cos(v),cos(u+v));
}

GLdouble surface5::u_min = 1;
GLdouble surface5::u_max = 30;
GLdouble surface5::v_min = 1;
GLdouble surface5::v_max = 30;

DCoordinate3 surface5::d00(GLdouble u, GLdouble v){
      return DCoordinate3(u,v,cos(v/u));
}
DCoordinate3 surface5::d10(GLdouble u, GLdouble v){
      return DCoordinate3(1,0,sin(v/u)*(v/(u*u)));
}

DCoordinate3 surface5::d01(GLdouble u, GLdouble v){
      return DCoordinate3(0,1,-sin(v/u)/u);
}
