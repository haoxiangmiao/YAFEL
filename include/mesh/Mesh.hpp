#ifndef _YAFEL_MESH_HPP
#define _YAFEL_MESH_HPP

#include <vector>
#include "yafel_globals.hpp"
#include "lin_alg/Vector.hpp"

/*

This header describes the interface for the Mesh class, which holds
node locations, element connectivity, element faces (tbd), additional "tags" (gmsh parlance)
associated with elements.

The element_types vector holds a list of integers corresponding to the GMSH element type
(see online documentation for details)

 */

YAFEL_NAMESPACE_OPEN

class Mesh {
  
private:
  unsigned n_elems;
  unsigned n_nodes;
  double minLength;
  
public:
  std::vector< Vector > nodal_coords;
  std::vector< std::vector<unsigned> > elements;
  std::vector< std::vector<unsigned> > el_tags;
  std::vector<unsigned> element_type;
  
  // set mesh nodes and elements
  Mesh();
  Mesh(const std::vector<Vector> &nodes, 
       const std::vector<std::vector<unsigned> > &elems,
       const std::vector<unsigned> & eltype);

  // set mesh nodes, elems, and tags
  Mesh(const std::vector<Vector> &nodes, 
       const std::vector<std::vector<unsigned> > &elems, 
       const std::vector<unsigned> & eltype,
       const std::vector<std::vector<unsigned> > & _tags);
  
  void compute_min_length();

  inline unsigned get_n_nodes() const {return this->n_nodes;}
  inline unsigned get_n_elems() const {return this->n_elems;}
  inline double get_minLength() const {return this->minLength;}
  // copy ctor -- use default
  //Mesh(const Mesh & m_in);

  //destructor -- use default
  //~Mesh();

  // Node reordering methods
  void reorder_rcm();
  
};

YAFEL_NAMESPACE_CLOSE

#endif // end #ifndef _YAFEL_MESH_HP
