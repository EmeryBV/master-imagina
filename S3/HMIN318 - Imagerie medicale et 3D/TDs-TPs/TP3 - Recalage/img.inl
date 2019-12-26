#include <iostream>
#include <fstream>
#include <stdio.h>
#include <sstream>
#include <string>
#include <math.h>
using namespace std;

#include "img.h"
#include "mathematics.h"



template<typename T,typename Real>
void IMG<T,Real>::load_metaimage(const char *  headerFilename)
{
    std::ifstream fileStream(headerFilename, std::ifstream::in);
    if (!fileStream.is_open())	{	std::cout << "Can not open " << headerFilename << std::endl;	return; }

    std::string str,str2,imageFilename;
    unsigned int nbchannels=1,nbdims=4,dim[] = {1,1,1,1};
    std::string inputType(cimg::type<T>::string());
    while(!fileStream.eof())
    {
        fileStream >> str;

        if(!str.compare("ObjectType"))
        {
            fileStream >> str2; // '='
            fileStream >> str2;
            if(str2.compare("Image")) { std::cout << "MetaImageReader: not an image ObjectType "<<std::endl; return;}
        }
        else if(!str.compare("ElementDataFile"))
        {
            fileStream >> str2; // '='
            fileStream >> imageFilename;
        }
        else if(!str.compare("NDims"))
        {
            fileStream >> str2;  // '='
            fileStream >> nbdims;
            if(nbdims>4) { std::cout << "MetaImageReader: dimensions > 4 not supported  "<<std::endl; return;}
        }
        else if(!str.compare("ElementNumberOfChannels"))
        {
            fileStream >> str2;  // '='
            fileStream >> nbchannels;
        }
        else if(!str.compare("DimSize") || !str.compare("dimensions") || !str.compare("dim"))
        {
            fileStream >> str2;  // '='
            for(unsigned int i=0;i<nbdims;i++) fileStream >> dim[i];
        }
        else if(!str.compare("ElementSpacing") || !str.compare("spacing") || !str.compare("scale3d") || !str.compare("voxelSize"))
        {
            fileStream >> str2; // '='
            double val[4];
            for(unsigned int i=0;i<nbdims;i++) {fileStream >> val[i]; if(i<4) this->voxelSize[i] = (Real)val[i];}
        }
        else if(!str.compare("Position") || !str.compare("Offset") || !str.compare("translation") || !str.compare("origin"))
        {
            fileStream >> str2; // '='
            double val[4];
            for(unsigned int i=0;i<nbdims;i++) {fileStream >> val[i]; if(i<4) translation[i] = (Real)val[i];}
        }
        else if(!str.compare("Orientation"))
        {
            fileStream >> str2; // '='
            double val[4*4];
            for(unsigned int i=0;i<nbdims*nbdims;i++) fileStream >> val[i];
            for(unsigned int i=0;i<3;i++) if(i<nbdims) for(unsigned int j=0;j<3;j++) if(j<nbdims) affine[i][j] = (Real)val[i*nbdims+j];
            // to do: handle "CenterOfRotation" Tag
        }
        else if(!str.compare("ElementType") || !str.compare("voxelType"))  // not used (should be known in advance for template)
        {
            fileStream >> str2; // '='
            fileStream >> str2;

            if(!str2.compare("MET_CHAR"))           inputType=std::string("char");
            else if(!str2.compare("MET_DOUBLE"))    inputType=std::string("double");
            else if(!str2.compare("MET_Real"))     inputType=std::string("Real");
            else if(!str2.compare("MET_INT"))       inputType=std::string("int");
            else if(!str2.compare("MET_LONG"))      inputType=std::string("long");
            else if(!str2.compare("MET_SHORT"))     inputType=std::string("short");
            else if(!str2.compare("MET_UCHAR"))     inputType=std::string("unsigned char");
            else if(!str2.compare("MET_UINT"))      inputType=std::string("unsigned int");
            else if(!str2.compare("MET_ULONG"))     inputType=std::string("unsigned long");
            else if(!str2.compare("MET_USHORT"))    inputType=std::string("unsigned short");
            else if(!str2.compare("MET_BOOL"))      inputType=std::string("bool");

            if(inputType!=std::string(cimg::type<T>::string()))  std::cout<<"MetaImageReader: Image type ( "<< str2 <<" ) is converted to Image type ( "<< cimg::type<T>::string() <<" )"<<std::endl;
        }
    }
    fileStream.close();

    if(!imageFilename.size()) // no specified file name -> replace .mhd by .raw
    {
        imageFilename = std::string(headerFilename);
        imageFilename .replace(imageFilename.find_last_of('.')+1,imageFilename.size(),"raw");
    }
    else // add path to the specified file name
    {
        std::string tmp(headerFilename);
        std::size_t pos=tmp.find_last_of('/');
        if(pos==std::string::npos) pos=tmp.find_last_of('\\');
        if(pos!=std::string::npos) {tmp.erase(pos+1); imageFilename.insert(0,tmp);}
    }

    img.assign(dim[0],dim[1],dim[2],nbchannels);
    unsigned int nb = dim[0]*dim[1]*dim[2]*nbchannels;
    std::FILE *const nfile = std::fopen(imageFilename.c_str(),"rb");
    if(!nfile) return;

    if(inputType==std::string(cimg::type<T>::string()))
    {
        cimg::fread(img._data,nb,nfile);
    }
    else
    {
        if(inputType==std::string("char"))
        {
            char *const buffer = new char[nb];
            cimg::fread(buffer,nb,nfile);
            //if (endian) cimg::invert_endianness(buffer,nb);
            cimg_foroff(img,off) img._data[off] = (T)(buffer[off]);
            delete[] buffer;
        }
        else if(inputType==std::string("double"))
        {
            double *const buffer = new double[nb];
            cimg::fread(buffer,nb,nfile);
            //if (endian) cimg::invert_endianness(buffer,nb);
            cimg_foroff(img,off) img._data[off] = (T)(buffer[off]);
            delete[] buffer;
        }
        else if(inputType==std::string("Real"))
        {
            Real *const buffer = new Real[nb];
            cimg::fread(buffer,nb,nfile);
            //if (endian) cimg::invert_endianness(buffer,nb);
            cimg_foroff(img,off) img._data[off] = (T)(buffer[off]);
            delete[] buffer;
        }
        else if(inputType==std::string("int"))
        {
            int *const buffer = new int[nb];
            cimg::fread(buffer,nb,nfile);
            //if (endian) cimg::invert_endianness(buffer,nb);
            cimg_foroff(img,off) img._data[off] = (T)(buffer[off]);
            delete[] buffer;
        }
        else if(inputType==std::string("long"))
        {
            long *const buffer = new long[nb];
            cimg::fread(buffer,nb,nfile);
            //if (endian) cimg::invert_endianness(buffer,nb);
            cimg_foroff(img,off) img._data[off] = (T)(buffer[off]);
            delete[] buffer;
        }
        else if(inputType==std::string("short"))
        {
            short *const buffer = new short[nb];
            cimg::fread(buffer,nb,nfile);
            //if (endian) cimg::invert_endianness(buffer,nb);
            cimg_foroff(img,off) img._data[off] = (T)(buffer[off]);
            delete[] buffer;
        }
        else if(inputType==std::string("unsigned char"))
        {
            unsigned char *const buffer = new unsigned char[nb];
            cimg::fread(buffer,nb,nfile);
            //if (endian) cimg::invert_endianness(buffer,nb);
            cimg_foroff(img,off) img._data[off] = (T)(buffer[off]);
            delete[] buffer;
        }
        else if(inputType==std::string("unsigned int"))
        {
            unsigned int *const buffer = new unsigned int[nb];
            cimg::fread(buffer,nb,nfile);
            //if (endian) cimg::invert_endianness(buffer,nb);
            cimg_foroff(img,off) img._data[off] = (T)(buffer[off]);
            delete[] buffer;
        }
        else if(inputType==std::string("unsigned long"))
        {
            unsigned long *const buffer = new unsigned long[nb];
            cimg::fread(buffer,nb,nfile);
            //if (endian) cimg::invert_endianness(buffer,nb);
            cimg_foroff(img,off) img._data[off] = (T)(buffer[off]);
            delete[] buffer;
        }
        else if(inputType==std::string("unsigned short"))
        {
            unsigned short *const buffer = new unsigned short[nb];
            cimg::fread(buffer,nb,nfile);
            //if (endian) cimg::invert_endianness(buffer,nb);
            cimg_foroff(img,off) img._data[off] = (T)(buffer[off]);
            delete[] buffer;
        }
        else if(inputType==std::string("bool"))
        {
            bool *const buffer = new bool[nb];
            cimg::fread(buffer,nb,nfile);
            //if (endian) cimg::invert_endianness(buffer,nb);
            cimg_foroff(img,off) img._data[off] = (T)(buffer[off]);
            delete[] buffer;
        }
    }
    cimg::fclose(nfile);

    Invert(affineinv,affine);
}


