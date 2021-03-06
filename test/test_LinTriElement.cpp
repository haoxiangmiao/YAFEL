#include "yafel_globals.hpp"
#include "element/LinTri.hpp"
#include "mesh/GmshMesh.hpp"
#include "mesh/RectilinearMesh.hpp"
#include "utils/DoFManager.hpp"
#include "utils/CG_DoFManager.hpp"
#include "utils/ElementType.hpp"
#include <iostream>
#include <cmath>

using namespace yafel;

template<unsigned NSD>
bool test_1() {
    GmshMesh<NSD> M("minsquare.msh");
    CG_DoFManager<GmshMesh<NSD>,NSD> dofm(M,1);
    LinTri<NSD> el(dofm);
    bool good = true;

    good = good && 
        el.n_spaceDim == NSD &&
        el.n_topoDim == 2 &&
        el.n_quadPoints == 1 && 
        el.dof_per_node == 1 && 
        el.dof_per_el == 3 && 
        el.vtk_type == 5 &&
        el.element_type == ElementType::LINEAR_TRI;

    return good;
}

// integrate over the 1x1 square in minsquare.msh (1054 elements)
bool test_2() {

    GmshMesh<2> M("minsquare.msh");
    CG_DoFManager<GmshMesh<2>,2> dofm(M,1);
    LinTri<2> el(dofm);
    el.init_element();


    double sum = 0;
    for(std::size_t elnum=0; elnum<M.n_elements(); ++elnum) {

        if(M.element_type(elnum) != ElementType::LINEAR_TRI) {
            continue;
        }

        el.update_element(M,elnum);

        for(std::size_t qpi=0; qpi<el.n_quadPoints; ++qpi) {

            for(std::size_t A=0; A<el.nodes_per_el; ++A) {
                sum += el.shape_vals[qpi](A)*el.JxW(qpi);
            }

        } //end quadpoints loop

    } // end elements loop
  
    return abs(sum - 1) < 1.0e-8;

}

int main() {

    int retval = 0;

    if(!test_1<2>()) {
        std::cerr << "Failed test_1<2>()" << std::endl;
        retval |= 1<<0;
    }
    if(!test_1<3>()) {
        std::cerr << "Failed test_1<3>()" << std::endl;
        retval |= 1<<0;
    }
    if(!test_2()) {
        std::cerr << "Failed test_2()" << std::endl;
        retval |= 1<<1;
    }
    return retval;
}
