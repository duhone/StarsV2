#include "engine.h"

class backstar
{
private:
	static int swidth,sheight,swidthh,sheighth;
	float x,y;
	float vx,vy,ax,ay;
	int xs,ys;
	PIXEL_OBJECT *pixel;
	int rs,gs,bs;
	float r,g,b;
	float colorv;
	int type;
protected:
	void createnewstar();
public:
	backstar();
	~backstar();
	void __fastcall movestar(float time);
	void __fastcall drawstar();
	void setres(int w,int h);
};

class midstar
{
private:
	static int swidth,sheight,swidthh,sheighth;
	float x,y;
	float vx,vy,ax,ay;
	int xs,ys;
	PIXEL_OBJECT *pixel;
	int rs,gs,bs;
	float r,g,b;
	float colorv;
	int type;
	float velocity,magnitude;
	float size,sizev;
protected:
	void createnewstar();
public:
	midstar();
	~midstar();
	void movestar(float time);
	void drawstar();
	void drawbigstar();
	void setres(int w,int h);
};


class forestar
{
private:
	static int swidth,sheight;
	float x,y;
	float vx,vy,ax,ay;
	int xs,ys;
	PIXEL_OBJECT *pixel;
	int rs,gs,bs;
	float r,g,b;
	float colorv;
	int type;
	float size,sizev;
protected:
	void createnewstar();
public:
	forestar();
	~forestar();
	void movestar(float time);
	void drawstar();
	void setres(int w,int h);
};

class planet
{
private:
	static int swidth,sheight;
	float x,y;
	float vx,vy,ax,ay;
	int xs,ys;
	int type;
	float size,sizev;
	int timeleft,lasttickcount;
protected:
	void createnewplan();
public:
	planet();
	~planet();
	void moveplan(float time);
	void drawplan();
	void setres(int w,int h);
};


void loadstarimages();
void destroystarimages();