template<typename T,typename Real>
void IMG<T,Real>::drawImage3d(CImg<unsigned char> &visu, CImg<float> &zbuffer,const int slice[3], const float opacity, const float &focale, const CImg<float> &pose, const int &renderMode, const float &Xoff , const float &Yoff , const float &Zoff , const float &sprite_scale )
{
    CImgList<unsigned char> colors;
    CImgList<unsigned int> faces;
    CImg<float> vertices = img.get_projections3d(faces,colors,slice[0],slice[1],slice[2],true);
    for(unsigned int l=0;l<colors.size();l++)
    {
        colors(l).resize(colors(l).width(),colors(l).height(),1,3);
        colors(l).get_channel(1)=colors(l).get_channel(0);
        colors(l).get_channel(2)=colors(l).get_channel(0);
    }
    cimg_forX(vertices,l)  { int P[3]={(int)vertices(l,0),(int)vertices(l,1),(int)vertices(l,2)}; float p[3]; this->fromImage(p,P); vertices(l,0)=p[0]; vertices(l,1)=p[1]; vertices(l,2)=p[2]; }

    CImg<float> rotated_vertices(vertices.width(),3);
    cimg_forX(vertices,l)  for(unsigned int i=0;i<3;i++) rotated_vertices(l,i) = pose(0,i)*vertices(l,0) + pose(1,i)*vertices(l,1) + pose(2,i)*vertices(l,2) + pose(3,i);
    CImgList<float> opacities;   opacities.insert(faces.size(),CImg<float>::vector(opacity,opacity,opacity));

    const float light_x = 0,light_y = 0,light_z = -5000,specular_light = 0.2f,specular_shine = 0.1f; // rendering params
    visu._draw_object3d((void*)0,zbuffer,
                        Xoff + visu._width/2.0f,Yoff + visu._height/2.0f,Zoff,
                        rotated_vertices,faces,
                        colors,opacities,renderMode,1,focale,
                        visu.width()/2.0f+light_x,visu.height()/2.0f+light_y,light_z,specular_light,specular_shine,
                        sprite_scale);
}



