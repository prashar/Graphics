#include "GraphicsUtils.h"
#include <math.h>

GraphicsUtils::GraphicsUtils(void)
{}

// Return Luminosity
GLdouble GraphicsUtils::GetLuminosity(GLubyte *color)
{
	// Must multiply by R*.299 G*.587, B*.114
	return .299*color[0] + .587*color[1] + .114*color[2]; 
}