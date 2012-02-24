#include "stdafx.h"
#include "HenyeyGreensteinSampleGenerator.h"
#include <iostream>
#include <fstream>

using namespace std;

HenyeyGreensteinSampleGenerator::HenyeyGreensteinSampleGenerator(unsigned int N, float g)
{
	point2dVec = new vector<SobolPoints2D *>;
	point3dVec = new vector<SobolPoints3D *>;
	this ->nPoints = N;
	GenerateSobolPoints2D(N);
	GenerateWellDistributedPointsOnSphere(g);
}


HenyeyGreensteinSampleGenerator::~HenyeyGreensteinSampleGenerator(void)
{
	delete [] points2d;
	delete [] points3d;
	delete point2dVec;
	delete point3dVec;
}

void HenyeyGreensteinSampleGenerator::PrintVectors(vector<vector<MediaPath *>> &mpVec)
{
	int len = mpVec.size();
	for(int i = 0 ; i < len ; i++) 
	{
		vector<MediaPath *> mpVec2 = mpVec.at(i);
		int len2 = mpVec2.size();
		for(int j = 0 ; j < len2 ; j++) {
			MediaPath *mp = mpVec2.at(j);
			cout << mp ->wNext.x << " : " <<mp ->wNext.y << " : " << mp ->wNext.z << endl;
		}
	}
}

void HenyeyGreensteinSampleGenerator::TransformVector(Vector curDir, vector<vector<MediaPath *>> &paths, // paths = sobel points.
								vector<vector<MediaPath *>> &transformedPaths)
{
	Vector arbitrary(0.f, 1.f, 0.f);
	Vector x = Cross(curDir, arbitrary);
	Vector y = Cross(x, curDir);
	float wPrev[3], wNext[3];
	//float pPrev[3], pCur[3];
	transformedPaths.reserve(paths.size());
	vector<vector<MediaPath *>>::iterator itend = paths.end();
	for(vector<vector<MediaPath *>>::iterator it = paths.begin() ; it < itend ; it++) {
		vector<MediaPath *> vecMediaPaths = *it;
		vector<MediaPath *>::iterator insideItEnd = vecMediaPaths.end();
		vector<MediaPath *> *convertedPaths = new vector<MediaPath *>();
		convertedPaths ->reserve(vecMediaPaths.size());
		for(vector<MediaPath *>::iterator insideIt = vecMediaPaths.begin() ; insideIt < insideItEnd ; insideIt++) {
			MediaPath * mediaPath = *insideIt;
			MediaPath * mediaPathConverted = new MediaPath();
			float len = mediaPath ->wNext.Length();
			mediaPathConverted ->wNext.x = x.x * mediaPath ->wNext.x + y.x * mediaPath ->wNext.y + curDir.x * mediaPath ->wNext.z;
			mediaPathConverted ->wNext.y = x.y * mediaPath ->wNext.x + y.y * mediaPath ->wNext.y + curDir.y * mediaPath ->wNext.z;
			mediaPathConverted ->wNext.z = x.z * mediaPath ->wNext.x + y.z * mediaPath ->wNext.y + curDir.z * mediaPath ->wNext.z;

			mediaPathConverted ->wPrev.x = x.x * mediaPath ->wPrev.x + y.x * mediaPath ->wPrev.y + curDir.x * mediaPath ->wPrev.z;
			mediaPathConverted ->wPrev.y = x.y * mediaPath ->wPrev.x + y.y * mediaPath ->wPrev.y + curDir.y * mediaPath ->wPrev.z;
			mediaPathConverted ->wPrev.z = x.z * mediaPath ->wPrev.x + y.z * mediaPath ->wPrev.y + curDir.z * mediaPath ->wPrev.z;

			mediaPathConverted ->alpha = mediaPath ->alpha;
			mediaPathConverted ->pCur =  mediaPath ->pCur;
			mediaPathConverted ->pPrev =  mediaPath ->pPrev;
			convertedPaths ->push_back(mediaPathConverted);
		}
		transformedPaths.push_back(*convertedPaths);
	}
}

void HenyeyGreensteinSampleGenerator::ToString(Vector &z, vector<vector<MediaPath *>> &paths, vector<vector<MediaPath* >> &paths2) {
	//Vector z(0.f,0.f,1.f);
	//Vector scatterDir(-1.3f, 0.8f, 0.3f);
	//scatterDir.Normalize();
	vector<vector<MediaPath *>>::iterator itend = paths.end();
	vector<vector<MediaPath *>>::iterator it2 = paths2.begin();
	for(vector<vector<MediaPath *>>::iterator it = paths.begin() ; it < itend ; it++, it2++) {
		vector<MediaPath *> vecMediaPaths = *it;
		vector<MediaPath *> vecMediaPaths2 = *it2;
		vector<MediaPath *>::iterator insideIt2 = vecMediaPaths2.begin();
		vector<MediaPath *>::iterator insideItEnd = vecMediaPaths.end();
		for(vector<MediaPath *>::iterator insideIt = vecMediaPaths.begin() ; insideIt < insideItEnd ; insideIt++, insideIt2++) {
			MediaPath *mp = *insideIt;
			//				cout << mp ->wNext.x << ":" << mp ->wNext.y << ":" << mp ->wNext.z << endl;
			MediaPath *mp2 = *insideIt2;
			float dot = Dot(mp ->wNext, Vector(0.f, 0.f, 1.f));
			float dot2 = Dot(mp2 ->wNext, z);
			cout << dot << " : " << dot2 << endl;
		}
	}
}