template<typename T,typename Real>
void IMG<T,Real>::toImage(int pi[3],const Real p[3])
{
    Real p2[3];  for(unsigned int i=0;i<3;i++) p2[i]=(Real)p[i]-translation[i];
    Real p3[3];  Mult(p3,affineinv,p2);
    for(unsigned int i=0;i<3;i++) pi[i]=round(p3[i]/voxelSize[i]);
}

template<typename T,typename Real>
void IMG<T,Real>::fromImage(Real p[3],const int pi[3])
{
    Real p2[3];  for(unsigned int i=0;i<3;i++) p2[i]=(Real)pi[i]*voxelSize[i];
    Real p3[3];  Mult(p3,affine,p2);
    for(unsigned int i=0;i<3;i++) p[i]=(p3[i]+translation[i]);
}


template<typename T,typename Real>
T IMG<T,Real>::getValue(const Real p[3],const unsigned int interpolationType) const
{
    Real p2[3];  for(unsigned int i=0;i<3;i++) p2[i]=(Real)p[i]-translation[i];
    Real p3[3];  Mult(p3,affineinv,p2);
    for(unsigned int i=0;i<3;i++) p3[i]/=voxelSize[i];

    T outval=(T)0;

    if(p3[0]<0 || p3[1]<0 || p3[2]<0 || p3[0]>=img.width() || p3[1]>=img.height() || p3[2]>=img.depth()) return outval;

    if(interpolationType==0) return (T)img.atXYZ(round(p3[0]),round(p3[1]),round(p3[2])); // nearest neighbor
    else if(interpolationType==1)  return (T)img.linear_atXYZ(p3[0],p3[1],p3[2],0,outval); //trilinear
    else  return (T)img.cubic_atXYZ(p3[0],p3[1],p3[2],0,(T)outval,cimg::type<T>::min(),cimg::type<T>::max()); // cubic
}

template<typename T,typename Real>
void IMG<T,Real>::getGradient(Real grad[3], const Real p[3],const unsigned int interpolationType) const
{
    if(!gradient.size()) return;

    Real p2[3];  for(unsigned int i=0;i<3;i++) p2[i]=(Real)p[i]-translation[i];
    Real p3[3];  Mult(p3,affineinv,p2);
    for(unsigned int i=0;i<3;i++) p3[i]/=voxelSize[i];

    Real g[3];
    Real outval=0;

    for(unsigned int j=0;j<3;j++) grad[j]=g[j]=outval;

    if(p3[0]<0 || p3[1]<0 || p3[2]<0 || p3[0]>=img.width() || p3[1]>=img.height() || p3[2]>=img.depth()) return;

    if(interpolationType==0) for(unsigned int j=0;j<3;j++) g[j]=gradient(j).atXYZ(round(p3[0]),round(p3[1]),round(p3[2])); // nearest neighbor
    else if(interpolationType==1)  for(unsigned int j=0;j<3;j++) g[j]=gradient(j).linear_atXYZ(p3[0],p3[1],p3[2],0,outval); //trilinear
    else  for(unsigned int j=0;j<3;j++) g[j]=gradient(j).cubic_atXYZ(p3[0],p3[1],p3[2],0,(T)outval,cimg::type<T>::min(),cimg::type<T>::max()); // cubic

    for(unsigned int i=0;i<3;i++) g[i]*=voxelSize[i];
    Mult(grad,affine,g);

    return;
}
