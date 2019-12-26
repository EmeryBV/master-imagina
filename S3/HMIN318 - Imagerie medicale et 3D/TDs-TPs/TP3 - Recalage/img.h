#include "CImg.h"
using namespace cimg_library;

template<typename T,typename Real>
class IMG
{
public:

    CImg<T> img;
    CImgList< Real > gradient;

    Real voxelSize[3];
    Real translation[3];
    Real affine[3][3];
    Real affineinv[3][3];

    IMG()
    {
        for(unsigned int i=0;i<3;i++)
        {
            voxelSize[i]=1;
            translation[i]=0;
            for(unsigned int j=0;j<3;j++) affine[i][j]=(i==j)?1:0;
        }
    }

    CImg<T>& operator()()    { return img; }

    void load_metaimage(const char *  headerFilename);
    void drawImage3d(CImg<unsigned char> &visu, CImg<float> &zbuffer, const int slice[3],const float opacity, const float &focale, const CImg<float> &pose, const int &renderMode, const float &Xoff , const float &Yoff , const float &Zoff , const float &sprite_scale );
    void updateGradient(float sigma) { gradient=img.get_blur(sigma).get_gradient(); }

    void toImage(int pi[3],const Real p[3]);
    void fromImage(Real p[3],const int pi[3]);

    T getValue(const Real p[3],const unsigned int interpolationType=1) const;
    void getGradient(Real grad[3], const Real p[3],const unsigned int interpolationType=1) const;

};
