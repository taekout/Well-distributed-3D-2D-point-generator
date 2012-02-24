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
	vector<vector< MediaPath *>> mpVec2nd;
	for(int i = 0 ; i < 10 ; i++)
	{
		vector<MediaPath *> *mpVec = new vector<MediaPath *>();
		for(int j = 0 ; j < 5 ; j++)
		{
			float wPrev[3];
			for(int k = 0 ; k < 3 ; k++)
				wPrev[k] = rng.RandomFloat();
			Vector _wPrev(wPrev[0], wPrev[1], wPrev[2]);
			_wPrev.Normalize();
			wPrev[0] = _wPrev.x; wPrev[1] = _wPrev.y; wPrev[2] = _wPrev.z;
			float wNext[3];
			for(int k = 0 ; k < 3 ; k++)
				wNext[k] = rng.RandomFloat();
			Vector _wNext(wNext[0], wNext[1], wNext[2]);
			_wNext.Normalize();
			wNext[0]= _wNext.x; wNext[1]= _wNext.y; wNext[2]= _wNext.z;
			//Point pPrev(0.f, 0.f, 0.f);
			//Point pCur(pPrev);
			float pCur[3] = {0.f}, pPrev[3] = {0.f};
			Spectrum alpha(0.f);
			MediaPath * mp = new MediaPath(pPrev, pCur, wPrev, wNext);
			mpVec ->push_back(mp);
		}
		mpVec2nd.push_back(*mpVec);
	}
	vector<vector<MediaPath *>> transformedPaths;
	Vector z(1.f,-1.f,1.5f);
	z.Normalize();
	// when z = (0, 1, 0), it will cause some problem!
	hg.TransformVector(z, mpVec2nd, transformedPaths);
	//hg.PrintVectors(transformedPaths);
	hg.ToString(z, mpVec2nd, transformedPaths);

	// Later, make sure that I free the one dimensional vectors then 2 dimensional vectors. You must loop over the vector<vector<MediaPath *>>.

}
/*
	Vector wPrev, wNext;
	Point pPrev, pCur;
	//BSDF *bsdf
	Spectrum alpha;
*/