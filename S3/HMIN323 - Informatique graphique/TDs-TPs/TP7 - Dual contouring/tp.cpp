// -------------------------------------------
// gMini : a minimal OpenGL/GLUT application
// for 3D graphics.
// Copyright (C) 2006-2008 Tamy Boubekeur
// All rights reserved.
// -------------------------------------------

// -------------------------------------------
// Disclaimer: this code is dirty in the
// suming that there is no attention paid to
// proper class attribute access, memory
// management or optimisation of any kind. It
// is designed for quick-and-dirty testing
// purpose.
// -------------------------------------------

#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cmath>

#include <algorithm>
#include <GL/glut.h>
#include <float.h>
#include "src/Vec3.h"
#include "src/Camera.h"
#include "src/jmkdtree.h"


std::vector< Vec3 > positions;
std::vector< Vec3 > normals;

std::vector< Vec3 > positions2;
std::vector< Vec3 > normals2;


std::vector< Vec3 > meshPositions;
std::vector< unsigned int > meshTriangles;

bool showMesh = true;

// -------------------------------------------
// OpenGL/GLUT application code.
// -------------------------------------------

static GLint window;
static unsigned int SCREENWIDTH = 640;
static unsigned int SCREENHEIGHT = 480;
static Camera camera;
static bool mouseRotatePressed = false;
static bool mouseMovePressed = false;
static bool mouseZoomPressed = false;
static int lastX=0, lastY=0, lastZoom=0;
static bool fullScreen = false;


Vec3 projectOnPlane(Vec3 const & x, Vec3 const & p, Vec3 const & n) {
    return x - Vec3::dot( x - p , n ) * n / n.length();
}

#define GAUSSIEN 1
#define WENDLAND 2
#define SINGULIER 3

double HPSS_weight(double r,  double h, unsigned short kernel_type = 0)
{
    switch(kernel_type){

        case GAUSSIEN: // Approximation
            return exp( (-r) / (h) );
            break;
        case WENDLAND: // Approximation
            return pow( (1 - (sqrt(r) / sqrt(h)) ), 4) * (1 + 4 * (sqrt(r) / sqrt(h)) );
            break;
        case SINGULIER: // Interpolation
            return pow( (sqrt(h) / sqrt(r)), 1.0);
            break;
        default:
            std::cerr << "Error : bad kernel type (1 = GAUSSIEN or 2 = WENDLAND or 3 = SINGULIER)" << std::endl;
            return 0;
            break;
    }
}

void HPSS(Vec3 inputPoint, Vec3 & outputPoint, Vec3 & outputNormal, std::vector<Vec3> const & positions,
            std::vector<Vec3> const & normals, BasicANNkdTree const & kdtree,
            unsigned short kernel_type = GAUSSIEN, float radius = 1, unsigned int nbIterations = 10, unsigned int knn = 20)
{
    outputPoint = inputPoint;

    ANNidxArray id_nearest_neighbors = new ANNidx[knn];
    ANNdistArray square_distances_to_neighbors = new ANNdist[knn];

    for(unsigned int it = 0 ; it < nbIterations; ++it) {
        
        Vec3 c = Vec3(0,0,0);
        outputNormal = Vec3(0,0,0);
        double swi = 0;

        kdtree.knearest(outputPoint, knn, id_nearest_neighbors, square_distances_to_neighbors);

        double max_dist = sqrt(square_distances_to_neighbors[knn - 1]);

        for(unsigned int i = 0 ; i < knn ; i++)
        {
            unsigned int nni_idx = id_nearest_neighbors[i];
            double nni_sqrDist = square_distances_to_neighbors[i];

            Vec3 ptilde = projectOnPlane(outputPoint, positions[nni_idx], normals[nni_idx]);

            double wi = exp(-sqrt(nni_sqrDist) / max_dist); // poids gaussien //weightKernel.weight( sqrt( nni_sqrDist ) , maxDist );
            
            c += wi * ptilde;
            outputNormal += wi * normals[nni_idx];
            swi += wi;
        }

        c /= swi;
        outputNormal.normalize();
        outputPoint = projectOnPlane(outputPoint, c, outputNormal);
    }

    delete[] id_nearest_neighbors;
    delete[] square_distances_to_neighbors;
}

