#ifndef _YAFEL_JACOBIPRECONDITIONER_HPP
#define _YAFEL_JACOBIPRECONDITIONER_HPP

#include "yafel_globals.hpp"
#include "lin_alg/solver/iterative/Preconditioner.hpp"
#include "lin_alg/Vector.hpp"
#include "lin_alg/sparse_csr.hpp"

YAFEL_NAMESPACE_OPEN

template<typename dataType>
class JacobiPreconditioner : public Preconditioner<JacobiPreconditioner<dataType>, dataType> {

private:
  Vector<dataType> AdiagInv;

public:

  using size_type = typename sparse_csr<dataType>::size_type;
  using value_type = typename sparse_csr<dataType>::value_type;
  using reference = typename sparse_csr<dataType>::reference;

  JacobiPreconditioner(const sparse_csr<dataType> &A) : AdiagInv(A.rows(), dataType(0))
  {
    bool flag;
    for(size_type i=0; i<A.rows(); ++i) {
      value_type aii = A(i,i,flag);
      if(flag) {
        AdiagInv(i) = value_type(1)/aii;
      }
      else{
        std::cerr << "Warning: JacobiPreconditioner found 0.0 on diag of A." << std::endl;
        std::cerr << "\tResults using this preconditioner may be bad." << std::endl;
        std::cerr << "\tPlacing value_type(1) in the preconditioner at location: " << i << std::endl;
      }
    }
  }
  
  void solve(Vector<dataType> &rhs) const {
    for(size_type i=0; i<rhs.size(); ++i) {
      rhs(i) *= AdiagInv(i);
    }
  }

};

YAFEL_NAMESPACE_CLOSE

#endif
