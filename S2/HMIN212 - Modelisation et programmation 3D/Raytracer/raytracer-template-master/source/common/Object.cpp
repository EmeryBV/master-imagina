//////////////////////////////////////////////////////////////////////////////
//
//  --- Object.cpp ---
//  Created by Brian Summa
//
//////////////////////////////////////////////////////////////////////////////


#include "common.h"


vec3 Vec3(vec4 v) {
  return vec3(v.x,v.y,v.z);
}

vec4 Vec4(vec3 v) {
  return vec4(v.x,v.y,v.z,0);
}

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
Object::IntersectionValues Sphere::intersect(vec4 p0_w, vec4 V_w){
  IntersectionValues result;
  result.name = this->name;

  //TODO: Ray-sphere setup
  
  // p0_w.w = 0;

  vec4 rayOrigin_o    = INVC * p0_w; 
  vec4 rayDir_o       = INVC * V_w;
  vec4 sphereCenter_o = vec4(0.0, 0.0, 0.0, 0.0);

  result.t_o = raySphereIntersection(rayOrigin_o, rayDir_o);
  rayOrigin_o.w = 0;

  result.P_o = rayOrigin_o + result.t_o * rayDir_o;
  result.N_o = result.P_o - sphereCenter_o;

  result.t_w = result.t_o;
  result.P_w = p0_w + result.t_w * V_w;
  result.N_w = C * result.N_o;

  /*
  static bool test = true;

  if (test) {
    std::cerr << this->name << "\n";
    std::cerr << "p0_w            : " << p0_w << "\n";
    std::cerr << "p0_w * C        : " << C * p0_w << "\n";
    std::cerr << "p0_w * INVC     : " << INVC * p0_w << "\n";
    std::cerr << "p0_w * INVCStar : " << INVCStar * p0_w << "\n";
    std::cerr << "V_w             : " << V_w << "\n";
    std::cerr << "V_w * C         : " << C * V_w << "\n";
    std::cerr << "V_w * INVC      : " << INVC * V_w << "\n";
    std::cerr << "V_w * INVCStar  : " << INVCStar * V_w << "\n";
    std::cerr << "length(V_w)         : " << length(V_w) << "\n";
    std::cerr << "length(V_w * INVC)  : " << length(INVC * V_w) << "\n";
    test = true;
  } else {
    test = false;
  } 

  */

  return result;
}



/* -------------------------------------------------------------------------- */
/* ------ Ray = p0 + t*V  sphere at origin O and radius r    : Find t ------- */
double Sphere::raySphereIntersection(vec4 p0, vec4 V, vec4 O, double r){
  double t = std::numeric_limits< double >::infinity();
  
  //TODO: Ray-sphere intersection;
  
  vec3 rayOrigin = Vec3(p0);
  vec3 rayDir = normalize(Vec3(V));
  vec3 centerToRay = Vec3(p0 - O);

/*
  vec4 rayOrigin = p0;
  vec4 rayDir = normalize(V);
  vec4 centerToRay = p0 - O;
*/
  double a = dot(rayDir, rayDir);
  double b = 2 * dot(centerToRay, rayDir);
  double c = dot(centerToRay, centerToRay) - (r * r);
  double discriminant = (b * b) - 4 * a * c;

  if (discriminant > EPSILON){
    t = (-b - sqrt(discriminant)) / (2.0 * length(V) * a);
  }

  return t;
}

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
Object::IntersectionValues Square::intersect(vec4 p0_w, vec4 V_w){
  IntersectionValues result;
  result.name = this->name;

  //TODO: Ray-square setup
  
  vec4 rayOrigin_o = INVC * p0_w;
  vec4 rayDir_o    = INVC * V_w;

  vec4 planeNormal_o = mesh.normals[0]; planeNormal_o.w = 0;

  result.t_o = raySquareIntersection(rayOrigin_o, rayDir_o);
  result.P_o = rayOrigin_o + result.t_o * rayDir_o;
  result.N_o = planeNormal_o;

  result.t_w = result.t_o;
  result.P_w = p0_w + result.t_w * V_w;
  result.N_w = C * result.N_o;

/*

  static bool test = true;

  if (test) {
    std::cerr << this->name << "\n";
    std::cerr << "p0_w            : " << p0_w << "\n";
    std::cerr << "p0_w * C        : " << C * p0_w << "\n";
    std::cerr << "p0_w * INVC     : " << INVC * p0_w << "\n";
    std::cerr << "p0_w * INVCStar : " << INVCStar * p0_w << "\n";
    std::cerr << "V_w             : " << V_w << "\n";
    std::cerr << "V_w * C         : " << C * V_w << "\n";
    std::cerr << "V_w * INVC      : " << INVC * V_w << "\n";
    std::cerr << "V_w * INVCStar  : " << INVCStar * V_w << "\n";
    std::cerr << "length(V_w)         : " << length(V_w) << "\n";
    std::cerr << "length(V_w * INVC)  : " << length(INVC * V_w) << "\n";
    test = true;
  } else {
    test = false;
  } 
*/
  return result;
}


/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
double Square::raySquareIntersection(vec4 p0, vec4 V){
  double t = std::numeric_limits< double >::infinity();

  vec3 a = Vec3(mesh.vertices[5]); //a.w = 0;// up left
  vec3 b = Vec3(mesh.vertices[1]); //b.w = 0;// up right
  vec3 c = Vec3(mesh.vertices[2]); //c.w = 0;// down right

  vec3 rayOrigin = Vec3(p0);
  vec3 rayDir = Vec3(V);

  vec3 planeNormal = cross(b - a, c - a); 

  vec3 originToA = a - rayOrigin;

  float denom = dot(planeNormal, rayDir);

  if (denom > EPSILON)
  {
    t = dot(originToA, planeNormal) / denom;

    if (t > EPSILON)
    {
      vec3 projOnPlane = rayOrigin + rayDir * t;
      
      if (projOnPlane.x < 1 && projOnPlane.x > -1 && projOnPlane.y < 1 && projOnPlane.y > -1)
      {
        return t;
      }
    }
  }

  return std::numeric_limits< double >::infinity();
}