struct GridPoint {
    double f;
};

struct GridCell {
    unsigned int idx_in_mesh_vertices;
};

struct Grid {
    // size of grid
    unsigned int nX,nY,nZ;

    // bounding box
    Vec3 bb , BB;

    // points (not centers of cells)
    std::vector< GridPoint > points;
    std::vector< GridCell > cells;


    Grid() {}
    Grid(unsigned int x,unsigned int y,unsigned int z , Vec3 _bb , Vec3 _BB){
        assert(x > 0 && y > 0 && z > 0);
        nX=x;
        nY=y;
        nZ=z;
        bb=_bb;
        BB=_BB;
        points.resize(nX*nY*nZ);
        cells.resize((nX-1)*(nY-1)*(nZ-1));
    }

    size_t pointSerializedIndex( unsigned int x , unsigned int y , unsigned int z ) const {
        return x + y*nX + z*nX*nY;
    }
    size_t cellSerializedIndex( unsigned int x , unsigned int y , unsigned int z ) const {
        return x + y*(nX-1) + z*(nX-1)*(nY-1);
    }

    GridPoint & point(unsigned int x,unsigned int y,unsigned int z){
        return points[ pointSerializedIndex(x,y,z) ];
    }
    GridCell & cell(unsigned int x,unsigned int y,unsigned int z){
        return cells[ cellSerializedIndex(x,y,z) ];
    }

    // return position of a point in the grid
    Vec3 position(float x,float y,float z){ // x \in [0, nX] , ...
        return Vec3( bb[0] + (x/nX) * (BB[0]-bb[0]) , bb[1] + (y/nY) * (BB[1]-bb[1]) , bb[2] + (z/nZ) * (BB[2]-bb[2]));
    }
};

// ------------------------------------------------------------------------------------------------------------
// i/o and some stuff
// ------------------------------------------------------------------------------------------------------------
void loadPN (const std::string & filename , std::vector< Vec3 > & o_positions , std::vector< Vec3 > & o_normals ) {
    unsigned int surfelSize = 6;
    FILE * in = fopen (filename.c_str (), "rb");
    if (in == NULL) {
        std::cout << filename << " is not a valid PN file." << std::endl;
        return;
    }
    size_t READ_BUFFER_SIZE = 1000; // for example...
    float * pn = new float[surfelSize*READ_BUFFER_SIZE];
    o_positions.clear ();
    o_normals.clear ();
    while (!feof (in)) {
        unsigned numOfPoints = fread (pn, 4, surfelSize*READ_BUFFER_SIZE, in);
        for (unsigned int i = 0; i < numOfPoints; i += surfelSize) {
            o_positions.push_back (Vec3 (pn[i], pn[i+1], pn[i+2]));
            o_normals.push_back (Vec3 (pn[i+3], pn[i+4], pn[i+5]));
        }

        if (numOfPoints < surfelSize*READ_BUFFER_SIZE) break;
    }
    fclose (in);
    delete [] pn;
}
void savePN (const std::string & filename , std::vector< Vec3 > const & o_positions , std::vector< Vec3 > const & o_normals ) {
    if ( o_positions.size() != o_normals.size() ) {
        std::cout << "The pointset you are trying to save does not contain the same number of points and normals." << std::endl;
        return;
    }
    FILE * outfile = fopen (filename.c_str (), "wb");
    if (outfile == NULL) {
        std::cout << filename << " is not a valid PN file." << std::endl;
        return;
    }
    for(unsigned int pIt = 0 ; pIt < o_positions.size() ; ++pIt) {
        fwrite (&(o_positions[pIt]) , sizeof(float), 3, outfile);
        fwrite (&(o_normals[pIt]) , sizeof(float), 3, outfile);
    }
    fclose (outfile);
}
void scaleAndCenter( std::vector< Vec3 > & io_positions ) {
    Vec3 bboxMin( FLT_MAX , FLT_MAX , FLT_MAX );
    Vec3 bboxMax( FLT_MIN , FLT_MIN , FLT_MIN );
    for(unsigned int pIt = 0 ; pIt < io_positions.size() ; ++pIt) {
        for( unsigned int coord = 0 ; coord < 3 ; ++coord ) {
            bboxMin[coord] = std::min<float>( bboxMin[coord] , io_positions[pIt][coord] );
            bboxMax[coord] = std::max<float>( bboxMax[coord] , io_positions[pIt][coord] );
        }
    }
    Vec3 bboxCenter = (bboxMin + bboxMax) / 2.f;
    float bboxLongestAxis = std::max<float>( bboxMax[0]-bboxMin[0] , std::max<float>( bboxMax[1]-bboxMin[1] , bboxMax[2]-bboxMin[2] ) );
    for(unsigned int pIt = 0 ; pIt < io_positions.size() ; ++pIt) {
        io_positions[pIt] = (io_positions[pIt] - bboxCenter) / bboxLongestAxis;
    }
}

