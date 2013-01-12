
#include "stdafx.h"
#include "starobject.h"
#include <stdlib.h>
#include <math.h>

#define red 3
#define yellow 1
#define blue 2
#define white 0

int backstar::swidth = 640;
int backstar::sheight = 480;
int backstar::swidthh = 320;
int backstar::sheighth = 240;
int midstar::swidth = 640;
int midstar::sheight = 480;
int midstar::swidthh = 320;
int midstar::sheighth = 240;
int forestar::swidth = 640;
int forestar::sheight = 480;
int planet::swidth = 640;
int planet::sheight = 480;

extern ENGINE engine;
IMAGE_OBJECT *imagered;
IMAGE_OBJECT *imageblue;
IMAGE_OBJECT *imageyellow;
IMAGE_OBJECT *imagewhite;
IMAGE_OBJECT *imageearth;
IMAGE_OBJECT *imagejupiter;
IMAGE_OBJECT *imagemars;
IMAGE_OBJECT *imageneptune;
IMAGE_OBJECT *imagevenus;
IMAGE_OBJECT *imagesaturn;

float starspeed = 1.0f;
float starspeedinv = 1.0f;
bool plannow = true;

void loadstarimages()
{
		imagered = engine.CreateImageObject();
		imageblue = engine.CreateImageObject();
		imageyellow = engine.CreateImageObject();
		imagewhite = engine.CreateImageObject();
		imageearth = engine.CreateImageObject();
		imagejupiter = engine.CreateImageObject();
		imagemars = engine.CreateImageObject();
		imageneptune = engine.CreateImageObject();
		imagevenus = engine.CreateImageObject();
		imagesaturn = engine.CreateImageObject();
		imagered->ALoadImage("redstar",100,100);
		imageblue->ALoadImage("bluestar",100,100);
		imageyellow->ALoadImage("yellowstar",100,100);
		imagewhite->ALoadImage("whitestar",100,100);
		imageearth->ALoadImage("earth",150,150);
		imagejupiter->ALoadImage("jupiter",150,150);
		imagemars->ALoadImage("mars",150,150);
		imageneptune->ALoadImage("neptune",150,150);
		imagevenus->ALoadImage("venus",150,150);
		imagesaturn->ALoadImage("saturn",150,150);

}

void destroystarimages()
{
	engine.DestroyImageObject(imagered);
	engine.DestroyImageObject(imageblue);
	engine.DestroyImageObject(imageyellow);
	engine.DestroyImageObject(imagewhite);
	engine.DestroyImageObject(imageearth);
	engine.DestroyImageObject(imagejupiter);
	engine.DestroyImageObject(imagemars);
	engine.DestroyImageObject(imageneptune);
	engine.DestroyImageObject(imagevenus);
	engine.DestroyImageObject(imagesaturn);
}

backstar::backstar()
{
	pixel = engine.CreatePixelObject();
	createnewstar();
}

backstar::~backstar()
{
	engine.DestroyPixelObject(pixel);
}

void backstar::createnewstar()
{
	float velocity,magnitude;
	x = ((rand() % 9000) - 4500) / (float)10000;
	y = ((rand() % 7000) - 3500) / (float)10000;
	velocity = ((rand() % 20)+1) / (float)1000;
	magnitude = (float)sqrt((x*x) + (y*y));
	vx = (x / magnitude) * velocity;
	vy = (y / magnitude) * velocity;
	ax = (vx / 5);
	ay = (vy / 5);
	colorv = velocity*5;
	type = rand() % 4;
	float color1,color2;
	color1 = (rand() % 3000) / (float)10000;
	color2 = color1 + (rand() % 1000) / (float)10000;
	
	switch(type)
	{
	case red:
		r = color2;
		g = color1;
		b = color1;
		break;
	case yellow:
		r = color2;
		g = color2;
		b = color1;
		break;
	case blue:
		r = color1;
		g = color1;
		b = color2;
		break;
	case white:
		r = color2;
		g = color2;
		b = color2;
		break;
	}
}

