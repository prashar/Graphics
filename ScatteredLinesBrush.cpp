//
// SingleLineBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ScatteredLinesBrush.h"
#include "GraphicsUtils.h"
#include <time.h>
#include <math.h>

#define PI 3.14159265


extern float frand();

ScatteredLinesBrush::ScatteredLinesBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc,name)
{
	m_ccount = 0; 
}

void ScatteredLinesBrush::BrushBegin(const ImpBrush::Point source, const ImpBrush::Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	int dWidth = pDoc->getLineSize();

	glLineWidth((float)dWidth);

	BrushMove( source, target );
}

void ScatteredLinesBrush::BrushMove(const ImpBrush::Point source, const ImpBrush::Point target)
{
	// Get the basic pointers to document and UI.  
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	// Draw Random Lines
	m_ccount >= INT_MAX ? m_ccount = 0 : m_ccount++; 
	srand(time(NULL) + m_ccount);
	ImpBrush::Point temp_point;
	GLint total_lines = 4; 
	GLint A1x, A2x, AxMid, A1y, A2y, Ox, Oy; 
	
	if ( pDoc == NULL ) {
		printf( "SingleLineBrush::BrushMove  document is NULL\n" );
		return;
	}
	
	// Get the size, width, angle, and direction of the stroke
	int dSize = pDoc->getSize();
	int dWidth = pDoc->getLineSize(); 
	int dAngle = pDoc->getLineAngle();  
	int dDirection = pDoc->getStrokeDirection(); 

	// Overwrite stroke if GRADIENT chosen
	if (dDirection == STROKE_DIRECTION_GRADIENT)
	{
		dAngle = SobelGradient(source); 
	}

	// Compute Line Points
	Ox = target.x - (.5 * dSize); 
	Oy = target.y - (.5 * dSize);

	GLint rand_lines = rand() % total_lines + 1; 
	for (int i = 0; i < rand_lines; i++)
	{
		// Get Y first
		A1y = rand() % dSize + Oy; 
		A2y = A1y; 

		// Get X1 and X2 ( start and end lines ) 
		A1x = rand() % dSize + Ox;
		A2x = A1x + dSize;
		AxMid = .5*(A1x + A2x);

		temp_point.x = source.x + (AxMid - target.x);
		temp_point.y = source.y + (A1y - target.y);
		SetColor(temp_point);

		// Translate, Rotate, Translate
		glPushMatrix();
		glTranslatef(target.x, target.y, 0);
		glRotatef(dAngle, 0.0, 0.0, 1.0);
		glTranslatef(-target.x, -target.y, 0);

		glBegin(GL_LINES);

		glVertex2i(A1x, A1y);
		glVertex2i(A2x, A2y);

		glEnd();
		glPopMatrix();

	}
}

GLint ScatteredLinesBrush::SobelGradient(const ImpBrush::Point source)
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
	a5 = GraphicsUtils::GetLuminosity(dColor);

	// Calculate a6 above .. 
	srcPoint.x = source.x - 1;
	srcPoint.y = source.y - 1;
	memcpy(dColor, pDocument->GetOriginalPixel(srcPoint), 3);
	a6 = GraphicsUtils::GetLuminosity(dColor);

	// Calculate a7 above .. 
	srcPoint.x = source.x - 1;
	srcPoint.y = source.y;
	memcpy(dColor, pDocument->GetOriginalPixel(srcPoint), 3);
	a7 = GraphicsUtils::GetLuminosity(dColor);

	/* 
		Sobel Filter is -  the following two matrices convolved
		with the image. fdY & fdX

		[-1		0		 1]
		[-2		0(x,y)	 2]
		[-1		0		 1]

		[1		2		1]
		[0		0(x,y)	 0]
		[-1		-2		 -1]

		We'll average the gradient at the center pixel
	*/

	// Multiply with a the matrix above 
	GLdouble fDx = (a2 + 2 * a3 + a4) - (a0 + 2 * a7 + a6); 
	GLdouble fDy = (a0 + 2 * a1 + a2) - (a6 + 2 * a5 + a4);
	fAtan = atan2(fDy, fDx); 
	
	// Switch to Radians
	dAngle = fAtan * (180 / PI); 
	
	// Account for negative angles .. 
	dAngle = (dAngle < 0) ? (360 + dAngle) : dAngle; 

	// We want the line to be drawn 90 degress 
	// perpendicular to the gradient change. 
	dAngle = (GLint)(dAngle + 90);
	dAngle %= 360; 

	return dAngle; 

}

void ScatteredLinesBrush::BrushEnd(const ImpBrush::Point source, const ImpBrush::Point target)
{
	// do nothing so far
}