void applyRandomRigidTransformation( std::vector< Vec3 > & io_positions , std::vector< Vec3 > & io_normals ) {
    srand(time(NULL));
    Mat3 R = Mat3::RandRotation();
    Vec3 t = Vec3::Rand(1.f);
    for(unsigned int pIt = 0 ; pIt < io_positions.size() ; ++pIt) {
        io_positions[pIt] = R * io_positions[pIt] + t;
        io_normals[pIt] = R * io_normals[pIt];
    }
}

void subsample( std::vector< Vec3 > & i_positions , std::vector< Vec3 > & i_normals , float minimumAmount = 0.1f , float maximumAmount = 0.2f ) {
    std::vector< Vec3 > newPos , newNormals;
    std::vector< unsigned int > indices(i_positions.size());
    for( unsigned int i = 0 ; i < indices.size() ; ++i ) indices[i] = i;
    srand(time(NULL));
    std::random_shuffle(indices.begin() , indices.end());
    unsigned int newSize = indices.size() * (minimumAmount + (maximumAmount-minimumAmount)*(float)(rand()) / (float)(RAND_MAX));
    newPos.resize( newSize );
    newNormals.resize( newSize );
    for( unsigned int i = 0 ; i < newPos.size() ; ++i ) {
        newPos[i] = i_positions[ indices[i] ];
        newNormals[i] = i_normals[ indices[i] ];
    }
    i_positions = newPos;
    i_normals = newNormals;
}

bool save( const std::string & filename , std::vector< Vec3 > & vertices , std::vector< unsigned int > & triangles ) {
    std::ofstream myfile;
    myfile.open(filename.c_str());
    if (!myfile.is_open()) {
        std::cout << filename << " cannot be opened" << std::endl;
        return false;
    }

    myfile << "OFF" << std::endl;

    unsigned int n_vertices = vertices.size() , n_triangles = triangles.size()/3;
    myfile << n_vertices << " " << n_triangles << " 0" << std::endl;

    for( unsigned int v = 0 ; v < n_vertices ; ++v ) {
        myfile << vertices[v][0] << " " << vertices[v][1] << " " << vertices[v][2] << std::endl;
    }
    for( unsigned int f = 0 ; f < n_triangles ; ++f ) {
        myfile << 3 << " " << triangles[3*f] << " " << triangles[3*f+1] << " " << triangles[3*f+2];
        myfile << std::endl;
    }
    myfile.close();
    return true;
}



// ------------------------------------------------------------------------------------------------------------
// rendering.
// ------------------------------------------------------------------------------------------------------------

void initLight () {
    GLfloat light_position1[4] = {22.0f, 16.0f, 50.0f, 0.0f};
    GLfloat direction1[3] = {-52.0f,-16.0f,-50.0f};
    GLfloat color1[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat ambient[4] = {0.3f, 0.3f, 0.3f, 0.5f};

    glLightfv (GL_LIGHT1, GL_POSITION, light_position1);
    glLightfv (GL_LIGHT1, GL_SPOT_DIRECTION, direction1);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, color1);
    glLightfv (GL_LIGHT1, GL_SPECULAR, color1);
    glLightModelfv (GL_LIGHT_MODEL_AMBIENT, ambient);
    glEnable (GL_LIGHT1);
    glEnable (GL_LIGHTING);
}

