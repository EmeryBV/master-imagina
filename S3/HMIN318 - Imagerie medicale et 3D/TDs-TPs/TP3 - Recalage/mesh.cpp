#include <iostream>
#include <fstream>
#include <stdio.h>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>

#include "CImg.h"

#include "mesh.h"

using namespace cimg_library;
using namespace std;

void MESH::drawMesh(CImg<unsigned char> &visu, CImg<float> &zbuffer, const unsigned char color[3], const float opacity, const float &focale, const CImg<float> &pose, const int &renderMode, const float &Xoff , const float &Yoff , const float &Zoff , const float &sprite_scale ) const
{
    CImg<float> rotated_vertices(vertices.width(),3);
    cimg_forX(vertices,l)  for(unsigned int i=0;i<3;i++) rotated_vertices(l,i) = pose(0,i)*vertices(l,0) + pose(1,i)*vertices(l,1) + pose(2,i)*vertices(l,2) + pose(3,i);
    CImgList<unsigned char> colors;        colors.insert(faces.size(),CImg<unsigned char>::vector(color[0],color[1],color[2]));
    CImgList<float> opacities;   opacities.insert(faces.size(),CImg<float>::vector(opacity,opacity,opacity));

    const float light_x = 0,light_y = 0,light_z = -5000,specular_light = 0.2f,specular_shine = 0.1f; // rendering params
    visu._draw_object3d((void*)0,zbuffer,
                        Xoff + visu._width/2.0f,Yoff + visu._height/2.0f,Zoff,
                        rotated_vertices,faces,
                        colors,opacities,renderMode,1,focale,
                        visu.width()/2.0f+light_x,visu.height()/2.0f+light_y,light_z,specular_light,specular_shine,
                        sprite_scale);
}

void MESH::drawCorrespondences(CImg<unsigned char> &visu, CImg<float> &zbuffer, const unsigned char color[3], const float opacity, const float &focale, const CImg<float> &pose, const int &renderMode, const float &Xoff , const float &Yoff , const float &Zoff , const float &sprite_scale )
{
    if(correspondences.width()!=vertices.width()) correspondences=vertices;

    CImg<float> rotated_vertices(vertices.width()+correspondences.width(),3);
    cimg_forX(vertices,l)  for(unsigned int i=0;i<3;i++) rotated_vertices(l,i) = pose(0,i)*vertices(l,0) + pose(1,i)*vertices(l,1) + pose(2,i)*vertices(l,2) + pose(3,i);
    cimg_forX(correspondences,l)  if(weights(l)) {for(unsigned int i=0;i<3;i++) rotated_vertices((unsigned int)vertices.width()+l,i) = pose(0,i)*correspondences(l,0) + pose(1,i)*correspondences(l,1) + pose(2,i)*correspondences(l,2) + pose(3,i);} else {for(unsigned int i=0;i<3;i++) rotated_vertices((unsigned int)vertices.width()+l,i) = pose(0,i)*vertices(l,0) + pose(1,i)*vertices(l,1) + pose(2,i)*vertices(l,2) + pose(3,i);}
    CImgList<unsigned int> edges; cimg_forX(vertices,l) edges.insert(CImg<unsigned int>(1,2,1,1,l,(unsigned int)vertices.width()+l));

    CImg<unsigned char> palette=CImg<unsigned char>::HSV_LUT256 ();
    CImgList<unsigned char> colors;  cimg_forX(vertices,l)   { float col=weights(l)*256; if(col<0) col=0; if(col>255) col=255;  colors.insert(CImg<unsigned char>::vector(palette(0,(int)col,0,0),palette(0,(int)col,0,1),palette(0,(int)col,0,2))); }
    CImgList<float> opacities;   opacities.insert(edges.size(),CImg<float>::vector(opacity,opacity,opacity));

    const float light_x = 0,light_y = 0,light_z = -5000,specular_light = 0.2f,specular_shine = 0.1f; // rendering params
    visu._draw_object3d((void*)0,zbuffer,
                        Xoff + visu._width/2.0f,Yoff + visu._height/2.0f,Zoff,
                        rotated_vertices,edges,
                        colors,opacities,renderMode,1,focale,
                        visu.width()/2.0f+light_x,visu.height()/2.0f+light_y,light_z,specular_light,specular_shine,
                        sprite_scale);
}

void MESH::drawNormals(const float &scale, CImg<unsigned char> &visu, CImg<float> &zbuffer, const unsigned char color[3], const float opacity, const float &focale, const CImg<float> &pose, const int &renderMode, const float &Xoff , const float &Yoff , const float &Zoff , const float &sprite_scale )
{
    if(normals.width()!=vertices.width()) updateNormals();

    CImg<float> rotated_vertices(vertices.width()+normals.width(),3);
    cimg_forX(vertices,l)  for(unsigned int i=0;i<3;i++) rotated_vertices(l,i) = pose(0,i)*vertices(l,0) + pose(1,i)*vertices(l,1) + pose(2,i)*vertices(l,2) + pose(3,i);
    cimg_forX(normals,l)  for(unsigned int i=0;i<3;i++) rotated_vertices((unsigned int)vertices.width()+l,i) = pose(0,i)*(vertices(l,0)+scale*normals(l,0)) + pose(1,i)*(vertices(l,1)+scale*normals(l,1)) + pose(2,i)*(vertices(l,2)+scale*normals(l,2)) + pose(3,i);
    CImgList<unsigned int> edges; cimg_forX(vertices,l) edges.insert(CImg<unsigned int>(1,2,1,1,l,(unsigned int)vertices.width()+l));

    CImgList<unsigned char> colors;        colors.insert(edges.size(),CImg<unsigned char>::vector(color[0],color[1],color[2]));
    CImgList<float> opacities;   opacities.insert(edges.size(),CImg<float>::vector(opacity,opacity,opacity));

    const float light_x = 0,light_y = 0,light_z = -5000,specular_light = 0.2f,specular_shine = 0.1f; // rendering params
    visu._draw_object3d((void*)0,zbuffer,
                        Xoff + visu._width/2.0f,Yoff + visu._height/2.0f,Zoff,
                        rotated_vertices,edges,
                        colors,opacities,renderMode,1,focale,
                        visu.width()/2.0f+light_x,visu.height()/2.0f+light_y,light_z,specular_light,specular_shine,
                        sprite_scale);
}

