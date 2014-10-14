// 
// impressionistDoc.cpp
//
// It basically maintain the Image for answering the color query from the brush.
// It also acts as the bridge between brushes and UI (including views)
//


#include <FL/fl_ask.H>

#include <algorithm>

#include "impressionistDoc.h"
#include "impressionistUI.h"

#include "impBrush.h"

// Include individual brush headers here.
#include "pointBrush.h"
#include "triangleBrush.h"
#include "SingleLineBrush.h"
#include "ScatteredLinesBrush.h"
#include "ScatteredPointsBrush.h"
#include "ScatteredCirclesBrush.h"
#include "RotatingTrianglesBrush.h"


#define DESTROY(p)	{  if ((p)!=NULL) {delete [] p; p=NULL; } }

ImpressionistDoc::ImpressionistDoc() 
{
	// Set NULL image name as init. 
	m_imageName[0]	='\0';	

	m_nWidth		= -1;
	m_ucImage		= NULL;
	m_ucPainting	= NULL;
	m_ucPreviewBackup = NULL;


	// create one instance of each brush
	ImpBrush::c_nBrushCount	= NUM_BRUSH_TYPE;
	ImpBrush::c_pBrushes	= new ImpBrush* [ImpBrush::c_nBrushCount];

	ImpBrush::c_pBrushes[BRUSH_POINTS]	= new PointBrush( this, "Points" );

	// Note: You should implement these 5 brushes.  They are set the same (PointBrush) for now
	ImpBrush::c_pBrushes[BRUSH_LINES]				
		= new SingleLineBrush( this, "Lines" );
	ImpBrush::c_pBrushes[BRUSH_CIRCLES]				
		= new PointBrush( this, "Circles" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_POINTS]	
		= new ScatteredPointsBrush( this, "Scattered Points" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_LINES]		
		= new ScatteredLinesBrush( this, "Scattered Lines" );
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_CIRCLES]	
		= new ScatteredCirclesBrush( this, "Scattered Circles" );
	ImpBrush::c_pBrushes[BRUSH_TRIANGLE]
		= new TriangleBrush(this, "Triangle Brush");
	ImpBrush::c_pBrushes[BRUSH_ROTATING_TRIANGLE]
		= new RotatingTrianglesBrush(this, "Rotating Triangle");
	// make one of the brushes current
	m_pCurrentBrush	= ImpBrush::c_pBrushes[0];

}


//---------------------------------------------------------
// Set the current UI 
//---------------------------------------------------------
void ImpressionistDoc::setUI(ImpressionistUI* ui) 
{
	m_pUI	= ui;
}

//---------------------------------------------------------
// Returns the active picture/painting name
//---------------------------------------------------------
char* ImpressionistDoc::getImageName() 
{
	return m_imageName;
}

//---------------------------------------------------------
// Called by the UI when the user changes the 
// stroke direction type.
// type: one of the defined stroke direction types.
//---------------------------------------------------------
void ImpressionistDoc::setStrokeDirection(int type)
{
	m_pUI->setStrokeDirection(type);
}

//---------------------------------------------------------
// Called by the UI when the user changes the brush type.
// type: one of the defined brush types.
//---------------------------------------------------------
void ImpressionistDoc::setBrushType(int type)
{
	m_pCurrentBrush	= ImpBrush::c_pBrushes[type];
}

//---------------------------------------------------------
// Return Line Size
//---------------------------------------------------------
int ImpressionistDoc::getLineSize()
{
	return m_pUI->getLineSize();
}

//---------------------------------------------------------
// Return Line Angle
//---------------------------------------------------------
int ImpressionistDoc::getLineAngle()
{
	return m_pUI->getLineAngle();
}


//---------------------------------------------------------
// Return Stroke Direction
//---------------------------------------------------------
int ImpressionistDoc::getStrokeDirection()
{
	return m_pUI->getStrokeDirection();
}

//---------------------------------------------------------
// Return Alpha
//---------------------------------------------------------
GLfloat ImpressionistDoc::getAlpha()
{
	return m_pUI->getAlpha();
}


