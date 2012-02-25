// SobolSequence.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include "Matrix4x4.h"

using namespace std;

// Frances Y. Kuo
//
// Email: <f.kuo@unsw.edu.au>
// School of Mathematics and Statistics
// University of New South Wales
// Sydney NSW 2052, Australia
// 
// Last updated: 21 October 2008
//
//   You may incorporate this source code into your own program 
//   provided that you
//   1) acknowledge the copyright owner in your program and publication
//   2) notify the copyright owner by email
//   3) offer feedback regarding your experience with different direction numbers
#include <cstdlib>
#include <cmath>
#include <math.h>

#include <iostream>
#include <iomanip>
#include <fstream>
#include "RNG.h"
#include "SobelGenerator.h"
#include "HenyeyGreensteinSampleGenerator.h"

#define		PI	3.141592

using namespace std;

int main(int argc, char **argv)
{
	if (argc != 3) {
		cout << endl << "input format: sobol N D FILENAME" << endl << endl;
		cout << "The program prints the first N sobol points in D dimensions." << endl;
		cout << "The points are generated in graycode order." << endl;
		cout << "The primitive polynomials and initial direction numbers are" << endl
			<< "given by the input file FILENAME." << endl << endl;
		return 0;
	}

	int N = atoi(argv[1]);
	HenyeyGreensteinSampleGenerator hg(N);
	RNG rng;
	rng.Seed(3);
	vector<vector<SobolPoints3D *>> mpVec2nd;
	for(int i = 0 ; i < 10 ; i++)
	{
		vector<SobolPoints3D *> *mpVec = new vector<SobolPoints3D *>();
		for(int j = 0 ; j < 5 ; j++)
		{
			float wPrev[3];
			for(int k = 0 ; k < 3 ; k++)
				wPrev[k] = rng.RandomFloat();
			Vector _wPrev(wPrev[0], wPrev[1], wPrev[2]);
			_wPrev.Normalize();
			wPrev[0] = _wPrev.x; wPrev[1] = _wPrev.y; wPrev[2] = _wPrev.z;
			SobolPoints3D * mp = new SobolPoints3D(wPrev[0], wPrev[1], wPrev[2], 0.f);
			mpVec ->push_back(mp);
		}
		mpVec2nd.push_back(*mpVec);
	}
	vector<vector<SobolPoints3D *>> transformedPaths;
	Vector z(1.f,-1.f,1.5f);
	z.Normalize();
	// when z = (0, 1, 0), it will cause some problem!
	hg.TransformVector(z, mpVec2nd, transformedPaths);
	vector<SobolPoints3D *> paths;
	vector<SobolPoints3D *> pathsConverted;
	for(int i = 0 ; i < 20 ; i++) {
		SobolPoints3D * sp = new SobolPoints3D(rng.RandomFloat(), rng.RandomFloat(), rng.RandomFloat(), rng.RandomFloat());
		Vector v(sp ->operator Vector()); v.Normalize();
		sp ->x = v.x; sp ->y = v.y; sp ->z = v.z;
		paths.push_back(sp);
	}
	hg.TransformVector(z, paths, pathsConverted);
	//hg.PrintVectors(transformedPaths);
	hg.ToString(z, mpVec2nd, transformedPaths);
	//hg.ToString(z, paths, pathsConverted);

	// Later, make sure that I free the one dimensional vectors then 2 dimensional vectors. You must loop over the vector<vector<MediaPath *>>.

}
/*
	Vector wPrev, wNext;
	Point pPrev, pCur;
	//BSDF *bsdf
	Spectrum alpha;
*/