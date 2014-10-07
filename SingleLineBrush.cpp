//
// SingleLineBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "SingleLineBrush.h"
#include "GraphicsUtils.h"

extern float frand();

SingleLineBrush::SingleLineBrush( ImpressionistDoc* pDoc, char* name ) :
	ImpBrush(pDoc,name)
{
}

void SingleLineBrush::BrushBegin( const ImpBrush::Point source, const ImpBrush::Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	int dWidth = pDoc->getSize();

	glLineWidth((float)dWidth);

	BrushMove( source, target );
}

void SingleLineBrush::BrushMove( const ImpBrush::Point source, const ImpBrush::Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	if ( pDoc == NULL ) {
		printf( "SingleLineBrush::BrushMove  document is NULL\n" );
		return;
	}

	int dSize = pDoc->getSize();
	int dWidth = pDoc->getLineSize(); 
	int dAngle = pDoc->getLineAngle();  
	int dDirection = pDoc->getStrokeDirection(); 

	if (dDirection == STROKE_DIRECTION_GRADIENT)
	{
		dAngle = SobelGradient(source); 
	}

	GLint Ax, Ay, Bx, By, Qx, Qy ;

	glBegin( GL_POINTS );
		SetColor( source );

		glVertex2d( target.x, target.y );

	glEnd();
}

void SingleLineBrush::BrushEnd( const ImpBrush::Point source, const ImpBrush::Point target )
{
	// do nothing so far
}