void init () {
    camera.resize (SCREENWIDTH, SCREENHEIGHT);
    initLight ();
    glCullFace (GL_BACK);
    glEnable (GL_CULL_FACE);
    glDepthFunc (GL_LESS);
    glEnable (GL_DEPTH_TEST);
    glClearColor (0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_COLOR_MATERIAL);
}



void drawTriangleMesh( std::vector< Vec3 > const & i_positions , std::vector< unsigned int > const & i_triangles ) {
    glBegin(GL_TRIANGLES);
    for(unsigned int tIt = 0 ; tIt < i_triangles.size() / 3 ; ++tIt) {
        Vec3 p0 = i_positions[3*tIt];
        Vec3 p1 = i_positions[3*tIt+1];
        Vec3 p2 = i_positions[3*tIt+2];
        Vec3 n = Vec3::cross(p1-p0 , p2-p0);
        n.normalize();
        glNormal3f( n[0] , n[1] , n[2] );
        glVertex3f( p0[0] , p0[1] , p0[2] );
        glVertex3f( p1[0] , p1[1] , p1[2] );
        glVertex3f( p2[0] , p2[1] , p2[2] );
    }
    glEnd();
}

void drawPointSet( std::vector< Vec3 > const & i_positions , std::vector< Vec3 > const & i_normals ) {
    glBegin(GL_POINTS);
    for(unsigned int pIt = 0 ; pIt < i_positions.size() ; ++pIt) {
        glNormal3f( i_normals[pIt][0] , i_normals[pIt][1] , i_normals[pIt][2] );
        glVertex3f( i_positions[pIt][0] , i_positions[pIt][1] , i_positions[pIt][2] );
    }
    glEnd();
}

void draw () {

    glColor3f(0.8, 0.8, 1); drawPointSet(positions , normals);

    glPointSize(2); // for example...
    // glPointSize(1);

    glColor3f(1, 0.1, 0.1); drawPointSet(positions2 , normals2);
}








void display () {
    glLoadIdentity ();
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera.apply ();
    draw ();
    glFlush ();
    glutSwapBuffers ();
}

void idle () {
    glutPostRedisplay ();
}

void key (unsigned char keyPressed, int x, int y) {
    switch (keyPressed) {
    case 'f':
        if (fullScreen == true) {
            glutReshapeWindow (SCREENWIDTH, SCREENHEIGHT);
            fullScreen = false;
        } else {
            glutFullScreen ();
            fullScreen = true;
        }
        break;

// new begin

    case 's':
        showMesh = !showMesh;
        break;


    case '-':
        camera.zoom (10.0f/SCREENHEIGHT);
        lastZoom = y;
        break;


    case '+':
        camera.zoom (-10.0f/SCREENHEIGHT);
        lastZoom = y;
        break;

// new end

    case 'w':
        GLint polygonMode[2];
        glGetIntegerv(GL_POLYGON_MODE, polygonMode);
        if(polygonMode[0] != GL_FILL)
            glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
        else
            glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
        break;

    default:
        break;
    }
    idle ();
}

void mouse (int button, int state, int x, int y) {
    if (state == GLUT_UP) {
        mouseMovePressed = false;
        mouseRotatePressed = false;
        mouseZoomPressed = false;
    } else {
        if (button == GLUT_LEFT_BUTTON) {
            camera.beginRotate (x, y);
            mouseMovePressed = false;
            mouseRotatePressed = true;
            mouseZoomPressed = false;
        } else if (button == GLUT_RIGHT_BUTTON) {
            lastX = x;
            lastY = y;
            mouseMovePressed = true;
            mouseRotatePressed = false;
            mouseZoomPressed = false;
        } else if (button == GLUT_MIDDLE_BUTTON) {
            if (mouseZoomPressed == false) {
                lastZoom = y;
                mouseMovePressed = false;
                mouseRotatePressed = false;
                mouseZoomPressed = true;
            }
        }
    }
    idle ();
}

void motion (int x, int y) {
    if (mouseRotatePressed == true) {
        camera.rotate (x, y);
    }
    else if (mouseMovePressed == true) {
        camera.move ((x-lastX)/static_cast<float>(SCREENWIDTH), (lastY-y)/static_cast<float>(SCREENHEIGHT), 0.0);
        lastX = x;
        lastY = y;
    }
    else if (mouseZoomPressed == true) {
        camera.zoom (float (y-lastZoom)/SCREENHEIGHT);
        lastZoom = y;
    }
}

