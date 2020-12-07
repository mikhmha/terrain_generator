#pragma once
#include <bezier_surface.h>



class CSurface;






void stitch_BezierSurfaces(unsigned int l, unsigned int w, std::vector<BezierSurface>& bsurfaces);	// takes a vector of Bezier surfaces in l x w order and passes them to stitch function
void stitch_ADJEdges_smooth(BezierSurface &b0, BezierSurface &b1, bool horizontal);					// stitch adj Bezier surfaces using bezier curves, assumes edges are vertically adjacent if horizontal == false	
void stitch_ADJEdges(BezierSurface& b0, BezierSurface& b1, bool horizontal);						// stitch adj Bezier surfaces by only making points closest to boundary edge co-linear. produces shading discontinuity near edge borders			




std::vector<BezierSurface> gen_Terrain(unsigned int n, std::int32_t seed, std::int32_t octaves, float freq);
std::vector<BezierSurface> gen_TerrainMasks(unsigned int l, unsigned int w, std::int32_t seed, std::int32_t octaves, float freq);
BezierSurface gen_TerrainSurface(const CSurface& surface, const BezierSurface& mask);
std::vector<BezierSurface> gen_TerrainSurfaces(const std::vector<CSurface> &l, const std::vector<BezierSurface> &masks);





