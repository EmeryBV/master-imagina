#include "gmap.hpp"
/*------------------------------------------------------------------------*/

GMap::GMap() : maxid(0) {}
GMap::~GMap() { }

GMap::idlist_t GMap::darts() const
{
    idlist_t mdarts;
    for(idalphamap_t::const_iterator it = alphas.begin(); it != alphas.end(); ++it)
        mdarts.push_back(it->first);
    return mdarts;
}

void GMap::print_alphas() {
    for(id_t d : darts())
    {
        printf("%lu | %lu %lu %lu\n", d, alpha(0,d), alpha(1,d), alpha(2,d));
    }        
}

GMap::idlist_t GMap::elements( degree_t degree) {
    idlist_t elements;
    idlist_t vdarts = darts();
    idset_t sdarts (vdarts.begin(), vdarts.end());
 
    degreelist_t  list_of_alpha_value = {0, 1, 2};
    list_of_alpha_value.erase(list_of_alpha_value.begin()+degree);
 
    while (!sdarts.empty()){
        id_t dart = *sdarts.begin();
        sdarts.erase(sdarts.begin());
        idlist_t elementi = orbit(list_of_alpha_value, dart);
        for (id_t d : elementi) sdarts.erase(d);
        elements.push_back(dart);
    }
 
    return elements;
}


GMap::idlist_t GMap::add_square() {
    idlist_t vdarts;
    for (unsigned char i = 0; i < 8; ++i) { vdarts.push_back(add_dart()); }
    for (unsigned char i = 0; i < 4; ++i)
        link_darts(0, vdarts[2*i], vdarts[2*i+1]);
    for (unsigned char i = 0; i < 4; ++i)
        link_darts(1, vdarts[2*i+1], vdarts[(2*i+2) % 8]);
    return vdarts;    
}

GMap GMap::square() {
    GMap m;
    m.add_square();
    return m;
}

/*------------------------------------------------------------------------*/




/*------------------------------------------------------------------------*/

GMap3D::GMap3D() {}
GMap3D::~GMap3D() { }


void GMap3D::print_alphas() {
    for(id_t d : darts())
    {
        printf("%lu | %lu %lu %lu", d, alpha(0,d), alpha(1,d), alpha(2,d));
        if(properties.count(d) == 1 ) {
            vec3_t pos = get_position(d);
            printf(" : (%f,%f,%f)", pos[0], pos[1], pos[2]);
        }
        printf("\n");
    }        
}


vec3_t GMap3D::element_center(degree_t degree, id_t dart){
    degreelist_t  list_of_alpha_value = {0, 1, 2};
    list_of_alpha_value.erase(list_of_alpha_value.begin()+degree);

    vec3_t center;
    float count = 0;
    for (id_t d : orbit(list_of_alpha_value, dart)) {
        center += get_position(d);
        count += 1;
    }

    return center / count;
}


GMap3D GMap3D::square(float xsize, float ysize, float height)
{
    GMap3D gmap;
    idlist_t vdarts = gmap.add_square();

    gmap.set_position(0, vec3_t(xsize,  ysize,  height));
    gmap.set_position(2, vec3_t(xsize,  -ysize, height));
    gmap.set_position(4, vec3_t(-xsize, -ysize, height));
    gmap.set_position(6, vec3_t(-xsize, ysize,  height));

    return gmap;
}

GMap3D GMap3D::cube(float xsize, float ysize, float zsize)
{
    GMap3D gmap;
    std::vector<idlist_t> squares;
    for (unsigned char i = 0; i < 6; ++i)
        squares.push_back(gmap.add_square());
 
    // sew top square to lateral squares
    gmap.sew_dart(2, squares[0][0], squares[1][1] );
    gmap.sew_dart(2, squares[0][2], squares[4][1] );
    gmap.sew_dart(2, squares[0][4], squares[3][1] );
    gmap.sew_dart(2, squares[0][6], squares[2][1] );
 
    // sew bottom square to lateral squares
    gmap.sew_dart(2, squares[5][0], squares[1][5] );
    gmap.sew_dart(2, squares[5][2], squares[2][5] );
    gmap.sew_dart(2, squares[5][4], squares[3][5] );
    gmap.sew_dart(2, squares[5][6], squares[4][5] );
 
    // sew lateral squares between each other
    gmap.sew_dart(2, squares[1][2], squares[2][7] );
    gmap.sew_dart(2, squares[2][2], squares[3][7] );
    gmap.sew_dart(2, squares[3][2], squares[4][7] );
    gmap.sew_dart(2, squares[4][2], squares[1][7] );
 
    gmap.set_position(squares[0][0], vec3_t(xsize,  ysize,  zsize));
    gmap.set_position(squares[0][2], vec3_t(xsize,  -ysize, zsize));
    gmap.set_position(squares[0][4], vec3_t(-xsize, -ysize, zsize));
    gmap.set_position(squares[0][6], vec3_t(-xsize, ysize,  zsize));

    gmap.set_position(squares[5][0], vec3_t(xsize,  ysize,  -zsize));
    gmap.set_position(squares[5][2], vec3_t(xsize,  -ysize, -zsize));
    gmap.set_position(squares[5][4], vec3_t(-xsize, -ysize, -zsize));
    gmap.set_position(squares[5][6], vec3_t(-xsize, ysize,  -zsize));

    return gmap;
}