bool MESH::LoadObj(const char* filename)
{
    FILE* file=fopen(filename,"rt"); if(!file) return false;
    char line[1024],*pChar; int iVert,iTCoord,iNormal;
    while (fgets(line,1024,file))
    {
        if (strncmp(line,"v ",2)==0) { float p[3]; if (sscanf(line, "v %f %f %f", p,p+1,p+2)==3) { vertices.append(CImg<float>(1,3,1,1,p[0],p[1],p[2]),'x'); }    }
        else if (strncmp(line,"f ",2)==0)
        {
            CImg<unsigned int> face;
            pChar = line + 2;
            const char *pEnd = line + strlen(line);
            while (pChar<pEnd)
            {
                while (*pChar==' ' && pChar<pEnd) pChar++;
                if (pChar<pEnd)
                {
                    if (sscanf(pChar,"%d/%d/%d",&iVert,&iTCoord,&iNormal)==3) face.append(CImg<unsigned int>(1,1,1,1,iVert-1,'x'));
                    else if (sscanf(pChar,"%d//%d",&iVert,&iNormal)==2) face.append(CImg<unsigned int>(1,1,1,1,iVert-1,'x'));
                    else if (sscanf(pChar,"%d/%d",&iVert,&iTCoord)==2)  face.append(CImg<unsigned int>(1,1,1,1,iVert-1,'x'));
                    else if (sscanf(pChar,"%d",&iVert)==1)  face.append(CImg<unsigned int>(1,1,1,1,iVert-1,'x'));
                    while (*pChar!=' ' && pChar<pEnd) pChar++;
                }
            }
            faces.insert(face);
        }
    }
    fclose(file);

    weights.resize(vertices.width(),1,1,1); weights.fill(1);
    correspondences=vertices;
    vertices0=vertices;
    updateNormals();

    return true;
}

void MESH::updateNormals()
{
    normals.resize(vertices.width(),3); normals.fill(0);
    for(unsigned int i=0;i<faces.size();i++)
        if(faces(i).width()>2)
        {
            float p1[3]; getPoint(p1,faces(i)(0));
            float p2[3]; getPoint(p2,faces(i)(1));
            float p3[3]; getPoint(p3,faces(i)(2));
            float u[3]; for(unsigned int j=0;j<3;j++)  u[j]=p2[j]-p1[j];
            float v[3]; for(unsigned int j=0;j<3;j++)  v[j]=p3[j]-p1[j];
            float w[3]={u[1]*v[2]-u[2]*v[1] , u[2]*v[0]-u[0]*v[2] , u[0]*v[1]-u[1]*v[0]};
            float nrm = sqrt(w[0]*w[0]+w[1]*w[1]+w[2]*w[2]); if(nrm) for(unsigned int j=0;j<3;j++)  w[j]/=nrm;
            for(int k=0;k<faces(i).width();k++) for(unsigned int j=0;j<3;j++) normals(faces(i)(k),j)+=w[j];
        }
    for(int i=0;i<normals.width();i++) {float nrm = sqrt(normals(i,0)*normals(i,0)+normals(i,1)*normals(i,1)+normals(i,2)*normals(i,2)); if(nrm) for(unsigned int j=0;j<3;j++)  normals(i,j)/=nrm;}
}

// returns the intersection points between faces and a slice oriented along direction "dir"
void MESH::getTrace(const unsigned int dir, const float slicePos, CImg<float> &intersectionPoints,     CImgList<unsigned int> &intersectionEdges) const
{
    for(unsigned int i=0;i<faces.size();i++)
    {
        unsigned int count=0;
        for(int k=0;k<faces(i).width();k++)
        {
            unsigned int index1=faces(i)(k),index2=faces(i)((k==0)?(faces(i).width()-1):(k-1));
            if(vertices(index2,dir)!=vertices(index1,dir))
            {
                float alpha=(vertices(index2,dir)-slicePos)/(vertices(index2,dir)-vertices(index1,dir));
                if(alpha>=0 && alpha<=1 && count<2)
                {
                    if(dir==0) intersectionPoints.append(CImg<float>(1,3,1,1,slicePos,alpha*vertices(index2,1)+(1.-alpha)*vertices(index1,1),alpha*vertices(index2,2)+(1.-alpha)*vertices(index1,2)),'x');
                    else if(dir==1) intersectionPoints.append(CImg<float>(1,3,1,1,alpha*vertices(index2,0)+(1.-alpha)*vertices(index1,0),slicePos,alpha*vertices(index2,2)+(1.-alpha)*vertices(index1,2)),'x');
                    else if(dir==2) intersectionPoints.append(CImg<float>(1,3,1,1,alpha*vertices(index2,0)+(1.-alpha)*vertices(index1,0),alpha*vertices(index2,1)+(1.-alpha)*vertices(index1,1),slicePos),'x');
                    count++;
                }
            }
        }
        if(intersectionPoints.width()>=2) intersectionEdges.insert(CImg<unsigned int>(2,1,1,1,intersectionPoints.width()-1,intersectionPoints.width()-2));
    }
}
