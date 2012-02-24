// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <fstream>

using namespace std;
 


// TODO: reference additional headers your program requires here
#include "Vector.h"
#include "Matrix4x4.h"
#include "RNG.h"
#include "RGBSpectrum.h"
#include "Pos.h"
#define PI 3.141592
#define M_PI 3.141592
#define INFINITY 1000000

struct MediaPath {
	Vector wPrev, wNext;
	Point pPrev, pCur;
	//BSDF *bsdf;
	Spectrum alpha;
	MediaPath() {
		wPrev.x = wPrev.y = wPrev.z = INFINITY;
		wNext.x = wNext.y = wNext.z = INFINITY;
		pPrev.x = pPrev.y = pPrev.z = INFINITY;
		pCur.x = pCur.y = pCur.z = INFINITY;
		float rgb[3] = {0};
		alpha.FromRGB(rgb);
	}
	MediaPath(float _pPrev[3], float _pCur[3], float _wPrev[3], float _wNext[3]) {
		wPrev.x = _wPrev[0]; wPrev.y = _wPrev[1]; wPrev.z = _wPrev[2];
		wNext.x = _wNext[0]; wNext.y = _wNext[1]; wNext.z = _wNext[2];
		pPrev.x = _pPrev[0]; pPrev.y = _pPrev[1]; pPrev.z = _pPrev[2];
		pCur.x = _pCur[0]; pCur.y = _pCur[1]; pCur.z = _pCur[2];
		float rgb[3] = {0};
		alpha.FromRGB(rgb);
	}

};