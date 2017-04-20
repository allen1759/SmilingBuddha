/*!
 *\author Kai-Chih Huang
 * 
 *\copyright Copyright (c) 2012 by < Kai-Chih Huang/ NTU EE>
 */
#include "SmilingAnimation.h"
#include "Picture.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <windows.h>
#include <time.h>
//#include <director.h.>

using namespace cv;
using namespace std;

void SmilingAnimation::play(Picture &picture)
{
	clock_t duration;
	clock_t time;
	time = clock();
	time = time - starttime; 
	duration = time;

	//float alpha=1.0f;
	//float rate=0.025f;
	if(counter==picno*speedd-1){

		//picture.setAnimation(FadeAnimationEnum, mp->smilePhotos[counter],mp->smilePhotos[counter]);
		//FadeAnimation *fadeAnimation = static_cast< FadeAnimation *>(picture.getAnimation());
		//fadeAnimation->setProperties(FADE_OUT);
		//temp=(mp->smilePhotos[counter-1])*alpha
		if(flag!=1)
		{
		 if(fb==0)
			picture.setContent((mp->smilePhotos[type*30+29])*alpha*ori_light);
		 else
		    picture.setContent((mp->smilePhotos[type*30])*alpha*ori_light);
		}
		else
		{
			if(fb==0)
				picture.setContent((mp->smilePhotos[type*30+29])*ori_light);
			else
				picture.setContent((mp->smilePhotos[type*30])*ori_light);
		}
//		picture *= alpha;
		 
		alpha -= rate;
		if (alpha*ori_light <= 0.61f) {
			alpha = 0.0f;
			if(fb==0)
			{picture.setContent(mp->smilePhotos[type*30+29]*0.4);}//yyy
			else
			{picture.setContent((mp->smilePhotos[type*30])*0.4);}//yyy
			ended = true;
		}

//		picture.setContent(picture);
//		this->setEnd(true);
		return;
	}
		
	else{
		   if(flag==1)
		   {beta=1;}
		  	
		   if(fb==0)
				picture.setContent(mp->smilePhotos[type*30+(counter/speedd)]*ori_light*beta);
			else
			{picture.setContent(mp->smilePhotos[type*30-((counter)/speedd)+29]*ori_light*beta);}
	        if(flag!=1)
			{beta+=rate*2;}
			else
			{
			 alpha=beta;
			}
			if(beta>1)
			{beta=1;}
		    
			if(counter==(picno*speedd-2)&&flag==1&&fb==0)
			{
			  counter=0;
			  fb=1;
			}
	//picture.setContent(mp->smilePhotos[counter]);
	counter++;
	if(beta!=1&&flag!=1)
	{
	 counter=1;
	}
	return;
	}
}
void SmilingAnimation::display(Picture &picture)
{


}