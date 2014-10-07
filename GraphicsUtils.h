#ifndef GRAPHICSUTILS_H
#define GRAPHICSUTILS_H
#include "impressionist.h"

class GraphicsUtils
{
public:
	GraphicsUtils(void);
	static GLdouble GetLuminosity(GLubyte *color);
	static void Translate(GLint &x, GLint &y, const GLint Tx, const GLint Ty);
	static void Rotate(GLint &x, GLint &y, GLint degrees);
};

#endif