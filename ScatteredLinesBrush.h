//
// pointBrush.h
//
// The header file for Point Brush. 
//

#ifndef SCATTEREDLINESBRUSH_H
#define SCATTEREDLINESBRUSH_H

#include "impBrush.h"

class ScatteredLinesBrush : public ImpBrush
{
private:
	int m_ccount; 

public:
	ScatteredLinesBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin( const ImpBrush::Point source, const ImpBrush::Point target );
	void BrushMove( const ImpBrush::Point source, const ImpBrush::Point target );
	void BrushEnd( const ImpBrush::Point source, const ImpBrush::Point target );
	GLint SobelGradient(const ImpBrush::Point source);
	char* BrushName( void );
};

#endif
