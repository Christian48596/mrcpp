#pragma once

#include "mwbuilders/TreeCalculator.h"
#include "mwtrees/FunctionTreeVector.h"

namespace mrcpp {

template<int D>
class MultiplicationCalculator : public TreeCalculator<D> {
public:
    MultiplicationCalculator(FunctionTreeVector<D> &inp) : prod_vec(&inp) { }
    virtual ~MultiplicationCalculator() { }

protected:
    FunctionTreeVector<D> *prod_vec;

    virtual void calcNode(MWNode<D> &node_o) {
        const NodeIndex<D> &idx = node_o.getNodeIndex();
        double *coefs_o = node_o.getCoefs();
        for (int j = 0; j < node_o.getNCoefs(); j++) {
            coefs_o[j] = 1.0;
        }
        for (int i = 0; i < this->prod_vec->size(); i++) {
            double c_i = this->prod_vec->getCoef(i);
            FunctionTree<D> &func_i = this->prod_vec->getFunc(i);
            // This generates missing nodes
            MWNode<D> node_i = func_i.getNode(idx); // Copy node
            node_i.mwTransform(Reconstruction);
            node_i.cvTransform(Forward);
            const double *coefs_i = node_i.getCoefs();
            int n_coefs = node_i.getNCoefs();
            for (int j = 0; j < n_coefs; j++) {
                coefs_o[j] *= c_i * coefs_i[j];
            }
        }
        node_o.cvTransform(Backward);
        node_o.mwTransform(Compression);
        node_o.setHasCoefs();
        node_o.calcNorms();
    }
};

}