void HenyeyGreensteinSampleGenerator::GenerateWellDistributedPointsOnSphere(float g)
{
	points3d = new SobolPoints3D[nPoints];
	for(int i = 0 ; i < nPoints ; i++) { // random number points2d[i].x becomes the pdf.
		double s = points2d[i].x * 2. - 1.; // s = 2P-1.
		double Q = points2d[i].y;

		//float numerator = 1 - g*g;
		//float denominator = 1 + g * s ;//pow(g, (float)s);
		//float secondterm = -pow(numerator / denominator, 2.f);
		//float terms[2] = {1 + g * g,  secondterm};
		float mu = (1.f + g*g - pow((1.f-g*g) / (1.f + g*(float)s), 2.0f) ) * 0.5 / g;
		//float mu = 0.5f / g * (terms[0] + terms[1]);
		//mu = (mu > 1.0f) ? 1.0f : mu;
		//mu = (mu < -1.f) ? -1.f : mu;
		float sin_theta = sqrt(1.f - mu * mu);
		points3d[i].x = sin_theta * cos(M_PI * 2.f * Q);
		points3d[i].y = sin_theta * sin(M_PI * 2.f * Q);
		points3d[i].z = mu;
		points3d[i].pdf = (1.f - g*g) / pow( (1.f + g*g - 2.f * g * mu), 1.5f ) * .5f;

		SobolPoints3D *p = new SobolPoints3D(points3d[i]);
		point3dVec ->push_back(p);
	}
}



void HenyeyGreensteinSampleGenerator::GenerateSobolPoints2D(unsigned int N)
{
	char dir_file[] = "joe-kuo-other-4.5600"; int D = 2;
	ifstream infile(dir_file,ios::in);
	if (!infile) {
		cout << "Input file joe-kuo-other-4.5600 containing direction numbers cannot be found!\n";
		exit(1);
	}
	char buffer[1000];
	infile.getline(buffer,1000,'\n');

	// L = max number of bits needed 
	unsigned L = (unsigned)ceil(log((double)N)/log(2.0)); 

	// C[i] = index from the right of the first zero bit of i
	unsigned *C = new unsigned [N];
	C[0] = 1;
	for (unsigned i=1;i<=N-1;i++) {
		C[i] = 1;
		unsigned value = i;
		while (value & 1) {
			value >>= 1;
			C[i]++;
		}
	}

	// POINTS[i][j] = the jth component of the ith point
	//                with i indexed from 0 to N-1 and j indexed from 0 to D-1

	//double **POINTS = new double * [N];
	points2d = new SobolPoints2D[N];
	//for (unsigned i=0;i<=N-1;i++) POINTS[i] = new double [D];
	//for (unsigned j=0;j<=D-1;j++) points[//POINTS[0][j] = 0; 

	// ----- Compute the first dimension -----

	// Compute direction numbers V[1] to V[L], scaled by pow(2,32)
	unsigned *V = new unsigned [L+1]; 
	for (unsigned i=1;i<=L;i++) V[i] = 1 << (32-i); // all m's = 1

	// Evalulate X[0] to X[N-1], scaled by pow(2,32)
	unsigned *X = new unsigned [N];
	X[0] = 0;
	for (unsigned i=1;i<=N-1;i++) {
		X[i] = X[i-1] ^ V[C[i-1]];
		points2d[i].x = (double)X[i]/pow(2.0,32); // *** the actual points
		//POINTS[i][0] = (double)X[i]/pow(2.0,32); // *** the actual points
		//        ^ 0 for first dimension
	}

	// Clean up
	delete [] V;
	delete [] X;


	// ----- Compute the remaining dimensions -----
	// Read in parameters from file 
	unsigned d, s;
	unsigned a;
	infile >> d >> s >> a;
	unsigned *m = new unsigned [s+1];
	for (unsigned i=1;i<=s;i++) infile >> m[i];

	// Compute direction numbers V[1] to V[L], scaled by pow(2,32)
	V = new unsigned [L+1];
	if (L <= s) {
		for (unsigned i=1;i<=L;i++) V[i] = m[i] << (32-i); 
	}
	else {
		for (unsigned i=1;i<=s;i++) V[i] = m[i] << (32-i); 
		for (unsigned i=s+1;i<=L;i++) {
			V[i] = V[i-s] ^ (V[i-s] >> s); 
			for (unsigned k=1;k<=s-1;k++) 
				V[i] ^= (((a >> (s-1-k)) & 1) * V[i-k]); 
		}
	}

	// Evalulate X[0] to X[N-1], scaled by pow(2,32)
	X = new unsigned [N];
	X[0] = 0;
	for (unsigned i=1;i<=N-1;i++) {
		X[i] = X[i-1] ^ V[C[i-1]];
		points2d[i].y = (double)X[i]/pow(2.0,32);
		//POINTS[i][j] = (double)X[i]/pow(2.0,32); // *** the actual points
		//        ^ j for dimension (j+1)
	}

	// Clean up
	delete [] m;
	delete [] V;
	delete [] X;

	delete [] C;

	for(int i = 0 ; i < N ; i++) {
		SobolPoints2D *p = new SobolPoints2D();
		p ->x = points2d[i].x; p ->y = points2d[i].y;
		point2dVec ->push_back(p);
	}
}