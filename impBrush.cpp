//
// impBrush.cpp
//
// The implementation of virtual brush. All the other brushes inherit from it.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "impBrush.h"

// Static class member initializations
int			ImpBrush::c_nBrushCount	= 0;
ImpBrush**	ImpBrush::c_pBrushes	= NULL;

ImpBrush::ImpBrush(ImpressionistDoc*	pDoc, 
				   char*				name) :
					m_pDoc(pDoc), 
					m_pBrushName(name)
{
}

//---------------------------------------------------
// Return m_pDoc, which connects the UI and brushes
//---------------------------------------------------
ImpressionistDoc* ImpBrush::GetDocument(void)
{
	return m_pDoc;
}

//---------------------------------------------------
// Return the name of the current brush
//---------------------------------------------------
char* ImpBrush::BrushName(void)
{
	return m_pBrushName;
}

//----------------------------------------------------
// Set the color to paint with to the color at source,
// which is the coord at the original window to sample 
// the color from
//----------------------------------------------------
void ImpBrush::SetColor (const ImpBrush::Point source)
{
	ImpressionistDoc* pDoc = GetDocument();
	GLfloat alpha = pDoc->getAlpha(); 


	GLubyte color[3];
	memcpy ( color, pDoc->GetOriginalPixel( source ), 3 );
 
	// Change Alpha Settings
	GLfloat fRcolor = ((GLfloat)color[0]/255);
	GLfloat fGcolor = ((GLfloat)color[1]/255);
	GLfloat fBcolor = ((GLfloat)color[2]/255);

	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	glColor4f( fRcolor, fGcolor, fBcolor, alpha);

}
