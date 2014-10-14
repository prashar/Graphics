//
// pointBrush.h
//
// The header file for Point Brush. 
//

#ifndef ROTATINGTRIANGLESBRUSH_H
#define ROTATINGTRIANGLESBRUSH_H

#include "impBrush.h"

class RotatingTrianglesBrush : public ImpBrush
{
private:
	int m_rotangle; 
public:
	RotatingTrianglesBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin( const ImpBrush::Point source, const ImpBrush::Point target );
	void BrushMove( const ImpBrush::Point source, const ImpBrush::Point target );
	void BrushEnd( const ImpBrush::Point source, const ImpBrush::Point target );
	char* BrushName( void );
};

#endif