void reshape(int w, int h) {
    camera.resize (w, h);
}


template<class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& array)
{
	for (const auto& v : array)
	{
		os << v << '\n';
	}

	return os;
}

auto Vec3_inf = [](const Vec3& a, const Vec3& b)
{
	if (a[0] < b[0])
	{
		return true;
	}
	else if (a[0] == b[0])
	{
		if (a[1] < b[1])
		{
			return true;
		}
		else if (a[1] == b[1])
		{
			return a[2] < b[2];
		}
	}

	return false;
};

// ------------------------------------------------------------------------------------------------------------
// DUAL CONTOURING
// ------------------------------------------------------------------------------------------------------------


template <typename T> int sgn(T val){
    return (T(0) < val) - (val < T(0));
}

bool sameSign(float f1, float f2, float f3, float f4){
    int sign1 = sgn(f1), sign2 = sgn(f2), sign3 = sgn(f3), sign4 = sgn(f4);

    return sign1 == sign2 && sign3 == sign4 && sign2 == sign3 && sign4 == sign1 && sign1 == sign3 && sign2 == sign4;
}

bool sameSign(float f1, float f2){
    int sign1 = sgn(f1), sign2 = sgn(f2);

    return sign1 == sign2;
}

// renvoie vrai si il y a un changement de signe entre les 8 points

bool computeCell(int x, int y, int z, Grid grid, Vec3 &pos, Vec3 &normal){

    pos = Vec3(0,0,0);
    normal = Vec3(0,0,0);

    int sign = sgn(grid.point(x,y,z).f);
    bool changed = false;

    for(unsigned int i=0; i<2; i++){
        for(unsigned int j=0; j<2; j++){
            for(unsigned int k=0; k<2; k++){
                int currentsign = sgn(grid.point(x+i,y+j,z+k).f);
                if(currentsign != sign){
                    changed = true;
                }

                pos += grid.position(x+i,y+j,z+k);
                // normal += normals[grid.cell(x+i,y+j,z+k).idx_in_mesh_vertices];
                normal += Vec3(1,2,1);


            }
        }
    }

    pos /= 8.0f;
    normal /= 8.0f;

    return changed;
}