void __fastcall backstar::drawstar()
{
	xs = (int(swidth * x) + (swidthh));
	ys = (int(swidth * y) + (sheighth));
	if(xs < 0 || xs >= swidth || ys < 0 || ys >= sheight)
	{
		createnewstar();
		xs = (int(swidth * x) + (swidthh));
		ys = (int(swidth * y) + (sheighth));
	}
	rs = int(r*254.0);
	gs = int(g*254.0);
	bs = int(b*254.0);
	pixel->DrawPixel(xs,ys,rs,gs,bs);
}

void __fastcall backstar::movestar(float time)
{
	float temp;
	time *= starspeedinv;
	temp = (0.00001f*time)+1.0f;
	ax *= temp;
	ay *= temp;
	vx += ax*time;
	vy += ay*time;
	x += vx*time;
	y += vy*time;
	if(r > 1.0) 
		{
			r = 1.0;
			colorv = (float)0;
		}
	/*	if(r >= 1.0) 
		{
			r = 1.0;
			g = 1.0;
			colorv = (float)0;
		}
	*/	if(b > 1.0) 
		{
			b = 1.0;
			colorv = (float)0;
		}
		if(g > 1.0) 
		{
			//r = 1.0;
			g = 1.0;
			//b = 1.0;
			colorv = (float)0;
		}
	
	temp = colorv*time;
	r += temp;
	g += temp;
	b += temp;
}

void backstar::setres(int w,int h)
{
	swidth = w;
	sheight = h;
	swidthh = w/2;
	sheighth = h/2;
}


midstar::midstar()
{
	pixel = engine.CreatePixelObject();
	createnewstar();
}

midstar::~midstar()
{
	engine.DestroyPixelObject(pixel);
}

void midstar::createnewstar()
{
	
	x = ((rand() % 6000) - 3000) / (float)10000;
	y = ((rand() % 4500) - 2250) / (float)10000;
	velocity = ((rand() % 30)+1) / (float)2000;
	magnitude = (float)sqrt((x*x) + (y*y));
	vx = (x / magnitude) * velocity;
	vy = (y / magnitude) * velocity;
	ax = (vx * 2);
	ay = (vy * 2);
	colorv = velocity*50;
	type = rand() % 7;
	if(type == 0 || type == 1 || type == 2) type = white;
	if(type == 3 || type == 4) type = yellow;
	if(type == 5) type = blue;
	if(type == 6) type = red;
	float color1,color2;
	color1 = (rand() % 3000) / (float)10000;
	color2 = color1 + (rand() % 1000) / (float)10000;
	size = 0.00f;
	sizev = 0;
	switch(type)
	{
	case red:
		r = color2;
		g = color1;
		b = color1;
		break;
	case yellow:
		r = color2;
		g = color2;
		b = color1;
		break;
	case blue:
		r = color1;
		g = color1;
		b = color2;
		break;
	case white:
		r = color2;
		g = color2;
		b = color2;
		break;
	}
}

void midstar::drawstar()
{
	xs = (int(swidth * x) + (swidthh));
	ys = (int(swidth * y) + (sheighth));
	if( ys < 0 || ys >= sheight || xs < 0 || xs >= swidth)
	{
		createnewstar();
		xs = (int)((swidth * x) + (swidthh));
		ys = (int)((swidth * y) + (sheighth));
	}
	rs = int(r*254.0f);
	gs = int(g*254.0f);
	bs = int(b*254.0f);
	pixel->DrawPixel(xs,ys,rs,gs,bs);
}

void midstar::drawbigstar()
{
		int temp1,temp2;
	temp1 = int(size*(swidthh));
	temp2 = int(-100 + size*swidth);
	switch(type)
	{
	case red: 
		imagered->DrawImageStreched(xs-temp1,ys-temp1,temp2,temp2);
		break;
	case blue: 
		imageblue->DrawImageStreched(xs-temp1,ys-temp1,temp2,temp2);
		break;
	case yellow: 
		imageyellow->DrawImageStreched(xs-temp1,ys-temp1,temp2,temp2);
		break;
	case white: 
		imagewhite->DrawImageStreched(xs-temp1,ys-temp1,temp2,temp2);
		break;
	}
	
}
void midstar::movestar(float time)
{
	float temp;
	time *= starspeedinv;
	temp = (0.1f*time)+1.0f;
	ax *= temp;
	ay *= temp;
	vx += ax*time;
	vy += ay*time;
	x += vx*time;
	y += vy*time;
	size += sizev*time;
	/*	if(r >= 1.0) 
		{
			r = 1.0;
			colorv = (float)0;
			sizev = velocity*0.1666666f;
		}
	*/	if(g > 1.0) 
		{
			//r = 1.0;
			g = 1.0;
			colorv = (float)0;
			sizev = velocity*0.1666666f;
		}
		if(b > 1.0) 
		{
			b = 1.0;
			sizev = velocity*0.1666666f;
			colorv = (float)0;
		}
		if(r > 1.0) 
		{
			r = 1.0;
			//g = 1.0;
			//b = 1.0;
			sizev = velocity*0.1666666f;
			colorv = (float)0;
		}
	temp = colorv*time;
	r += temp;
	g += temp;
	b += temp;
}

