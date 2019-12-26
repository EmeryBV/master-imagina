#include "gmap.hpp"
/*------------------------------------------------------------------------*/

// Return the application of the alpha_deg on dart
GMap::id_t GMap::alpha(degree_t degree, id_t dart) const
{
    assert(degree < 3);
    assert(dart < maxid);
    return alphas.at(dart)[degree];
}

// Return the application of a composition of alphas on dart
GMap::id_t GMap::alpha(degreelist_t degrees, id_t dart) const
{
    for(degree_t degree : degrees)
    {
        assert(degree < 3);
        assert(dart < maxid);
        dart = alphas.at(dart)[degree];
    }
    return dart;
}


//  Test if dart is free for alpha_degree (if it is a fixed point) 
bool GMap::is_free(degree_t degree, id_t dart) const
{
    assert(degree < 3);
    assert(dart < maxid);
    return alpha(degree, dart) == dart;
}

/*
    Test the validity of the structure. 
    Check that alpha_0 and alpha_1 are involutions with no fixed points.
*/
bool GMap::is_valid() const
{
    for(idalphamap_t::const_iterator it = alphas.begin() ; it != alphas.end() ; ++it)
    {
        const id_t& dart = it->first;
        for(degree_t degree = 0 ; degree < 2 ; ++degree)
        {
            const id_t& alpha_deg_of_dart = it->second[degree];
            if(dart == alpha_deg_of_dart) return false; // not fixed point
            if(dart != alpha(degree, alpha_deg_of_dart)) return false; // alpha_degree sho
        }
    }

    degreelist_t alphas = {0, 2, 0, 2};
    for(id_t dart : darts())
        if (alpha(alphas, dart) != dart)
            return false;

    return true;
}

/* 
    Create a new dart and return its id. 
    Set its alpha_i to itself (fixed points) 
*/
GMap::id_t GMap::add_dart()
{
    id_t dart = maxid;
    ++maxid;
    alphas[dart] = alpha_container_t(dart, dart, dart);
    return dart;
}

// Link the two darts with a relation alpha_degree if they are both free.
bool GMap::link_darts(degree_t degree, id_t dart1, id_t dart2)
{
    if (!is_free(degree, dart1)) return false;
    if (!is_free(degree, dart2)) return false;

    alphas[dart1][degree] = dart2;
    alphas[dart2][degree] = dart1;
    return true;
}

/* 
    Return the orbit of dart using a list of alpha relation.
    Example of use : gmap.orbit(0,[0,1]).
*/
GMap::idlist_t GMap::orbit(degreelist_t alphas, id_t dart)
{
    idlist_t result;
    idset_t marked;
    idlist_t toprocess = {dart};

    while (!toprocess.empty())
    {
        id_t d = toprocess.front();
        toprocess.erase(toprocess.begin());
        if(marked.count(d) == 0)
        {
            result.push_back(d);
            marked.insert(d);
            for(degree_t degree : alphas)
            {
                toprocess.push_back(alpha(degree, d));
            }    
        }
    }

    return result;
}

/*
    Return the ordered orbit of dart using a list of alpha relations by applying
    repeatingly the alpha relations of the list to dart.
    Example of use. gmap.orderedorbit(0,[0,1]).
    Warning: No fixed point for the given alpha should be contained.
*/
GMap::idlist_t GMap::orderedorbit(degreelist_t list_of_alpha_value, id_t dart)
{
    idlist_t result;
    id_t current_dart = dart;
    unsigned char current_alpha_index = 0;
    size_t n_alpha = list_of_alpha_value.size();

    do 
    {
        result.push_back(current_dart);
        degree_t current_alpha = list_of_alpha_value[current_alpha_index];
        current_dart = alpha(current_alpha, current_dart);
        current_alpha_index = (current_alpha_index+1) % n_alpha;
    }
    while ((current_dart != dart) || result.empty());

    return result;
}


/*
    Sew two elements of degree 'degree' that start at dart1 and dart2.
    Determine first the orbits of dart to sew and heck if they are compatible.
    Sew pairs of corresponding darts
    # and if they have different embedding  positions, merge them. 
*/
bool GMap::sew_dart(degree_t degree, id_t dart1, id_t dart2)
{
    if (degree == 1)
        link_darts(degree, dart1, dart2);

    else{
        idlist_t orbitList_dart1, orbitList_dart2;

        if (degree == 0){
            orbitList_dart1 = orbit({2}, dart1);
            orbitList_dart2 = orbit({2}, dart2);
        }
        if (degree == 2){
            orbitList_dart1 = orbit({0}, dart1);
            orbitList_dart2 = orbit({0}, dart2);
        }

        if (orbitList_dart1.size() != orbitList_dart2.size())
            return false;

        for (size_t i = 0; i < orbitList_dart1.size(); ++i)
            link_darts(degree, orbitList_dart1[i], orbitList_dart2[i]);

    }

    return true;
}

// Compute the Euler-Poincare characteristic of the subdivision
int GMap::eulercharacteristic()
{
    return elements(0).size() - elements(1).size() + elements(2).size();
}


/*------------------------------------------------------------------------*/

/*
    Check if a dart of the orbit representing the vertex has already been 
    associated with a value in propertydict. If yes, return this dart, else
    return the dart passed as argument.
template<class T>
GMap::id_t EmbeddedGMap<T>::get_embedding_dart(id_t dart) 
{
}
*/



/*------------------------------------------------------------------------*/

GMap3D GMap3D::dual()
{
    GMap3D dual_gmap;
    dual_gmap.maxid = maxid;

    idalphamap_t::iterator it = alphas.begin();
    while (it != alphas.end()) {
        id_t dart = it->first;
        alpha_container_t alphas_dart = it->second;
        dual_gmap.alphas.insert(std::make_pair(dart, alphas_dart.flip()));
        ++it;
    }
    
    GMap::idlist_t fdarts = elements(2);

    for (size_t i = 0; i < fdarts.size(); i++) {

        //Barycentre
        vec3_t barycentre = element_center(2, fdarts[i]);

        //On récupère la liste des brins représentant une face
        GMap::idlist_t orbit_i_darts = orderedorbit( {0,1}, fdarts[i]);

        // On considère que chaque face est un simple quadrilatère (4 arêtes = 8 brins): donc je simplifie le passage en triangle
        GMap::id_t A = get_embedding_dart( orbit_i_darts[0] );
        GMap::id_t B = get_embedding_dart( orbit_i_darts[2] );
        GMap::id_t C = get_embedding_dart( orbit_i_darts[4] );
        GMap::id_t D = get_embedding_dart( orbit_i_darts[6] );

        //Liste des sommets
        GMap::idlist_t vdarts = dual_gmap.elements(0);

        //Indices des sommets barycentriques
        unsigned short indexA = std::find(vdarts.begin(), vdarts.end(), A) - vdarts.begin();
        unsigned short indexB = std::find(vdarts.begin(), vdarts.end(), B) - vdarts.begin();
        unsigned short indexC = std::find(vdarts.begin(), vdarts.end(), C) - vdarts.begin();
        unsigned short indexD = std::find(vdarts.begin(), vdarts.end(), D) - vdarts.begin();

        //Set position
        dual_gmap.set_position(indexA, barycentre);
        dual_gmap.set_position(indexB, barycentre);
        dual_gmap.set_position(indexC, barycentre);
        dual_gmap.set_position(indexD, barycentre);

    }

    return dual_gmap;
}


/*------------------------------------------------------------------------*/
