//
// SingleLineBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ScatteredCirclesBrush.h"
#include "GraphicsUtils.h"
#include <time.h>
#include <math.h>

#define PI 3.14159265

extern float frand();

ScatteredCirclesBrush::ScatteredCirclesBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc,name)
{
	m_ccount = 0; 
}

void ScatteredCirclesBrush::BrushBegin(const ImpBrush::Point source, const ImpBrush::Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	glPointSize(1.0);

	BrushMove( source, target );
}

void ScatteredCirclesBrush::BrushMove(const ImpBrush::Point source, const ImpBrush::Point target)
{
	// Get the basic pointers to document and UI.  
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("ScatteredCirclesBrush::BrushMove  document is NULL\n");
		return;
	}

	// Draw Random Points based on the seed of the number
	// of times you've pressed it and intmax. 
	m_ccount >= INT_MAX ? m_ccount = 0 : m_ccount++; 
	srand(time(NULL) + m_ccount);

	// Get the size, width, angle, and direction of the stroke
	int dSize = pDoc->getSize();
	int dWidth = pDoc->getLineSize();
	int dAngle = pDoc->getLineAngle();
	int dDirection = pDoc->getStrokeDirection();

	// Point to be drawn .. 
	ImpBrush::Point temp_point;

	// Clamp the total number of points 
	GLint circ_count = 4;
	GLint seg_count = 40;
	
	// Point vertex
	GLint Ax, Ay, Bx, By, Ox, Oy; 
	
	// Find some random origin .. 
	Ox = target.x - (.5 * dSize); 
	Oy = target.y - (.5 * dSize);


	// Randomly generate number of circles .. 
	GLint ran_num_circles = rand()%circ_count + 1; 
	for (int i = 0; i < ran_num_circles; i++)
	{

		// Randomly move the X and y by some number <= SIZE
		Ax = rand() % dSize + Ox;
		Ay = rand() % dSize + Oy;

		// This is basically moving the new point certain distance away from the new point Ax - target(
		// which in most cases is the same ) 
		temp_point.x = source.x + (Ax - target.x);
		temp_point.y = source.y + (Ay - target.y);

		SetColor(temp_point);
	
		glBegin(GL_POLYGON);

		// Circle Loop .. 
		for (int j = 0; j < 360; j += (360 / seg_count))
		{
			GLfloat rad = j * (PI / 180); 
			Bx = Ax + cos(rad) * (.5*dSize); 
			By = Ay + sin(rad) * (.5*dSize);
			glVertex2d(Bx, By);
		}

		glEnd();
		
	}
}

void ScatteredCirclesBrush::BrushEnd(const ImpBrush::Point source, const ImpBrush::Point target)
{
	// do nothing so far
}

