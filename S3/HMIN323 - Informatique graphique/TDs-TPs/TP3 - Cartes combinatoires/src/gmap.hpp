
#include  <unordered_map>
#include  <unordered_set>
#include <vector>
#include <algorithm>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

typedef glm::vec3 vec3_t;

/*------------------------------------------------------------------------*/

// Build a topological GMap of degree 2
class GMap {
public:

    // id type
    typedef unsigned long int id_t;
    // degree type. should be in [0,1,2]
    typedef unsigned char degree_t;

    // a triplet of alpha value
    struct alpha_container_t {
        alpha_container_t(id_t a0 = 0, id_t a1 = 0, id_t a2 = 0) {
            values[0] = a0; values[1] = a1; values[2] = a2; 
        }
        id_t& operator[](degree_t index) { return values[index]; }
        const id_t& operator[](degree_t index) const { return values[index]; }

        // return alpha_container that has flipped value for alpha0 and alpha2.
        alpha_container_t flip() const { return alpha_container_t(values[2],values[1],values[0]); }

        id_t values[3];
    };

    // a type for list of id. usefull for processing
    typedef std::vector<id_t> idlist_t;
    // a type for set of id . usefull to check if darts have already been processed. 
    typedef std::unordered_set<id_t> idset_t;
    // a type of list of degree
    typedef std::vector<degree_t> degreelist_t;
    // a type of map of dart id to alpha values for this id.
    typedef std::unordered_map<id_t, alpha_container_t> idalphamap_t;


    GMap();
    ~GMap();

    // Return a list of id representing the darts of the structure
    idlist_t darts() const;

    // Return the application of the alpha_deg on dart
    id_t alpha(degree_t degree, id_t dart) const;

    // Return the application of a composition of alphas on dart
    id_t alpha(degreelist_t degrees, id_t dart) const;

    
    //  Test if dart is free for alpha_degree (if it is a fixed point) 
    bool is_free(degree_t degree, id_t dart) const;

    /*
        Test the validity of the structure. 
        Check that alpha_0 and alpha_1 are involutions with no fixed points.
        Check if alpha_0 o alpha_2 is an involution
    */
    bool is_valid() const;

    /* 
        Create a new dart and return its id. 
        Set its alpha_i to itself (fixed points) 
    */
    id_t add_dart();

    // Link the two darts with a relation alpha_degree if they are both free.
    bool link_darts(degree_t degree, id_t dart1, id_t dart2); 

    // Print for each dart, the value of the different alpha applications.
    void print_alphas();

    /* 
        Return the orbit of dart using a list of alpha relation.
        Example of use : gmap.orbit(0,[0,1]).
    */
    idlist_t orbit(degreelist_t alphas, id_t dart);

    /*
        Return the ordered orbit of dart using a list of alpha relations by applying
        repeatingly the alpha relations of the list to dart.
        Example of use. gmap.orderedorbit(0,[0,1]).
        Warning: No fixed point for the given alpha should be contained.
    */
    idlist_t orderedorbit(degreelist_t list_of_alpha_value, id_t dart);

    /* 
        Return one dart per element of degree. For this, consider all darts as initial set S. 
        Take the first dart d, remove from the set all darts of the orbit starting from d and 
        corresponding to element of degree degree. Take then next element from set S and do the 
        same until S is empty. 
        Return all darts d that were used. 
    */
    idlist_t elements( degree_t degree);

    /*
        Sew two elements of degree 'degree' that start at dart1 and dart2.
        Determine first the orbits of dart to sew and heck if they are compatible.
        Sew pairs of corresponding darts
        # and if they have different embedding  positions, merge them. 
    */
    bool sew_dart(degree_t degree, id_t dart1, id_t dart2);

    // Compute the Euler-Poincare characteristic of the subdivision
    int eulercharacteristic();


    idlist_t add_square();

    static GMap square() ;


protected:
    id_t maxid; // use to generate dart id
    idalphamap_t alphas; // a map with key corresponding to dart id and value a alpha_container_t that contains the 3 value of alpha for the given dart. example : { 0 : [0,1,2] }.
};

/*------------------------------------------------------------------------*/


template<class T>
class EmbeddedGMap : public GMap {
public:
    typedef T property_t;
    typedef std::unordered_map<id_t, property_t> idpropmap_t;

    EmbeddedGMap() {}
    ~EmbeddedGMap() {}

    /*
        Check if a dart of the orbit representing the vertex has already been 
        associated with a value in propertydict. If yes, return this dart, else
        return the dart passed as argument.
    */
    id_t get_embedding_dart(id_t dart) ;



    // Retrieve the coordinates associated to the vertex <alpha_1, alpha_2>(dart) 
    const T& get_property(id_t dart) {
        id_t d = get_embedding_dart(dart);
        if (properties.count(d) == 0) { throw d; }
        return properties[d];
    }


    // Associate coordinates with the vertex <alpha_1,alpha_2>(dart)
    id_t set_property(id_t dart, T prop) {
        id_t d = get_embedding_dart(dart);
        properties[d] = prop;
        return d;
    }

protected:

    idpropmap_t properties;
};



template<class T>
GMap::id_t EmbeddedGMap<T>::get_embedding_dart(id_t dart) 
{
    if (properties.count(dart) != 0)
        return dart;

    idlist_t orbitList = orbit({1, 2}, dart);
    for (id_t id : orbitList){
        if (properties.count(id) != 0)
            return id;
    }

    return dart;
}


/*------------------------------------------------------------------------*/


/* 
    The GMap3D extent GMap class with embedding.
    The property added is an index of position.
    A list of indexed position is also maintained.
*/ 
class GMap3D : public EmbeddedGMap<id_t> {
public:

    GMap3D();
    ~GMap3D();

    // Print for each dart, the value of the different alpha applications.
    void print_alphas();

    // Compute the center of each element.
    vec3_t element_center(degree_t degree, id_t dart);

    vec3_t get_position(id_t dart) {
        return positions[get_property(dart)];
    }

    // A new indexed position is added to the list
    // The index is associated to the dart.
    void set_position(id_t dart, vec3_t pos) {
        id_t pid = positions.size();
        positions.push_back(pos);
        set_property(dart, pid);
    }
 
    /*
        Compute the dual of the object
        Create a new GMap object with the same darts but reversed alpha relations
        Update the positions of the dual 0-cells as the centers of the 2-cells
    */
    GMap3D dual();

    static GMap3D square(float xsize = 5, float ysize = 5, float height = 0);

    static GMap3D cube(float xsize = 5, float ysize = 5, float zsize = 5);

    static GMap3D holeshape(float xsize = 5, float ysize = 5, float zsize = 5, float internalratio = 0.5);


    std::vector<vec3_t> positions;

};


int display(const GMap3D& gmap);

/*------------------------------------------------------------------------*/


