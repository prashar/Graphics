//
// SingleLineBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "RotatingTrianglesBrush.h"
#include "GraphicsUtils.h"
#include <math.h>

#define PI 3.14159265


extern float frand();

RotatingTrianglesBrush::RotatingTrianglesBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc,name)
{
}

void RotatingTrianglesBrush::BrushBegin(const ImpBrush::Point source, const ImpBrush::Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	int dWidth = pDoc->getLineSize();

	glLineWidth((float)dWidth);

	BrushMove( source, target );
}

void RotatingTrianglesBrush::BrushMove(const ImpBrush::Point source, const ImpBrush::Point target)
{
	// Get the basic pointers to document and UI.  
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;
	
	// These will be line endpoints for targets
	GLint Ax, Ay, Bx, By, Cx, Cy; 

	m_rotangle += 10; 
	m_rotangle %= 360; 

	if ( pDoc == NULL ) {
		printf( "SingleLineBrush::BrushMove  document is NULL\n" );
		return;
	}
	
	// Get the size, width, angle, and direction of the stroke
	int dSize = pDoc->getSize();
	int dWidth = pDoc->getLineSize(); 
	int dAngle = pDoc->getLineAngle();  
	int dDirection = pDoc->getStrokeDirection(); 

	// Compute Triangle Verticess
	Ax = target.x - (.5 * dSize); 
	Bx = target.x + (.5 * dSize);
	Cx = target.x; 

	Ay = target.y - (.5 * dSize);
	By = target.y - (.5 * dSize);
	Cy = target.y + (.5 * dSize);

	SetColor(source); 


	// Translate, Rotate, Translate
	glPushMatrix();
	glTranslatef(target.x, target.y, 0);
	glRotatef(m_rotangle, 0.0, 0.0, 1.0);
	glTranslatef(-target.x, -target.y, 0);

	glBegin(GL_POLYGON);

	glVertex2i(Ax, Ay);
	glVertex2i(Bx, By);
	glVertex2i(Cx, Cy);


	glEnd();
	glPopMatrix(); 

}

void RotatingTrianglesBrush::BrushEnd(const ImpBrush::Point source, const ImpBrush::Point target)
{
	// do nothing so far
}