void midstar::setres(int w,int h)
{
	swidth = w;
	sheight = h;
	swidthh = w/2;
	sheighth = h/2;

}


forestar::forestar()
{
	pixel = engine.CreatePixelObject();
	//image->SetTransparentColor(0,0,0);
	createnewstar();
}

forestar::~forestar()
{
	engine.DestroyPixelObject(pixel);
	//engine.DestroyImageObject(image);
}

void forestar::createnewstar()
{
	float velocity,magnitude;
	x = ((rand() % 2000) - 1000) / (float)10000;
	y = ((rand() % 1500) - 750) / (float)10000;
	velocity = ((rand() % 40)+1) / (float)15000;
	magnitude = (float)sqrt((x*x) + (y*y));
	vx = (x / magnitude) * velocity;
	vy = (y / magnitude) * velocity;
	ax = (vx * 2);
	ay = (vy * 2);
	//aax = vx / 5;
	//aay = vy / 5;
	colorv = velocity*125;
	type = rand() % 7;
	if(type == 0 || type == 1 || type == 2) type = white;
	if(type == 3 || type == 4) type = yellow;
	if(type == 5) type = blue;
	if(type == 6) type = red;
	float color1,color2;
	color1 = (rand() % 3000) / (float)10000;
	color2 = color1 + (rand() % 1000) / (float)10000;
	size = .0005f;
	sizev = velocity*1.8f;
	switch(type)
	{
	case red:
		//image->ALoadImage("redstar",100,100);
		r = color2;
		g = color1;
		b = color1;
		break;
	case yellow:
		//image->ALoadImage("yellowstar",100,100);
		r = color2;
		g = color2;
		b = color1;
		break;
	case blue:
		//image->ALoadImage("bluestar",100,100);
		r = color1;
		g = color1;
		b = color2;
		break;
	case white:
		//image->ALoadImage("whitestar",100,100);
		r = color2;
		g = color2;
		b = color2;
		break;
	}
}

void forestar::drawstar()
{
	xs = (int(swidth * x) + (swidth>>1));
	ys = (int(swidth * y) + (sheight>>1));
	if(ys < -20 || ys >= (sheight+20) || xs < -20 || xs >= (swidth+20))
	{
		createnewstar();
		xs = (int(swidth * x) + (swidth>>1));
		ys = (int(swidth * y) + (sheight>>1));
	}
	int temp1,temp2;
	temp1 = int(size*(swidth>>1));
	temp2 = int(-100 + size*swidth);
	switch(type)
	{
	case red: 
		imagered->DrawImageStreched(xs-temp1,ys-temp1,temp2,temp2);
		break;
	case blue: 
		imageblue->DrawImageStreched(xs-temp1,ys-temp1,temp2,temp2);
		break;
	case yellow: 
		imageyellow->DrawImageStreched(xs-temp1,ys-temp1,temp2,temp2);
		break;
	case white: 
		imagewhite->DrawImageStreched(xs-temp1,ys-temp1,temp2,temp2);
		break;
	}
	
}

