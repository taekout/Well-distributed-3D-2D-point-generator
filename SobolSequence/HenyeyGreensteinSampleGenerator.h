#pragma once

#include <vector>

using namespace std;

struct SobolPoints2D
{
	double x;
	double y;
	SobolPoints2D(void) {
		x = y = 0.0;
	}
};

struct SobolPoints3D
{
	double x;
	double y;
	double z;

	float pdf;
	SobolPoints3D(void) {
		x = y = z = 0.0;
		pdf = 0.f;
	}
	SobolPoints3D(double _x, double _y, double _z, float _pdf) {
		x = _x; y = _y; z = _z; pdf = _pdf;
	}
};


class HenyeyGreensteinSampleGenerator
{
private:
	void GenerateSobolPoints2D(unsigned int N); // First generate the Sobol 2D points.
	// Second use this function to generate 3D points from the previously generated 2D sobol points.
	void GenerateWellDistributedPointsOnSphere(float g);

public:
	HenyeyGreensteinSampleGenerator(unsigned int N, float g = 0.64f);
	virtual ~HenyeyGreensteinSampleGenerator(void);
	virtual void ToString(Vector &z, vector<vector<MediaPath *>> &paths, vector<vector<MediaPath* >> &paths2);
	static void PrintVectors(vector<vector<MediaPath *>> &mpVec);
	static void TransformVector(Vector curDir, vector<vector<MediaPath *>> &paths,
				vector<vector<MediaPath *>> &transformedPaths);
	/*
** z_basis is the scatter vector direction. x and y are generated with cross product.
(x_basis, y_basis, z_basis) * (0,0,1)' = (IdentityMatrix) * (SobelSequence)';
*/
	void TransformDirectionIntoCurrentRaySpace(Vector &scatterDir, // scatter ray direction.
		vector<Vector *> & resultRayDirectionInRaySpace)	// result Rays after space conversion.
	{
		Vector arbitrary(0.f, 1.f, 0.f);
		Vector x = Cross(scatterDir, arbitrary);
		Vector y = Cross(x, scatterDir);
		float dirx, diry, dirz;
		float hgvecx, hgvecy, hgvecz;
		for(int i = 0 ; i < nPoints ; i++)
		{
			hgvecx = points3d[i].x;
			hgvecy = points3d[i].y;
			hgvecz = points3d[i].z;
			dirx = x.x * hgvecx + y.x * hgvecy + scatterDir.x * hgvecz;
			diry = x.y * hgvecx + y.y * hgvecy + scatterDir.y * hgvecz;
			dirz = x.z * hgvecx + y.z * hgvecy + scatterDir.z * hgvecz;
			resultRayDirectionInRaySpace.push_back(new Vector(dirx, diry, dirz));
		}
	}
	inline Vector GetPoint3D(int index) {
		return Vector(points3d[index].x, points3d[index].y, points3d[index].z);
	}

	inline vector<SobolPoints3D *> & GetAll3DVectors(void) {
		return	*point3dVec;
	}

private: // Data
	int nPoints;
	SobolPoints2D *points2d;
	SobolPoints3D *points3d;
	// just add vector. vector<SobolPoint3D *> point3dvec; later get the vector point and feed it to LBidir function.
	vector<SobolPoints2D *> *point2dVec;
	vector<SobolPoints3D *> *point3dVec;
};