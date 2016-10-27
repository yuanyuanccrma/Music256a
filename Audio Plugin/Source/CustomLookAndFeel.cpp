/*
  ==============================================================================

    SourceCode.cpp
    Created: 23 Oct 2016 10:38:55am
    Author:  YuanLi

  ==============================================================================
*/


#include "CustomLookAndFeel.h"

CustomLookAndFeel::CustomLookAndFeel()
{
    
}
void CustomLookAndFeel::drawRotarySlider	(	Graphics & 	g,
                                             int 	x,
                                             int 	y,
                                             int 	width,
                                             int 	height,
                                             float 	sliderPosProportional,
                                             float 	rotaryStartAngle,
                                             float 	rotaryEndAngle,
                                             Slider & 	slider )
{
    Image Strip = ImageCache::getFromMemory (BinaryData::knob_png,BinaryData::knob_pngSize);
    
    const double Rotation = (slider.getValue() - slider.getMinimum())  /   (slider.getMaximum() - slider.getMinimum());
    
    const int Frames = Strip.getHeight()/Strip.getWidth(); // number of frames for vertical film strip
    const int Idx = (int)ceil(Rotation * ((double)Frames-1.0) ); // current index from 0 --> nFrames-1
    
    const float radius = jmin (width / 2.0f, height / 2.0f) ;
    const float centreX = x + width * 0.5f;
    const float centreY = y + height * 0.5f;
    const float rx = centreX - radius - 1.0f;
    const float ry = centreY - radius - 1.0f;
    
    g.drawImage(Strip,
                (int)rx,
                (int)ry,
                2*(int)radius,
                2*(int)radius,   //Dest
                0,
                Idx*Strip.getWidth(),
                Strip.getWidth(),
                Strip.getWidth()); //Source
    
}
