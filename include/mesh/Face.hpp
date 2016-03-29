#ifndef _YAFEL_FACE_HPP
#define _YAFEL_FACE_HPP

/*
 * Face: 
 *
 * Data structure to represent the boundary between two cells.
 * It is composed of a collection of nodes shared by two adjacent elements,
 * an "inner" element, an "outer" element, numbers indicating the local face
 * number of the inner and outer elements, and a bool flag indicating whether
 * the face comprises a part of the mesh boundary (in which case the "outer"
 * element will be the same as the "inner", and is meaningless!). 
 * The numbering convention is such that the element with the lower element 
 * number will be the the "inner", and the higher will be "outer".
 * The nodes in the set will be ordered as they appear in the "inner" element,


 Example: Two-element quad mesh:

 Given two quad elements numbered 0 and 1.

 Element(0) = {0 1 4 3}
 Element(1) = {1 2 5 4}

 The face adjoining them would be:

 Face = { nodes={1,4}, inner=0, inner_face=1, outer=1, outer_face=3, boundary=false }

 The left-hand face of element 0 is:

 Face = { nodes={3,0}, inner=0, inner_face=3, outer=0, outer_face=3, boundary=true }

 3--------------4--------------5
 |              |              |
 |              |              |
 |     (0)      |     (1)      |
 |              |              |
 |              |              |
 0--------------1--------------2
 
 */

#include "yafel_globals.hpp"
#include <vector>
#include <iostream>

YAFEL_NAMESPACE_OPEN

class Face {
public:
  using size_type = std::size_t;

  std::vector<size_type> nodes;
  size_type inner;
  size_type inner_face;
  size_type outer;
  size_type outer_face;
  bool boundary;

  Face();
  Face(const std::vector<size_type> &n,
       size_type _i, size_type _if,
       size_type _o, size_type _of,
       bool _b);

};

//nice printer function
std::ostream & operator<<(std::ostream& out, const Face &F);

YAFEL_NAMESPACE_CLOSE

#endif