//---------------------------------------------------------
// Returns the size of the brush.
//---------------------------------------------------------
int ImpressionistDoc::getSize()
{
	return m_pUI->getSize();
}

//---------------------------------------------------------
// Load the specified image
// This is called by the UI when the load image button is 
// pressed.
//---------------------------------------------------------
int ImpressionistDoc::loadImage(const char *iname) 
{
	// try to open the image to read
	unsigned char*	data;
	int				width, 
					height;

	if ( (data=load(iname, width, height))==NULL ) 
	{
		fl_alert("Can't load image file");
		return 0;
	}

	// reflect the fact of loading the new image
	m_nWidth		= width;
	m_nPaintWidth	= width;
	m_nHeight		= height;
	m_nPaintHeight	= height;

	// release old storage
	delete [] m_ucImage;
	delete [] m_ucPainting;
	delete [] m_ucPreviewBackup;

	m_ucImage		= data;

	// allocate space for draw view
	m_ucPainting		= new unsigned char [width*height*3];
	m_ucPreviewBackup	= new unsigned char [width*height*3];
	memset(m_ucPainting, 0, width*height*3);

	m_pUI->m_mainWindow->resize(m_pUI->m_mainWindow->x(), 
								m_pUI->m_mainWindow->y(), 
								width*2, 
								height+25);

	// display it on origView
	m_pUI->m_origView->resizeWindow(width, height);	
	m_pUI->m_origView->refresh();

	// refresh paint view as well
	m_pUI->m_paintView->resizeWindow(width, height);	
	m_pUI->m_paintView->refresh();


	return 1;
}


//----------------------------------------------------------------
// Save the specified image
// This is called by the UI when the save image menu button is 
// pressed.
//----------------------------------------------------------------
int ImpressionistDoc::saveImage(const char *iname, const char * type, int quality) 
{

	save(iname, m_ucPainting, m_nPaintWidth, m_nPaintHeight, type, quality);

	return 1;
}

//----------------------------------------------------------------
// Clear the drawing canvas
// This is called by the UI when the clear canvas menu item is 
// chosen
//-----------------------------------------------------------------
int ImpressionistDoc::clearCanvas() 
{

	// Release old storage
	if ( m_ucPainting ) 
	{
		delete [] m_ucPainting;

		// allocate space for draw view
		m_ucPainting	= new unsigned char [m_nPaintWidth*m_nPaintHeight*3];
		memset(m_ucPainting, 0, m_nPaintWidth*m_nPaintHeight*3);

		// refresh paint view as well	
		m_pUI->m_paintView->refresh();
	}
	
	return 0;
}

// Apply the filter specified by filter_kernel to the 
// each pixel in the source buffer and place the resulting
// pixel in the destination buffer.  

// This is called from the UI when the
// "preview" or "apply" button is pressed in the filter dialog.



/*
 *	INPUT: 
 *		sourceBuffer:		the original image buffer, 
 *		srcBufferWidth:		the width of the image buffer
 *		srcBufferHeight:	the height of the image buffer
 *							the buffer is arranged such that 
 *							origImg[3*(row*srcBufferWidth+column)+0], 
 *							origImg[3*(row*srcBufferWidth+column)+1], 
 *							origImg[3*(row*srcBufferWidth+column)+2]
 *							are R, G, B values for pixel at (column, row).
 *		destBuffer:			the image buffer to put the resulting
 *							image in.  It is always the same size
 *							as the source buffer.
 *
 *      filterKernel:		the 2D filter kernel,
 *		knlWidth:			the width of the kernel
 *		knlHeight:			the height of the kernel
 *
 *		divisor, offset:	each pixel after filtering should be
 *							divided by divisor and then added by offset
 */
