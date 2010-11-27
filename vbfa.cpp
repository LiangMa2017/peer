/*
 *  vbfa.cpp
 *  peer
 *
 *  Created by Oliver Stegle on 11/12/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "bayesnet.h"
#include "Eigen/Eigen"
#include "alglib/src/specialfunctions.h"
#include "vbfa.h"

/** Default constructors */ 
cWNode::cWNode(){}
cWNode::cWNode(MatrixXf E1){
	this->E1 = E1;
	//TODO: 2nd moment
}
cXNode::cXNode(){}
cXNode::cXNode(MatrixXf E1){
	this->E1 = E1;
	//TODO: 2nd moment
}


double cWNode::calcBound(cBayesNet &net){
	return 0.;
}


double cWNode::entropy(){
	return 0;
}


void cWNode::update(cBayesNet &net){
	cVBFA n = (cVBFA&)net;

	// for each phenotype, calculate covariance and mean of weight
	for(int i = 0; i < n.Np; i++){
		MatrixXf diagAE1 = MatrixXf::Zero(n.Np);
		diagAE1.diagonal() = n.Alpha.E1;
		MatrixXf cov = (diagAE1 + n.X.E2S*n.Eps.E1(i)).inverse(); // linalg.inv(diag(Alpha.E1) + Eps[d]*M)
		E1.row(i) = n.Eps.E1(i)*cov*n.X.E1.transpose()*n.pheno.E1.col(i); //  self.E1[d,:] = S.dot(dcov[:,:],Eps[d]*S.dot(_S.E1.T,net.dataNode.E1[ :,d]))
		E2S += (cov + E1.row(i).transpose()*E1.row(i)); //  E2 = dcov + outer(self.E1[d], self.E1[d])
	}
}



double cXNode::calcBound(cBayesNet &net){
	return 0.;
}


double cXNode::entropy(){
	return 0;
}


void cXNode::update(cBayesNet &net){
	cVBFA n = (cVBFA&)net;
	
	// big work - calculate precision matrix
	MatrixXf prec = MatrixXf::Identity(n.Nk, n.Nk);
	for(int i = 0; i < n.Np; i++){ // as we don't keep W.E2 in memory, have to recalculate it to compute the sum WE2[i]*Eps[i]
		MatrixXf diagAE1 = MatrixXf::Zero(n.Np);
		diagAE1.diagonal() = n.Alpha.E1;
		MatrixXf cov = (diagAE1 + n.X.E2S*n.Eps.E1(i)).inverse(); // linalg.inv(diag(Alpha.E1) + Eps[d]*M)
		E1.row(i) = n.Eps.E1(i)*cov*n.X.E1.transpose()*n.pheno.E1.col(i); //  self.E1[d,:] = S.dot(dcov[:,:],Eps[d]*S.dot(_S.E1.T,net.dataNode.E1[ :,d]))
		prec += n.Eps.E1(i)*(cov + E1.row(i).transpose()*E1.row(i)); //  E2 = dcov + outer(self.E1[d], self.E1[d])
	}
	
	// Invert precision to get covariance, update moments
	MatrixXf cov = prec.inverse(); 
	E1 = n.pheno.E1*cov*n.W.E1.transpose()*n.Eps.E1;
	E2S = cov + E1.transpose()*E1;
}


void cAlphaNode::update(cBayesNet &net){
	cVBFA n = (cVBFA&)net;
	b = pb + 0.5*n.W.E2S.diagonal().array();
	a = (pa + 0.5*n.Np)*MatrixXf::Ones(n.Nk);
	updateMoments();
}


void cEpsNode::update(cBayesNet &net){
	cVBFA n = (cVBFA&)net;
	a = (pa + 0.5*n.Nj)*MatrixXf::Ones(n.Np);
	
	MatrixXf m = n.pheno.E1*n.X.E1*n.W.E1.transpose();
	b = pa + 0.5*(m).array(); // INCOMPLETE!! + n.pheno.E2*MatrixXf::Ones());
	updateMoments();
}


cPhenoNode::cPhenoNode(){}


//constructor from expression data
cVBFA::cVBFA(MatrixXf pheno_mean)
{
	MatrixXf pheno_var(pheno_mean.rows(),pheno_mean.cols());
	cVBFA::cVBFA(pheno_mean,pheno_var);
}
//constructor that take variance into account
cVBFA::cVBFA(MatrixXf pheno_mean,MatrixXf pheno_var)
{
	

}


// Global update
void cVBFA::update(){
	for(int i=0; i < this->Niterations; ++i){
//		this.W.update();
//		this.Alpha.update();
//		this.S.update();
//		this.Eps.update();
	}
}
