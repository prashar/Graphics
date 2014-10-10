//
// pointBrush.h
//
// The header file for Point Brush. 
//

#ifndef SINGLELINEBRUSH_H
#define SINGLELINEBRUSH_H

#include "impBrush.h"

class SingleLineBrush : public ImpBrush
{
public:
	SingleLineBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin( const ImpBrush::Point source, const ImpBrush::Point target );
	void BrushMove( const ImpBrush::Point source, const ImpBrush::Point target );
	void BrushEnd( const ImpBrush::Point source, const ImpBrush::Point target );
	GLint SobelGradient(const ImpBrush::Point source);
	char* BrushName( void );
};

#endif
