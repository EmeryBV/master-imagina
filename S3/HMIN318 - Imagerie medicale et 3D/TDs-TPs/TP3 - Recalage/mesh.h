#include "CImg.h"

using namespace cimg_library;

struct MESH
{
    CImgList<unsigned int> faces;
    CImg<float> vertices;
    CImg<float> vertices0;
    CImg<float> correspondences;
    CImg<float> normals;
    CImg<float> weights;

    inline unsigned int getNbPoints() const {  return vertices.width(); }
    void getPoint0(float p[3], const unsigned int index) const {  for(unsigned int i=0;i<3;i++) p[i]=vertices0(index,i); }
    void getPoint(float p[3], const unsigned int index) const {  for(unsigned int i=0;i<3;i++) p[i]=vertices(index,i); }
    void setPoint(const float p[3], const unsigned int index) {  for(unsigned int i=0;i<3;i++) vertices(index,i)=p[i]; }
    void getCorrespondence(float p[3], const unsigned int index) const {  for(unsigned int i=0;i<3;i++) p[i]=correspondences(index,i); }
    void setCorrespondence(const float p[3], const unsigned int index) {  for(unsigned int i=0;i<3;i++) correspondences(index,i)=p[i]; }
    void getNormal(float n[3], const unsigned int index) const {  for(unsigned int i=0;i<3;i++) n[i]=normals(index,i); }
    float getWeight(const unsigned int index) const {  return weights(index); }
    void setWeight(const float w, const unsigned int index) {  weights(index)=w; }

    void drawMesh(CImg<unsigned char> &visu, CImg<float> &zbuffer, const unsigned char color[3], const float opacity, const float &focale, const CImg<float> &pose, const int &renderMode, const float &Xoff , const float &Yoff , const float &Zoff , const float &sprite_scale ) const;
    void drawCorrespondences(CImg<unsigned char> &visu, CImg<float> &zbuffer, const unsigned char color[3], const float opacity, const float &focale, const CImg<float> &pose, const int &renderMode, const float &Xoff , const float &Yoff , const float &Zoff , const float &sprite_scale );
    void drawNormals(const float &scale, CImg<unsigned char> &visu, CImg<float> &zbuffer, const unsigned char color[3], const float opacity, const float &focale, const CImg<float> &pose, const int &renderMode, const float &Xoff , const float &Yoff , const float &Zoff , const float &sprite_scale );
    bool LoadObj(const char* filename);
    void updateNormals();

    // returns the intersection points between a face i and a slice oriented along direction "dir"
    void getTrace(const unsigned int dir, const float slicePos, CImg<float> &intersectionPoints,     CImgList<unsigned int> &intersectionEdges) const;

};