void ImpressionistDoc::applyFilter( const unsigned char* sourceBuffer,
		int srcBufferWidth, int srcBufferHeight,
		unsigned char* destBuffer,
		const double *filterKernel, 
		int knlWidth, int knlHeight, 
		double divisor, double offset )
{
	// This needs to be implemented for image filtering to work.

	
	for (int row = 0; row < srcBufferHeight; row++)
	{
		for (int col = 0; col < srcBufferWidth; col++)
		{

			GLint ** rmat = new GLint*[knlHeight];
			GLint ** gmat = new GLint*[knlHeight];
			GLint ** bmat = new GLint*[knlHeight];

			GLdouble redsum = 0.0;
			GLdouble greensum = 0.0;
			GLdouble bluesum = 0.0;

			for (int i = 0; i < knlHeight; i++)
			{
				rmat[i] = new GLint[knlWidth];
				gmat[i] = new GLint[knlWidth];
				bmat[i] = new GLint[knlWidth];

			}

			// We want to go (row - 2) because 2,2 is the middle the middle pixel where we're calculating the value. 
			// 
			for (GLint y = (row - 2), a = knlHeight - 1; y < (row - 2 + knlHeight); y++, a--)
			{
				for (GLint x = (col - 2), b = 0; x < (col - 2 + knlWidth); x++, b++)
				{
					// Fixing the edge issues .. 
					// Just expand the edge pixels to pad for the additional space when convoluting. 
					GLint x0 = x < 0 ? 0 : x;
					x0 = x >= srcBufferWidth ? (srcBufferWidth - 1) : x0;
					GLint y0 = y < 0 ? 0 : y;
					y0 = y >= srcBufferHeight ? (srcBufferHeight - 1) : y0;

					// Save red, green, and blue channel values for each pixel
					rmat[a][b] = sourceBuffer[3 * (y0*srcBufferWidth + x0) + 0];
					gmat[a][b] = sourceBuffer[3 * (y0*srcBufferWidth + x0) + 1];
					bmat[a][b] = sourceBuffer[3 * (y0*srcBufferWidth + x0) + 2];

				}
			}

			// Perform the convolution - this is just basically looping over and multiplying numbers for each pixel. 
			for (int i = 0; i < knlHeight; i++)
			{
				for (int j = 0; j < knlWidth; j++)
				{
					redsum += rmat[i][j] * filterKernel[i*knlHeight + j];
					greensum += gmat[i][j] * filterKernel[i*knlHeight + j];
					bluesum += bmat[i][j] * filterKernel[i*knlHeight + j];
				}
			}

			// Take care of division and offset .. 
			if (divisor > 0)
			{
				redsum /= divisor;
				bluesum /= divisor;
				greensum /= divisor;
			}
			redsum += offset;
			greensum += offset;
			bluesum += offset;

			// Take care of color bounds ..
			redsum = redsum > 255 ? 255 : redsum;
			greensum = greensum > 255 ? 255 : greensum;
			bluesum = bluesum > 255 ? 255 : bluesum;
			redsum = redsum < 0 ? 0 : redsum;
			greensum = greensum < 0 ? 0 : greensum;
			bluesum = bluesum < 0 ? 0 : bluesum;

			// Finally write the color in the image
			destBuffer[3 * (row*srcBufferWidth + col) + 0] = (GLint)redsum;
			destBuffer[3 * (row*srcBufferWidth + col) + 1] = (GLint)greensum;
			destBuffer[3 * (row*srcBufferWidth + col) + 2] = (GLint)bluesum;

			// Clear all the unncesseary arrays you 
			// declared above .. 
			for (int i = 0; i < knlHeight; ++i)
			{
				delete[] rmat[i]; 		
				delete[] gmat[i];
				delete[] bmat[i];
			}

			delete[] rmat;
			delete gmat;
			delete bmat;

		}

	}


}


//------------------------------------------------------------------
// Get the color of the pixel in the original image at coord x and y
//------------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( int x, int y )
{
	if ( x < 0 ) 
		x = 0;
	else if ( x >= m_nWidth ) 
		x = m_nWidth-1;

	if ( y < 0 ) 
		y = 0;
	else if ( y >= m_nHeight ) 
		y = m_nHeight-1;

	return (GLubyte*)(m_ucImage + 3 * (y*m_nWidth + x));
}

//----------------------------------------------------------------
// Get the color of the pixel in the original image at point p
//----------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel( const ImpBrush::Point p )
{
	return GetOriginalPixel( p.x, p.y );
}



