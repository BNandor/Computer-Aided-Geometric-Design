#pragma once

#include "Colors4.h"
#include "HCoordinates3.h"
#include <GL/glew.h>

namespace cagd
{
    class DirectionalLight
    {
    protected:
        GLenum       _light_index;
        HCoordinate3 _position;
        Color4       _ambient_intensity, _diffuse_intensity, _specular_intensity;

    public:
        DirectionalLight(
            GLenum              light_index,
            const HCoordinate3& position,
            const Color4&       ambient_intensity,
            const Color4&       diffuse_intensity,
            const Color4&       specular_intensity);

        void Enable();
        void Disable();
    };

    class PointLight: public DirectionalLight
    {
    protected:
        GLfloat _constant_attenuation,
                _linear_attenuation,
                _quadratic_attenuation;


    public:
        PointLight(
            GLenum              light_index,
            const HCoordinate3& position,
            const Color4&       ambient_intensity,
            const Color4&       diffuse_intensity,
            const Color4&       specular_intensity,
            GLfloat             constant_attenuation,
            GLfloat             linear_attenuation,
            GLfloat             quadratic_attenuation);
    };

    class Spotlight: public PointLight
    {
    private:
        HCoordinate3 _spot_direction;
        GLfloat      _spot_cutoff, _spot_exponent;

    public:
        Spotlight(
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
            GLfloat             spot_exponent);
    };
}