int main (int argc, char ** argv) {
    if (argc > 2) {
        exit (EXIT_FAILURE);
    }
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize (SCREENWIDTH, SCREENHEIGHT);
    window = glutCreateWindow ("tp point processing");

    init ();
    glutIdleFunc (idle);
    glutDisplayFunc (display);
    glutKeyboardFunc (key);
    glutReshapeFunc (reshape);
    glutMotionFunc (motion);
    glutMouseFunc (mouse);
    key ('?', 0, 0);


    {
        // Load a first pointset, and build a kd-tree:
        loadPN("pointsets/igea.pn" , positions , normals);

        BasicANNkdTree kdtree;
        kdtree.build(positions);

        // Create a second pointset that is artificial, and project it on pointset1 using MLS techniques:
        positions2.resize( 2000 );
        normals2.resize(positions2.size());
        for( unsigned int pIt = 0 ; pIt < positions2.size() ; ++pIt ) {
            positions2[pIt] = Vec3(
                        -0.6 + 1.2 * (double)(rand())/(double)(RAND_MAX),
                        -0.6 + 1.2 * (double)(rand())/(double)(RAND_MAX),
                        -0.6 + 1.2 * (double)(rand())/(double)(RAND_MAX)
                        );
            positions2[pIt].normalize();
            positions2[pIt] = 0.6 * positions2[pIt];
        }

        // PROJECT USING MLS (HPSS and APSS):
        // TODO

        for (size_t i = 0 ; i < positions2.size() ; ++i)
        {
            HPSS(positions2[i], positions2[i], normals2[i], positions, normals, kdtree);
        }

        Vec3 gridbb = positions[0];
        Vec3 gridBB = positions[0];
        for( unsigned int pIt = 0 ; pIt < positions.size() ; ++pIt ) {
            for( unsigned int c = 0 ; c < 3 ; ++c ) {
                gridbb[c] = std::min(gridbb[c] , positions[pIt][c]);
                gridBB[c] = std::max(gridBB[c] , positions[pIt][c]);
            }
        }

        Vec3 gridCenter = (gridbb + gridBB) / 2.0;
        gridbb += 0.1 * (gridbb - gridCenter);
        gridBB += 0.1 * (gridBB - gridCenter);


        constexpr size_t grid_size = 32;

        Grid grid(grid_size, grid_size, grid_size, gridbb ,gridBB);
/*
        std::cout << "compute grid f" << std::endl;
        for( unsigned int x = 0 ; x < grid.nX ; ++x ) {
            for( unsigned int y = 0 ; y < grid.nY ; ++y ) {
                for( unsigned int z = 0 ; z < grid.nZ ; ++z ) {
                    Vec3 p = grid.position(x,y,z);
                    //TODO calculer grid.point(x,y,z).f
                }
            }
        }
*/

        std::cout << "compute grid f" << std::endl;
        for( unsigned int x = 0 ; x < grid.nX ; ++x ) {
            for( unsigned int y = 0 ; y < grid.nY ; ++y ) {
                for( unsigned int z = 0 ; z < grid.nZ ; ++z ) {
                    Vec3 p = grid.position(x,y,z);
                    Vec3 pi, ni;

                    HPSS(p ,pi, ni ,positions,normals ,kdtree,WENDLAND, 10,20);
                    // myHPSS(p ,pi, ni ,positions,normals ,kdtree,SINGULIER, 0.1, 10,20);

                    Vec3 point = p - pi;
                    point.normalize();
                    ni.normalize();

                    double isoVal = Vec3::dot(point, ni);
                    // std::cout << "isoVal : " << isoVal << "\n";



                    grid.point(x,y,z).f = isoVal;

                }
            }
        }

        positions2.resize(0);
        normals2.resize(0);


        std::cout << "Add point to grid\n";

        // on ajoute les points
        for( unsigned int x = 0 ; x < grid.nX-1 ; ++x ) {
            for( unsigned int y = 0 ; y < grid.nY-1 ; ++y ) {
                for( unsigned int z = 0 ; z < grid.nZ-1 ; ++z ) {
                    Vec3 pos, norm;

                    if(computeCell(x, y, z, grid, pos, norm)){
                        // GridCell cell;
                        // cell.idx_in_mesh_vertices = positions2.size();
                        positions2.push_back(pos);
                        normals2.push_back(norm);

                        //grid.cell(x, y, z) = cell;

                    }

                }
            }
        }

        std::cout << "Compute Edges\n";

        std::vector<std::array<int,2>> edges = std::vector<std::array<int,2>>();

        for( unsigned int x = 0 ; x < grid.nX-1 ; ++x ) {
            for( unsigned int y = 0 ; y < grid.nY-1 ; ++y ) {
                for( unsigned int z = 0 ; z < grid.nZ-1 ; ++z ) {
                    
                    if(!sameSign(grid.point(x,y,z).f, grid.point(x+1,y,z).f)){
                        std::array<int,2> ed;
                        ed[0] = grid.cell(x,y,z).idx_in_mesh_vertices; ed[1] = grid.cell(x+1,y,z).idx_in_mesh_vertices;
                        edges.push_back(ed);
                    }

                    if(!sameSign(grid.point(x,y,z).f, grid.point(x,y+1,z).f)){
                        std::array<int,2> ed;
                        ed[0] = grid.cell(x,y,z).idx_in_mesh_vertices; ed[1] = grid.cell(x,y+1,z).idx_in_mesh_vertices;
                        edges.push_back(ed);
                    }

                    if(!sameSign(grid.point(x,y,z).f, grid.point(x,y,z+1).f)){
                        std::array<int,2> ed;
                        ed[0] = grid.cell(x,y,z).idx_in_mesh_vertices; ed[1] = grid.cell(x,y,z+1).idx_in_mesh_vertices;
                        edges.push_back(ed);
                    }

                }
            }
        }
    }

    glutMainLoop ();
    return EXIT_SUCCESS;
}

