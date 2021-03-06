/**
 * @file Jacobian.h
 * @brief Jacobian and terms.
 * @author Michael Kaess
 * @version $Id: Jacobian.h 5797 2011-12-07 03:50:41Z kaess $
 *
 * Copyright (C) 2009-2012 Massachusetts Institute of Technology.
 * Michael Kaess, Hordur Johannsson, David Rosen and John J. Leonard
 *
 * This file is part of iSAM.
 *
 * iSAM is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 *
 * iSAM is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with iSAM.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma once

#include <Eigen/Dense>

namespace isam {

typedef double (*cost_func_t)(double);


// Elementary Jacobian for one specific variable/node, potentially containing multiple measurement rows.
class Term {

  Node* _node;

  Eigen::MatrixXd _term;

public:

  const Node* node() const {return _node;}

  const Eigen::MatrixXd& term() const {return _term;}

  Term(Node* node, const Eigen::MatrixXd& term) : _node(node), _term(term) {}

  Term(Node* node, const double * term, int r, int c) : _node(node), _term(r,c) {
    int n = 0;
    for (int row=0; row<r; row++) {
      for (int col=0; col<c; col++) {
        _term(row,col) = term[n++];
      }
    }
  }
};

typedef std::list<Term> Terms;

// Jacobian consisting of multiple blocks.
class Jacobian {

  int _dimtotal;

  Terms _terms;

  Eigen::VectorXd _residual;

public:

  Jacobian() : _dimtotal(0), _residual()  {}

  Jacobian(Eigen::VectorXd& residual) : _dimtotal(0), _residual(residual) {}

  inline Jacobian(const double * residual, int size) : _dimtotal(0), _residual(size) {
    memcpy(_residual.data(), residual, size*sizeof(double));
  }

  const Terms& terms() const {return _terms;}

  // note: rhs for linear system Ax=b is negative of residual!
  Eigen::VectorXd rhs() const {return - _residual;}

  void add_term(Node* node, const Eigen::MatrixXd& term) {
    _terms.push_back(Term(node, term));
    _dimtotal += node->dim();
  }

  inline void add_term(Node* node, const double* term, int r, int c) {
    _terms.push_back(Term(node, term, r, c));
    _dimtotal += node->dim();
  }
  
  int dimtotal() const { return _dimtotal; }
};

}
