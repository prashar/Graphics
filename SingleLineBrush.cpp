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
#include <math.h>

#define PI 3.14159265


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
		//dAngle = SobelGradient(source); 
	}

	GLint Ax, Ay, Bx, By, Qx, Qy ;

	//
	// Compute Line Points
	//
	Ax = target.x - (.5 * dSize); 
	Bx = target.x + (.5 * dSize);
	Ay = target.y;
	By = target.y;


	// Translate, Rotate, Translate
	glPushMatrix();
	glTranslatef(target.x, target.y, 0);
	glRotatef(dAngle, 0.0, 0.0, 1.0);
	glTranslatef(-target.x, -target.y, 0);

	SetColor(source);
	glBegin(GL_LINES);

	glVertex2i(Ax, Ay);
	glVertex2i(Bx, By);

	glEnd();
	glPopMatrix(); 

}

GLint SingleLineBrush::SobelGradient(const ImpBrush::Point source)
{
	/*
		a0		a1		a2
		a7				a3
		a6		a5		a4			
	*/
	ImpressionistDoc *pDocument = GetDocument(); 
	GLubyte dColor[3]; 
	GLdouble fAtan = 0.0; 
	GLint dAngle = 0.0; 
	ImpBrush::Point srcPoint = source;
	GLdouble a0, a1, a2, a3, a4, a5, a6, a7; 

	// Calculate a0 above .. 
	srcPoint.x = source.x-1; 
	srcPoint.y = source.y+1; 
	memcpy(dColor, pDocument->GetOriginalPixel(srcPoint),3);
	a0 = GraphicsUtils::GetLuminosity(dColor);

	// Calculate a1 above .. 
	srcPoint.x = source.x;
	srcPoint.y = source.y+1;
	memcpy(dColor, pDocument->GetOriginalPixel(srcPoint), 3);
	a1 = GraphicsUtils::GetLuminosity(dColor);

	// Calculate a2 above .. 
	srcPoint.x = source.x + 1;
	srcPoint.y = source.y + 1;
	memcpy(dColor, pDocument->GetOriginalPixel(srcPoint), 3);
	a2 = GraphicsUtils::GetLuminosity(dColor);

	// Calculate a3 above .. 
	srcPoint.x = source.x + 1;
	srcPoint.y = source.y;
	memcpy(dColor, pDocument->GetOriginalPixel(srcPoint), 3);
	a3 = GraphicsUtils::GetLuminosity(dColor);

	// Calculate a4 above .. 
	srcPoint.x = source.x + 1;
	srcPoint.y = source.y - 1;
	memcpy(dColor, pDocument->GetOriginalPixel(srcPoint), 3);
	a4 = GraphicsUtils::GetLuminosity(dColor);

	// Calculate a5 above .. 
	srcPoint.x = source.x + 1;
	srcPoint.y = source.y - 1;
	memcpy(dColor, pDocument->GetOriginalPixel(srcPoint), 3);
	a4 = GraphicsUtils::GetLuminosity(dColor);

	// Calculate a6 above .. 
	srcPoint.x = source.x - 1;
	srcPoint.y = source.y - 1;
	memcpy(dColor, pDocument->GetOriginalPixel(srcPoint), 3);
	a4 = GraphicsUtils::GetLuminosity(dColor);

	// Calculate a7 above .. 
	srcPoint.x = source.x - 1;
	srcPoint.y = source.y;
	memcpy(dColor, pDocument->GetOriginalPixel(srcPoint), 3);
	a4 = GraphicsUtils::GetLuminosity(dColor);

	/* 
		Sobel Filter is -  
		[-1		0		 1]
		[-2		0(x,y)	 2]
		[-1		0		 1]

		[1		2		1]
		[0		0(x,y)	 0]
		[-1		-2		 -1]

		We'll average the gradient at the center pixel
	*/
	GLdouble fDx = (a2 + 2 * a3 + a4) - (a0 + 2 * a7 + a6); 
	GLdouble fDy = (a0 + 2 * a1 + a2) - (a6 + 2 * a5 + a4);
	fAtan = atan2(fDy, fDx); 
	dAngle = fAtan * (180 / PI); 
	dAngle = (dAngle < 0) ? (360 + dAngle) : dAngle; 

	// Add 90 deg to make perpendicular
	dAngle = (GLint)(dAngle + 90);
	dAngle %= 360; 

	return dAngle; 

}

void SingleLineBrush::BrushEnd( const ImpBrush::Point source, const ImpBrush::Point target )
{
	// do nothing so far
}

