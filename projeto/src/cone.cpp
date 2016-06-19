#include "cone.h"
#include <cmath>

Cone::Cone(int _indice_textura, float _raio, float _altura, const Ponto_3D &_centro) : Objeto_3D (_indice_textura)
{
    raio = _raio;
    centro = _centro;
    altura = _altura;
}

Intersection Cone::Intercepta(const Raio &r_vis, Objeto_3D::IntersectionMode mode, float threshold)
{
    float a, b, c, delta, t0, t1, y0, y1;
    Intersection intersection;

    // valores intermediários
    Vetor_3D K = Vetor_3D(r_vis.X0() - centro.X(),
                          r_vis.Y0() - centro.Y(),
                          r_vis.Z0() - centro.Z());
//xE2+yE2-zE2
    a = r_vis.Dx() * r_vis.Dx() + r_vis.Dz() * r_vis.Dz() - r_vis.Dy() * r_vis.Dy();
    b = 2 * K.X() * r_vis.Dx() + 2 * K.Z() * r_vis.Dz() - 2 * K.Y() * r_vis.Dy();
    c = K.X() * K.X() + K.Z() * K.Z() - K.Y() * K.Y();

    delta = b * b - 4 * a * c;
    if (delta >= 0)
    {
        t0 = (-b + sqrt(delta)) / (2 * a);
        t1 = (-b - sqrt(delta)) / (2 * a);

        y0 = K.Y() + t0 * r_vis.Dy();
        y1 = K.Y() + t1 * r_vis.Dy();

        if ((centro.Y() - altura / 2 < y0 && y0 < centro.Y() + altura / 2) || (centro.Y() - altura / 2 < y1 && y1 < centro.Y() + altura / 2))
        {
            intersection = Intersection::nearest(Intersection(this, t0), Intersection(this, t1), threshold);
        }
    }

    return intersection;
}

Vetor_3D Cone::normal(const Ponto_3D &ponto) const
{
    Vetor_3D tmp;

    tmp = ponto - centro;
    tmp.normaliza();
    tmp.setY(0);

    return tmp;
}

TexturePoint Cone::pontoTextura(const Ponto_3D &ponto) const
{
    float s, t;
    Vetor_3D tmp;

    tmp = ponto - centro;
    tmp.normaliza();

    s = (atan2(tmp.X(), tmp.Z()) + M_PI) / (2 * M_PI);
    t = tmp.Y();

    return TexturePoint(s, t);
}
