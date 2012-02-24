#pragma once

#include <vector>

using namespace std;

struct OutgoingDistribution{
	int N;
	double **points;
	float *pdf;

	OutgoingDistribution(int _N, double **_points, float *_pdf) { N = _N;this ->points = _points; pdf = _pdf;}
	OutgoingDistribution() { N = 0; this ->points = NULL; pdf = NULL;}
	~OutgoingDistribution() { if(points != NULL) delete [] points; if(pdf != NULL) delete [] pdf;}
} ;



class SobelGenerator
{
public:
	SobelGenerator(void);
	~SobelGenerator(void);

	void DistributedVectorGenerator(unsigned N, char *dir_file, float power); // power == 1 --> well distributed over the sphere.
	double **sobol_points(unsigned N, unsigned D, char *dir_file);
	Matrix4x4 * TransformVector(const Vector &z);
	void PickRandomPoints(const RNG &rng, int nSamples, vector<int> &VectorIndices, vector<Matrix4x4 *> &transformationMatrices);
	float PhaseHG(const Vector &w, const Vector &wp, float g);
	inline Vector GetPoint(int index) {
		Vector vec(outDistr ->points[index][0], outDistr ->points[index][1], outDistr ->points[index][2]);
		return vec;
	}
	void TransformDirectionIntoCurrentRaySpace(Vector &scatterDir, vector<Vector *> largeRayDirectionIDSpace
									,vector<Vector *> & resultRayDirectionInRaySpace);
	void TransformDirectionIntoCurrentRaySpace(Vector &scatterDir, vector<Vector *> & resultRayDirectionInRaySpace);
private:
	OutgoingDistribution *outDistr ;
	vector<int> vectorIndices;
	vector<Matrix4x4 *> transformMs;
};

