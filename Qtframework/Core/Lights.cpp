#include "Exceptions.h"
#include "Lights.h"

using namespace cagd;

// directional light
DirectionalLight::DirectionalLight(
    GLenum              light_index,
    const HCoordinate3& position,
    const Color4&       ambient_intensity,
    const Color4&       diffuse_intensity,
    const Color4&       specular_intensity):

    _light_index(light_index),
    _position(position),
    _ambient_intensity(ambient_intensity),
    _diffuse_intensity(diffuse_intensity),
    _specular_intensity(specular_intensity)
{
    glLightfv(light_index, GL_POSITION, &_position.x());
    glLightfv(light_index, GL_AMBIENT,  &_ambient_intensity.r());
    glLightfv(light_index, GL_DIFFUSE,  &_diffuse_intensity.r());
    glLightfv(light_index, GL_SPECULAR, &_specular_intensity.r());
}

void DirectionalLight::Enable()
{
    glEnable(_light_index);
}

void DirectionalLight::Disable()
{
    glDisable(_light_index);
}

// point light
PointLight::PointLight(
    GLenum              light_index,
    const HCoordinate3& position,
    const Color4&       ambient_intensity,
    const Color4&       diffuse_intensity,
    const Color4&       specular_intensity,
    GLfloat             constant_attenuation,
    GLfloat             linear_attenuation,
    GLfloat             quadratic_attenuation):

    DirectionalLight(
            light_index,
            position,
            ambient_intensity, diffuse_intensity, specular_intensity),
    _constant_attenuation(constant_attenuation),
    _linear_attenuation(linear_attenuation),
    _quadratic_attenuation(quadratic_attenuation)
{
    if (position.w() == 0.0)
        throw Exception("PointLight::PointLight - Wrong position.");

    glLightf(_light_index, GL_SPOT_CUTOFF, 180.0);
    glLightf(_light_index, GL_CONSTANT_ATTENUATION,  _constant_attenuation);
    glLightf(_light_index, GL_LINEAR_ATTENUATION,    _linear_attenuation);
    glLightf(_light_index, GL_QUADRATIC_ATTENUATION, _quadratic_attenuation);
}

// spotlight
Spotlight::Spotlight(
    GLenum              light_index,
    const HCoordinate3& position,
    const Color4&       ambient_intensity,
    const Color4&       diffuse_intensity,
    const Color4&       specular_intensity,
    GLfloat             constant_attenuation,
    GLfloat             linear_attenuation,
    GLfloat             quadratic_attenuation,
    const HCoordinate3& spot_direction,
    GLfloat             spot_cutoff,
    GLfloat             spot_exponent):
    PointLight(
            light_index, position,
            ambient_intensity, diffuse_intensity, specular_intensity,
            constant_attenuation, linear_attenuation, quadratic_attenuation),
    _spot_direction(spot_direction),
    _spot_cutoff(spot_cutoff),
    _spot_exponent(spot_exponent)
{
    if (position.w() == 0.0)
        throw Exception("Spotlight::Spotlight - Wrong position.");

    if (_spot_cutoff > 90.0)
        throw Exception("Spotlight::Spotlight - Wrong spot cutoff.");

    glLightfv(_light_index, GL_SPOT_DIRECTION,	&_spot_direction.x());
    glLightf (_light_index, GL_SPOT_CUTOFF,     _spot_cutoff);
    glLightf (_light_index, GL_SPOT_EXPONENT,	_spot_exponent);
}
