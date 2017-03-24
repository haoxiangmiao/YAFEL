//
// Created by tyler on 3/23/17.
//

#ifndef YAFEL_SHAPEFUNCTIONUTILS_HPP
#define YAFEL_SHAPEFUNCTIONUTILS_HPP

/**
 * \file
 * This file contains utilities for computing shape functions
 * on tensor product and simplex elements. Functions included
 * are:
 *
 * - Jacobi polynomials
 * - Koornwinder polynomials
 */


#include "yafel_globals.hpp"
#include <vector>

YAFEL_NAMESPACE_OPEN

/**
 * \brief Evaluate a polynomial using Horner's Method
 *
 * Polynomial coeffs should be in decreasing order.
 * i.e.: polynomials of degree "n" are evaluated as:
 *
 * \begin{equation}
 * P(x) = coeffs[0]*x^{n} + coeffs[1]^x^{n-1} + ... + coeffs[n]
 * \end{equation}
 *
 * @tparam T datatype of "x"
 * @param coeffs coefficients of polynomial:
 * @param x
 * @return
 */
template<typename T>
T poly_eval(const std::vector<double> &coeffs, T x)
{
    T retval;
    for(int i=0; i<coeffs.size(); ++i) {
        retval *= x;
        retval += coeffs[i];
    }
    return retval;
}

/**
 * \brief Create coefficients of n-th degree Jacobi polynomial
 *
 * Recurrance relation given in http://lsec.cc.ac.cn/~hyu/teaching/shonm2013/STWchap3.2p.pdf
 *
 * @param n Jacobi polynomial degree
 * @param alpha Jacobi alhpa
 * @param beta Jacobi beta
 * @return polynomial coefficients suitable for use in poly_eval
 */
std::vector<double> jacobi(int n, double alpha, double beta);


YAFEL_NAMESPACE_CLOSE


#endif //YAFEL_SHAPEFUNCTIONUTILS_HPP
