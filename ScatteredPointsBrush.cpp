//
// SingleLineBrush.cpp
//
// The implementation of Point Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ScatteredPointsBrush.h"
#include "GraphicsUtils.h"
#include <time.h>
#include <math.h>

#define PI 3.14159265


extern float frand();

ScatteredPointsBrush::ScatteredPointsBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc,name)
{
	m_ccount = 0; 
}

void ScatteredPointsBrush::BrushBegin(const ImpBrush::Point source, const ImpBrush::Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	// Point size is 1, but slider indicates # of points you can spread. 
	glPointSize(1.0);

	BrushMove( source, target );
}

void ScatteredPointsBrush::BrushMove(const ImpBrush::Point source, const ImpBrush::Point target)
{
	// Get the basic pointers to document and UI.  
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("SingleLineBrush::BrushMove  document is NULL\n");
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
	GLint num_points = .1 * dSize * dSize; 
	if (num_points < 1)
	{
		num_points = 1; 
	}

	// Point vertex
	GLint Ax, Ay, Ox, Oy; 
	
	// Find some random origin .. 
	Ox = target.x - (.5 * dSize); 
	Oy = target.y - (.5 * dSize);


	for (int i = 0; i < num_points; i++)
	{

		// Randomly move the X and y by some number <= SIZE
		Ax = rand() % dSize + Ox;
		Ay = rand() % dSize + Oy;

		// This is basically moving the new point certain distance away from the new point Ax - target(
		// which in most cases is the same ) 
		temp_point.x = source.x + (Ax - target.x);
		temp_point.y = source.y + (Ay - target.y);

		SetColor(temp_point);
	
		glBegin(GL_POINTS);

		glVertex2d(Ax, Ay);
		
		glEnd();
		
	}
}

void ScatteredPointsBrush::BrushEnd(const ImpBrush::Point source, const ImpBrush::Point target)
{
	// do nothing so far
}