GMap3D GMap3D::holeshape(float xsize, float ysize, float zsize, float internalratio)
{
    assert(0 < internalratio && internalratio < 1);
 
    GMap3D gmap;
    std::vector<idlist_t> squares;
    for (unsigned char i = 0; i < 16; ++i)
        squares.push_back(gmap.add_square());
 
    // sew upper squares between each other
    gmap.sew_dart(2, squares[0][2], squares[1][1] );
    gmap.sew_dart(2, squares[1][4], squares[2][3] );
    gmap.sew_dart(2, squares[2][6], squares[3][5] );
    gmap.sew_dart(2, squares[3][0], squares[0][7] );
 
    // sew upper squares with external lateral
    gmap.sew_dart(2, squares[0][0], squares[8][1] );
    gmap.sew_dart(2, squares[1][2], squares[9][1] );
    gmap.sew_dart(2, squares[2][4], squares[10][1] );
    gmap.sew_dart(2, squares[3][6], squares[11][1] );
 
    // sew upper squares with internal lateral
    gmap.sew_dart(2, squares[0][5], squares[12][0] );
    gmap.sew_dart(2, squares[1][7], squares[13][0] );
    gmap.sew_dart(2, squares[2][1], squares[14][0] );
    gmap.sew_dart(2, squares[3][3], squares[15][0] );
 
    // sew lower squares between each other
    gmap.sew_dart(2, squares[4][6], squares[5][1] );
    gmap.sew_dart(2, squares[5][4], squares[6][7] );
    gmap.sew_dart(2, squares[6][2], squares[7][5] );
    gmap.sew_dart(2, squares[7][0], squares[4][3] );
 
    // sew lower squares with external lateral
    gmap.sew_dart(2, squares[4][0], squares[8][5] );
    gmap.sew_dart(2, squares[5][6], squares[9][5] );
    gmap.sew_dart(2, squares[6][4], squares[10][5] );
    gmap.sew_dart(2, squares[7][2], squares[11][5] );
 
    // sew lower squares with internal lateral
    gmap.sew_dart(2, squares[4][5], squares[12][4] );
    gmap.sew_dart(2, squares[5][3], squares[13][4] );
    gmap.sew_dart(2, squares[6][1], squares[14][4] );
    gmap.sew_dart(2, squares[7][7], squares[15][4] );
 
    // sew external lateral squares between each other
    gmap.sew_dart(2, squares[8][7], squares[9][2] );
    gmap.sew_dart(2, squares[9][7], squares[10][2] );
    gmap.sew_dart(2, squares[10][7], squares[11][2] );
    gmap.sew_dart(2, squares[11][7], squares[8][2] );
 
    // sew internal lateral squares between each other
    gmap.sew_dart(2, squares[12][2], squares[13][7] );
    gmap.sew_dart(2, squares[13][2], squares[14][7] );
    gmap.sew_dart(2, squares[14][2], squares[15][7] );
    gmap.sew_dart(2, squares[15][2], squares[12][7] );

    gmap.set_position(squares[0][0], vec3_t(xsize,  ysize,  zsize));
    gmap.set_position(squares[1][2], vec3_t(xsize,  -ysize, zsize));
    gmap.set_position(squares[2][4], vec3_t(-xsize, -ysize, zsize));
    gmap.set_position(squares[3][6], vec3_t(-xsize, ysize,  zsize));

    gmap.set_position(squares[0][5], vec3_t(xsize*internalratio,  ysize*internalratio,  zsize));
    gmap.set_position(squares[1][7], vec3_t(xsize*internalratio,  -ysize*internalratio, zsize));
    gmap.set_position(squares[2][1], vec3_t(-xsize*internalratio, -ysize*internalratio, zsize));
    gmap.set_position(squares[3][3], vec3_t(-xsize*internalratio, ysize*internalratio,  zsize));

    gmap.set_position(squares[4][1], vec3_t(xsize,  ysize,  -zsize));
    gmap.set_position(squares[5][7], vec3_t(xsize,  -ysize, -zsize));
    gmap.set_position(squares[6][5], vec3_t(-xsize, -ysize, -zsize));
    gmap.set_position(squares[7][3], vec3_t(-xsize, ysize,  -zsize));

    gmap.set_position(squares[4][4], vec3_t(xsize*internalratio,  ysize*internalratio,  -zsize));
    gmap.set_position(squares[5][2], vec3_t(xsize*internalratio,  -ysize*internalratio, -zsize));
    gmap.set_position(squares[6][0], vec3_t(-xsize*internalratio, -ysize*internalratio, -zsize));
    gmap.set_position(squares[7][6], vec3_t(-xsize*internalratio, ysize*internalratio,  -zsize));
 
    return gmap;
}

/*------------------------------------------------------------------------*/