void forestar::movestar(float time)
{
	time /= starspeed;
	ax *= ((float)0.8*time)+1.0f;
	ay *= ((float)0.8*time)+1.0f;
	vx += ax*time;
	vy += ay*time;
	x += vx*time;
	y += vy*time;
	r += colorv*time;
	g += colorv*time;
	b += colorv*time;
	size += sizev*time;
	switch(type)
	{
	case red:
		if(r >= 1.0) 
		{
			r = 1.0;
			colorv = (float)0;
		}
		break;
	case yellow:
		if(r >= 1.0) 
		{
			r = 1.0;
			g = 1.0;
			colorv = (float)0;
		}
		break;
	case blue:
		if(b >= 1.0) 
		{
			b = 1.0;
			colorv = (float)0;
		}
		break;
	case white:
		if(r >= 1.0) 
		{
			r = 1.0;
			g = 1.0;
			b = 1.0;
			colorv = (float)0;
		}
		break;
	}
}

void forestar::setres(int w,int h)
{
	swidth = w;
	sheight = h;
}





planet::planet()
{
	timeleft = (((rand() % 5) + 5) * 1000);
		lasttickcount = GetTickCount();
	plannow = false;
	//	createnewplan();
}

planet::~planet()
{

}

void planet::createnewplan()
{
	float velocity,magnitude;
	x = ((rand() % 1000) - 500) / (float)10000;
	y = ((rand() % 750) - 375) / (float)10000;
	velocity = ((rand() % 30)+1) / (float)20000;
	magnitude = (float)sqrt((x*x) + (y*y));
	vx = (x / magnitude) * velocity;
	vy = (y / magnitude) * velocity;
	ax = (vx * 5);
	ay = (vy * 5);
	type = rand() % 6;
	size = .0005f;
	sizev = velocity*18.0f;
}

void planet::drawplan()
{
	if(plannow)
	{
		xs = (int)((swidth * x) + (swidth * 0.5));
	ys = (int)((swidth * y) + (sheight * 0.5));
	if(xs < (0-size*swidth) || xs >= (swidth+size*swidth) || ys < (0-size*swidth) || ys >= (sheight+size*swidth))
	{
		plannow = false;
		//starspeed = 1.0f;
		timeleft = (((rand() % 60) + 30) * 1000);
		lasttickcount = GetTickCount();
		//createnewplan();
		//xs = (int)((swidth * x) + (swidth * 0.5));
		//ys = (int)((swidth * y) + (sheight * 0.5));
	}
	switch(type)
	{
	case 0: 
		imageearth->DrawImageStreched(xs-int(size*swidth/2),ys-int(size*swidth/2),int(-150+size*swidth),int(-150+size*swidth));
		break;
	case 1: 
		imagemars->DrawImageStreched(xs-int(size*swidth/2),ys-int(size*swidth/2),int(-150+size*swidth),int(-150+size*swidth));
		break;
	case 2: 
		imageneptune->DrawImageStreched(xs-int(size*swidth/2),ys-int(size*swidth/2),int(-150+size*swidth),int(-150+size*swidth));
		break;
	case 3: 
		imagesaturn->DrawImageStreched(xs-int(size*swidth/2),ys-int(size*swidth/2),int(-150+size*swidth),int(-150+size*swidth));
		break;
	case 4: 
		imagevenus->DrawImageStreched(xs-int(size*swidth/2),ys-int(size*swidth/2),int(-150+size*swidth),int(-150+size*swidth));
		break;
	case 5: 
		imagejupiter->DrawImageStreched(xs-int(size*swidth/2),ys-int(size*swidth/2),int(-150+size*swidth),int(-150+size*swidth));
		break;
	}
	}
	else
	{
		timeleft -= GetTickCount()-lasttickcount;
		lasttickcount = GetTickCount();
		if(timeleft < 0)
		{
			plannow = true;
			createnewplan();
		}
	}

}

void planet::moveplan(float time)
{
	if(plannow)
	{
		if(starspeed < 20.0f) 
		{
			starspeed *= (time/3)+1;
			starspeedinv = 1/starspeed;
		}
		else
		{

		ax *= ((float)0.8*time)+1.0f;
		ay *= ((float)0.8*time)+1.0f;
		vx += ax*time;
		vy += ay*time;
		x += vx*time;
		y += vy*time;
		sizev *= 0.6f*time+1.0f;
		size += sizev*time;
		}
	}
	else
	{
		if(starspeed > 1.0f) 
		{
			starspeed /= (time/3)+1;
			starspeedinv = 1/starspeed;
		}
	}
}

void planet::setres(int w,int h)
{
	swidth = w;
	sheight = h;
}
