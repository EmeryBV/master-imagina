//////////////////////////////////////////////////////////////////////////////
//
//  --- Object.h ---
//  Created by Brian Summa
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "common.h"

#define EPSILON  1e-3

// Converti un vec4 en vec3
vec3 Vec3(vec4 v);

// Converti un vec3 en vec4
vec4 Vec4(vec3 v);

class Object{
public:
  
  std::string name;
  
  friend class Sphere;
  friend class Square;
  
  typedef struct{
    vec4 color;
    float Kd;
    float Ks;
    float Kn;
    float Kt;
    float Ka;
    float Kr;
  } ShadingValues;
  
  typedef struct{
    double t_o;
    double t_w;
    vec4 P_o;
    vec4 P_w;
    vec4 N_o;
    vec4 N_w;
    int ID_;
    std::string name;
  } IntersectionValues;
  
  
  Object(std::string name): name(name)  {};
  ~Object() {};
  
  Mesh mesh;
  ShadingValues shadingValues;
  
private:
  mat4 C;
  mat4 INVC;
  mat4 INVCStar;
  mat4 TRANINVC;
  
public:
  
  void setShadingValues(ShadingValues _shadingValues){shadingValues = _shadingValues;}
  
  void setModelView(mat4 modelview){
    C = modelview;
    INVC = invert(modelview);
    mat4 CStar = modelview;
    CStar[0][3] = 0;
    CStar[1][3] = 0;
    CStar[2][3] = 0;
    INVCStar = invert(CStar);
    TRANINVC = transpose(invert(modelview));
  }
  
  mat4 getModelView(){ return C; }
  
  virtual IntersectionValues intersect(vec4 p0, vec4 V)=0;
  
  
};

class Sphere : public Object{
public:
  
  Sphere(std::string name) : Object(name) { mesh.makeSubdivisionSphere(8); };
  
  virtual IntersectionValues intersect(vec4 p0_w, vec4 V_w);
  
private:
  double raySphereIntersection(vec4 p0, vec4 V, vec4 O=vec4(0.0, 0.0, 0.0, 1.0), double r=1.0);
  
};

class Square : public Object{
public:
  
  Square(std::string name) : Object(name) {
    
    mesh.vertices.resize(6);
    mesh.uvs.resize(6);
    mesh.normals.resize(6);
    
    mesh.vertices[0]=vec4(-1.0, -1.0, 0.0, 1.0);
    mesh.uvs[0] = vec2(0.0,0.0);
    mesh.vertices[1]=vec4(1.0, 1.0, 0.0, 1.0);
    mesh.uvs[1] = vec2(1.0,1.0);
    mesh.vertices[2]=vec4(1.0, -1.0, 0.0, 1.0);
    mesh.uvs[2] = vec2(1.0,0.0);
    
    mesh.vertices[3]=vec4(-1.0, -1.0, 0.0, 1.0);
    mesh.uvs[3] = vec2(0.0,0.0);
    mesh.vertices[4]=vec4(1.0, 1.0, 0.0, 1.0);
    mesh.uvs[4] = vec2(1.0,1.0);
    mesh.vertices[5]=vec4(-1.0, 1.0, 0.0, 1.0);
    mesh.uvs[5] = vec2(0.0,1.0);
    
    mesh.normals[0]= vec3(0, 0, 1.0);
    mesh.normals[1]= vec3(0, 0, 1.0);
    mesh.normals[2]= vec3(0, 0, 1.0);
    mesh.normals[3]= vec3(0, 0, 1.0);
    mesh.normals[4]= vec3(0, 0, 1.0);
    mesh.normals[5]= vec3(0, 0, 1.0);
    
  };
  
  virtual IntersectionValues intersect(vec4 p0_w, vec4 V_w);
  
private:
  double raySquareIntersection(vec4 p0, vec4 V);
  
};
#endif /* defined(__OBJECT_H__) */
