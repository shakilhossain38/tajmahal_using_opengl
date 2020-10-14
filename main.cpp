#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;
float posx=250;
float posy=250;
float posz=0;
float lx=-3/sqrt(2);
float ly=-3/sqrt(2);
float lz=0;
float rx=-2/sqrt(2);
float ry=-2/sqrt(2);
float rz=0;
float ux=0;
float uy=0;
float uz=1;
struct point
{
	double x,y,z;
};


void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);

                glVertex3f(0, i*10, -90);
				glVertex3f(0, i*10, 90);



			}
		}glEnd();
	}
}

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,0);
		glVertex3f( a,-a,0);
		glVertex3f(-a,-a,0);
		glVertex3f(-a, a,0);
	}glEnd();
}
void drawCube(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,a);
		glVertex3f( a,-a,a);
		glVertex3f(-a,-a,a);
		glVertex3f(-a, a,a);

		glVertex3f(a,a,-a);
		glVertex3f(a,-a,-a);
		glVertex3f(-a,-a,-a);
		glVertex3f(-a,a,-a);

		glVertex3f(a,-a,a);
		glVertex3f(a,a,a);
		glVertex3f(a,a,-a);
		glVertex3f(a,-a,-a);


		glVertex3f(-a, -a,a);
		glVertex3f(-a,a,a);
		glVertex3f(-a,a,-a);
		glVertex3f(-a,-a,-a);


		glVertex3f( a,a,a);
		glVertex3f( a,a,-a);
		glVertex3f(-a,a,-a);
		glVertex3f(-a,a,a);

		glVertex3f(a,-a,a);
		glVertex3f(a,-a,-a);
		glVertex3f(-a,-a,-a);
		glVertex3f(-a,-a,a);
	}glEnd();
}


void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}


void drawSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{

		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

void drawCylinder(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0; i<=segments; i++)
        {
            points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
            points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
        }
    //draw quads using generated points
    for(i=0; i<segments; i++)
        {

            glBegin(GL_QUADS);
            {
                glVertex3f(points[i].x,points[i].y,height/2);
                glVertex3f(points[i+1].x,points[i+1].y,height/2);
                glVertex3f(points[i].x,points[i].y,-height/2);
                glVertex3f(points[i+1].x,points[i+1].y,-height/2);
            }
            glEnd();
        }


}


double tx=0;
double ty=0;
double a=0;
double b=0;
double d=0;
double e=0;
void drawSS()
{



//Water Wall3
glPushMatrix();
 {

 glScalef(1.7,1.7,17.9);
 glColor3f(0.5, 0.5, 0.5);
 glTranslatef(76.3,20,0);
 drawCube(1);
 }
 glPopMatrix();

//Water Wall4
glPushMatrix();
 {

 glScalef(1.7,1.7,17.9);
 glColor3f(0.5, 0.5, 0.5);
 glTranslatef(133.58,58.5,0);
 drawCube(1);
 }
 glPopMatrix();

glPushMatrix();
{
    glRotatef(35,0,0,1);
glScalef(5,30,30);


//Water

glPushMatrix();
 {
 glColor4f(0, 1, 1, 0.2);
 glScalef(12,0.04,0.55);
 glTranslatef(3,-42.2,0);
 drawCube(1);
 }
 glPopMatrix();



//Water Wall1

glPushMatrix();
 {

 glScalef(12.2,0.05,.05);
 glColor3f(0.5, 0.5, 0.5);
 glTranslatef(3,-32.2,-12);
 drawCube(1);
 }
 glPopMatrix();

 //Water Wall2

glPushMatrix();
 {
glColor3f(0.3,0.5,0.2);
 glScalef(12.2,0.05,.05);
 glColor3f(0.5, 0.5, 0.5);
 glTranslatef(3,-32.2,12);
 drawCube(1);
 }
 glPopMatrix();



//Road
glPushMatrix();
 {
 glColor3f(.15, .12,0);
 glScalef(30,0.04,2);
 glTranslatef(1,-47.2,0);
 drawCube(1);
 }
 glPopMatrix();


glPushMatrix();{
// bottom box

 glPushMatrix();
 {
 glColor3f(0.7, 0.7, 0.7);
 glScalef(1.8,0.04,0.5);
 glTranslatef(0.0,-40.2,0.0);
 drawCube(5.5);
 }
 glPopMatrix();

//main cube
  glPushMatrix();
  glColor3f(0.8, 0.8, 0.5);
  glScalef(4,1,1);
  glTranslatef(0.0,-0.6,0.0);
  drawCube(1);
  glPopMatrix();

 //main gombazz
 glPushMatrix();
 {
 glScalef(1.2,1,0.8);
 glColor3f(0.3,0.5,0.2);
 glTranslatef(0.0,.45,0.0);
 drawSphere(1,20,50);
 glPopMatrix();
 }
 //gombuz pointer

 glPushMatrix();
 glColor3f(0.3,0.5,0.2);
 glRotatef(-90,1,0,0);
 glScalef(0.1,0.02,2);
 drawCone(5,1.5,80);
 glPopMatrix();

 //man1

  glPushMatrix();

{


    glTranslatef(-tx,0,0);
    glTranslatef(21,0,-2.5);

//body

  glPushMatrix();

glColor3f(1,.1,0.2);
  glTranslatef(30,-.80,1.3);
  glScalef(.4,.25,.17);
  drawCube(1);
  glPopMatrix();

  //leg1

   glPushMatrix();
    glTranslatef(-a,0,0);
   glTranslatef(d,0,0);

  glColor3f(1, 0.8, 0.5);
  glTranslatef(30,-1.2,1.4);
  glScalef(.2,.165,.02);
  drawCube(1);
  glPopMatrix();

  //leg2

   glPushMatrix();
  glTranslatef(-b,0,0);
  glColor3f(1, 0.8, 0.5);
  glTranslatef(30,-1.2,1.2);
  glScalef(.2,.165,.02);
  drawCube(1);
  glPopMatrix();

  //hand1

  glPushMatrix();

  glColor3f(1, 0.8, 0.5);
  glRotatef(20,1,0,0);
  glTranslatef(30,-0.4,1.3);
  glScalef(.2,.165,.02);
  drawCube(1);
  glPopMatrix();

  //hand2

    glPushMatrix();

  glColor3f(1, 0.8, 0.5);
  glRotatef(-20,1,0,0);
  glTranslatef(30,-1.25,1.15);
  glScalef(.2,.165,.02);
  drawCube(1);
  glPopMatrix();

//head
glPushMatrix();
  glColor3f(.11, 0.8, 0.9);
  //glRotatef(-20,1,0,0);
  glTranslatef(29.8,-0.42,1.32);
  //glScalef(.2,.165,.02);
  glScalef(.07,.07,.07);
  drawSphere(2,10,50);
  glPopMatrix();
}
  glPopMatrix();


  //man2

  glPushMatrix();

{

    glTranslatef(-ty,0,0);

//body

  glPushMatrix();

  glColor3f(1,.1,0.2);
  glTranslatef(30,-.80,1.3);
  glScalef(.4,.25,.17);
  drawCube(1);
  glPopMatrix();

  //leg1

   glPushMatrix();

   glTranslatef(a,0,0);

  glColor3f(1, 0.8, 0.5);
  glTranslatef(30,-1.2,1.4);
  glScalef(.2,.165,.02);
  drawCube(1);
  glPopMatrix();

  //leg2

   glPushMatrix();
  glTranslatef(b,0,0);
  glColor3f(1, 0.8, 0.5);
  glTranslatef(30,-1.2,1.2);
  glScalef(.2,.165,.02);
  drawCube(1);
  glPopMatrix();

  //hand1

  glPushMatrix();

  glColor3f(1, 0.8, 0.5);
  glRotatef(20,1,0,0);
  glTranslatef(30,-0.4,1.3);
  glScalef(.2,.165,.02);
  drawCube(1);
  glPopMatrix();

  //hand2

    glPushMatrix();

  glColor3f(1, 0.8, 0.5);
  glRotatef(-20,1,0,0);
  glTranslatef(30,-1.25,1.15);
  glScalef(.2,.165,.02);
  drawCube(1);
  glPopMatrix();

//head
glPushMatrix();
  glColor3f(.11, 0.8, 0.9);
  //glRotatef(-20,1,0,0);
  glTranslatef(29.8,-0.42,1.32);
  //glScalef(.2,.165,.02);
  glScalef(.07,.07,.07);
  drawSphere(2,10,50);
  glPopMatrix();
}
  glPopMatrix();





 //2nd cube

  glPushMatrix();
  glColor3f(0.8, 0.8, 0.5);
  glScalef(3.7,.65,1);
  glTranslatef(0,-1.2,1.2);
  drawCube(1);
  glPopMatrix();


  //2nd gombazz
 glPushMatrix();
 {
 glScalef(.8,.5,0.4);
 glColor3f(0.3,0.5,0.2);
 glTranslatef(0,-.4,3.8);
 drawSphere(1.2,20,50);
 glPopMatrix();
 }

  //2nd gombazz pointer
 glPushMatrix();
 glColor3f(0.3,0.5,0.2);
 glTranslatef(0,0,1.5);
 glRotatef(-90,1,0,0);
 glScalef(0.01,0.009,2);
 drawCone(5,0.7,80);
 glPopMatrix();




   //3rd cube

  glPushMatrix();
  glColor3f(0.8, 0.8, 0.5);
  glScalef(3.7,.65,1);
  glTranslatef(0,-1.2,-1.2);
  drawCube(1);
  glPopMatrix();

  //3rd gumbazz
 glPushMatrix();
 {
 glScalef(0.8,.5,0.4);
 glColor3f(0.3,0.5,0.2);
 glTranslatef(0,-.4,-3.8);
 drawSphere(1.2,20,50);
 glPopMatrix();
 }


   //3rd gombuz pointer
 glPushMatrix();
 glColor3f(0.3,0.5,0.2);
 glTranslatef(0,0,-1.5);
 glRotatef(-90,1,0,0);
 glScalef(0.01,0.009,2);
 drawCone(5,0.7,80);
 glPopMatrix();


 //Main Door
glPushMatrix();
  glColor3f(0.8, 0.8, 0.9);
  glTranslatef(4.1,-.9,0);
  glRotatef(90,0,1,0);
  glScalef(0.4,0.8,0);
  drawCube(1);
  glPopMatrix();

   //2nd Door
  glPushMatrix();
  glColor3f(0.8, 0.8, 0.9);
  glTranslatef(3.88,-.95,1.5);
  glRotatef(90,0,1,0);
  glScalef(0.3,0.45,0);
  drawCube(1);
  glPopMatrix();

     //3rd Door
  glPushMatrix();
  glColor3f(0.8, 0.8, 0.9);
  glTranslatef(3.88,-.95,-1.5);
  glRotatef(90,0,1,0);
  glScalef(0.3,0.45,0);
  drawCube(1);
  glPopMatrix();




 //Minars Front-left
 glPushMatrix();
 glColor3f(0,0,1);
 glTranslatef(10,-.6,2.5);
 glRotatef(90,1,0,0);
 //glTranslatef(3,-1.1,4);
 glScalef(0.02,.03,.1);
 drawCylinder(2.3,14,90);
 glPopMatrix();


  glPushMatrix();
 glColor3f(1,0,1);
 glTranslatef(10,-.85,2.5);
 glRotatef(90,1,0,0);
 //glTranslatef(3,-1.1,4);
 glScalef(0.02,.03,.1);
 drawCylinder(2.81,.5,90);
 glPopMatrix();

 glPushMatrix();
 glColor3f(1,0,1);
 glTranslatef(10,-.45,2.5);
 glRotatef(90,1,0,0);
 //glTranslatef(3,-1.1,4);
 glScalef(0.02,.03,.1);
 drawCylinder(2.81,.5,90);
 glPopMatrix();

 glPushMatrix();
 glColor3f(1,0,1);
 glTranslatef(10,-.07,2.5);
 glRotatef(90,1,0,0);
 //glTranslatef(3,-1.1,4);
 glScalef(0.02,.03,.1);
 drawCylinder(2.81,.5,90);
 glPopMatrix();



//Front-Left-up minar's gombuz
glPushMatrix();
glTranslatef(10,0.1,2.5);
glColor3f(0.3,0.5,0.2);
// glScalef(0.06,0.6,0.2);
 drawSphere(0.1,20,50);
 glPopMatrix();

//Front-Left-up minar's pointer
 glPushMatrix();
 glColor3f(0.3,0.5,0.2);
 glTranslatef(10,0.1,2.5);
 glRotatef(-90,1,0,0);
 glScalef(0.01,0.009,1);
 drawCone(5,0.5,80);
 glPopMatrix();



  //Minars Front-Right
 glPushMatrix();
 glColor3f(0,0,1);
 glTranslatef(10,-.6,-2.5);
 glRotatef(90,1,0,0);
 //glTranslatef(3,-1.1,4);
 glScalef(0.02,.03,.1);
 drawCylinder(2.3,14,90);
 glPopMatrix();


  glPushMatrix();
 glColor3f(1,0,1);
 glTranslatef(10,-.85,-2.5);
 glRotatef(90,1,0,0);
 //glTranslatef(3,-1.1,4);
 glScalef(0.02,.03,.1);
 drawCylinder(2.81,.5,90);
 glPopMatrix();

 glPushMatrix();
 glColor3f(1,0,1);
 glTranslatef(10,-.45,-2.5);
 glRotatef(90,1,0,0);
 //glTranslatef(3,-1.1,4);
 glScalef(0.02,.03,.1);
 drawCylinder(2.81,.5,90);
 glPopMatrix();

 glPushMatrix();
 glColor3f(1,0,1);
 glTranslatef(10,-.07,-2.5);
 glRotatef(90,1,0,0);
 //glTranslatef(3,-1.1,4);
 glScalef(0.02,.03,.1);
 drawCylinder(2.81,.5,90);
 glPopMatrix();



//Front-right-up minar's gombuz
glPushMatrix();
glTranslatef(10,0.1,-2.5);
glColor3f(0.3,0.5,0.2);
// glScalef(0.06,0.6,0.2);
 drawSphere(0.1,20,50);
 glPopMatrix();


 //Front-right-up minar's Pointer
  glPushMatrix();
 glColor3f(0.3,0.5,0.2);
 glTranslatef(10,0.1,-2.5);
 glRotatef(-90,1,0,0);
 glScalef(0.01,0.009,1);
 drawCone(5,0.5,80);
 glPopMatrix();



 //Minars back-left
 glPushMatrix();
 glColor3f(0,0,1);
 glTranslatef(-8,-.6,2.5);
 glRotatef(90,1,0,0);
 //glTranslatef(3,-1.1,4);
 glScalef(0.02,.03,.1);
 drawCylinder(2.3,14,90);
 glPopMatrix();


  glPushMatrix();
 glColor3f(1,0,1);
 glTranslatef(-8,-.85,2.5);
 glRotatef(90,1,0,0);
 //glTranslatef(3,-1.1,4);
 glScalef(0.02,.03,.1);
 drawCylinder(2.81,.5,90);
 glPopMatrix();

 glPushMatrix();
 glColor3f(1,0,1);
 glTranslatef(-8,-.45,2.5);
 glRotatef(90,1,0,0);
 //glTranslatef(3,-1.1,4);
 glScalef(0.02,.03,.1);
 drawCylinder(2.81,.5,90);
 glPopMatrix();

 glPushMatrix();
 glColor3f(1,0,1);
 glTranslatef(-8,-.07,2.5);
 glRotatef(90,1,0,0);
 //glTranslatef(3,-1.1,4);
 glScalef(0.02,.03,.1);
 drawCylinder(2.81,.5,90);
 glPopMatrix();



//Left-back-up minar's gombuz
glPushMatrix();
glTranslatef(-8,0.1,2.5);
glColor3f(0.3,0.5,0.2);
// glScalef(0.06,0.6,0.2);
 drawSphere(0.1,20,50);
 glPopMatrix();


 //Left-back-up minar's pointer
 glPushMatrix();
glColor3f(0.3,0.5,0.2);
 glTranslatef(-8,0.1,2.5);
 glRotatef(-90,1,0,0);
 glScalef(0.01,0.009,1);
 drawCone(5,0.5,80);
 glPopMatrix();



 //Minars back-right

 glPushMatrix();
 glColor3f(0,0,1);
 glTranslatef(-8,-.6,-2.5);
 glRotatef(90,1,0,0);
 //glTranslatef(3,-1.1,4);
 glScalef(0.02,.03,.1);
 drawCylinder(2.3,14,90);
 glPopMatrix();


  glPushMatrix();
 glColor3f(1,0,1);
 glTranslatef(-8,-.85,-2.5);
 glRotatef(90,1,0,0);
 //glTranslatef(3,-1.1,4);
 glScalef(0.02,.03,.1);
 drawCylinder(2.81,.5,90);
 glPopMatrix();

 glPushMatrix();
 glColor3f(1,0,1);
 glTranslatef(-8,-.45,-2.5);
 glRotatef(90,1,0,0);
 //glTranslatef(3,-1.1,4);
 glScalef(0.02,.03,.1);
 drawCylinder(2.81,.5,90);
 glPopMatrix();

 glPushMatrix();
 glColor3f(1,0,1);
 glTranslatef(-8,-.07,-2.5);
 glRotatef(90,1,0,0);
 //glTranslatef(3,-1.1,4);
 glScalef(0.02,.03,.1);
 drawCylinder(2.81,.5,90);
 glPopMatrix();



//Right-back-up minar's gombuz
glPushMatrix();
glColor3f(0.3,0.5,0.2);
glTranslatef(-8,0.1,-2.5);
 drawSphere(0.1,20,50);
 glPopMatrix();

}glPopMatrix();


//Right-back-up minar's pointer
 glPushMatrix();
glColor3f(0.3,0.5,0.2);
 glTranslatef(-8,0.1,-2.5);
 glRotatef(-90,1,0,0);
 glScalef(0.01,0.009,1);
 drawCone(5,0.5,80);
 glPopMatrix();




//Trees

glPushMatrix();
{


 glScalef(1,1.1,1);
//Tree1

 //Body
 glPushMatrix();
 glColor3f(1,1,1);
 glTranslatef(50,-1.2,-0.7);
 glRotatef(90,1,0,0);

 glScalef(0.02,.03,.1);
 drawCylinder(.8,5,90);
 glPopMatrix();

    //leaf1


 glPushMatrix();
glColor3f(0.3,0.5,0.2);
 glTranslatef(50,-1,-0.75);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();

   //leaf2


 glPushMatrix();
glColor3f(0.3,0.5,0.2);
 glTranslatef(50,-1,-0.65);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);glColor3f(0.3,0.5,0.2);
 glPopMatrix();

    //leaf3


 glPushMatrix();
glColor3f(0,01,0);
 glTranslatef(49.8,-.9777777777,-0.7);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();


    //leaf4
  glPushMatrix();
glColor3f(0,1,0);
 glTranslatef(50.2,-1,-0.7);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();

//leaf5

  glPushMatrix();
glColor3f(0,1,0);
 glTranslatef(50,-.9,-0.7);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();




//Tree2
glPushMatrix();
{
    glTranslatef(-4,0,0);
 //Body
 glPushMatrix();
 glColor3f(1,1,1);
 //glColor3f(0,0,1);
 glTranslatef(50,-1.2,-0.7);
 glRotatef(90,1,0,0);
 //glTranslatef(3,-1.1,4);
 glScalef(0.02,.03,.1);
 drawCylinder(.8,5,90);
 glPopMatrix();

    //leaf1


 glPushMatrix();
glColor3f(0.3,0.5,0.2);
 glTranslatef(50,-1,-0.75);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();

   //leaf2


 glPushMatrix();
 glColor3f(0.3,0.5,0.2);
 glTranslatef(50,-1,-0.65);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();

    //leaf3


 glPushMatrix();
 glColor3f(0,1,0);
 glTranslatef(49.8,-.9777777777,-0.7);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();


    //leaf4
  glPushMatrix();
 glColor3f(0,1,0);
 glTranslatef(50.2,-1,-0.7);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();

//leaf5

  glPushMatrix();
 glColor3f(0,1,0);
 glTranslatef(50,-.9,-0.7);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();

}
glPopMatrix();



//Tree3
glPushMatrix();
{
    glTranslatef(-8,0,0);
 //Body
 glPushMatrix();
 glColor3f(1,1,1);
 //glColor3f(0,0,1);
 glTranslatef(50,-1.2,-0.7);
 glRotatef(90,1,0,0);
 //glTranslatef(3,-1.1,4);
 glScalef(0.02,.03,.1);
 drawCylinder(.8,5,90);
 glPopMatrix();

    //leaf1


 glPushMatrix();
glColor3f(0.3,0.5,0.2);
 glTranslatef(50,-1,-0.75);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();

   //leaf2


 glPushMatrix();
glColor3f(0.3,0.5,0.2);
 glTranslatef(50,-1,-0.65);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();

    //leaf3


 glPushMatrix();
 glColor3f(0,1,0);
 glTranslatef(49.8,-.9777777777,-0.7);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();


    //leaf4
  glPushMatrix();
 glColor3f(0,1,0);
 glTranslatef(50.2,-1,-0.7);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();

//leaf5

  glPushMatrix();
 glColor3f(0,1,0);
 glTranslatef(50,-.9,-0.7);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();

}
glPopMatrix();




//Tree4
glPushMatrix();
{
    glTranslatef(-12,0,0);
 //Body
 glPushMatrix();
 glColor3f(1,1,1);
 //glColor3f(0,0,1);
 glTranslatef(50,-1.2,-0.7);
 glRotatef(90,1,0,0);
 //glTranslatef(3,-1.1,4);
 glScalef(0.02,.03,.1);
 drawCylinder(.8,5,90);
 glPopMatrix();

    //leaf1


 glPushMatrix();
glColor3f(0.3,0.5,0.2);
 glTranslatef(50,-1,-0.75);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();

   //leaf2


 glPushMatrix();
glColor3f(0.3,0.5,0.2);
 glTranslatef(50,-1,-0.65);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();

    //leaf3


 glPushMatrix();
 glColor3f(0,1,0);
 glTranslatef(49.8,-.9777777777,-0.7);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();


    //leaf4
  glPushMatrix();
 glColor3f(0,1,0);
 glTranslatef(50.2,-1,-0.7);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();

//leaf5

  glPushMatrix();
 glColor3f(0,1,0);
 glTranslatef(50,-.9,-0.7);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();

}
glPopMatrix();





//Tree5
glPushMatrix();
{
    glTranslatef(-16,0,0);
 //Body
 glPushMatrix();
 glColor3f(1,1,1);
 //glColor3f(0,0,1);
 glTranslatef(50,-1.2,-0.7);
 glRotatef(90,1,0,0);
 //glTranslatef(3,-1.1,4);
 glScalef(0.02,.03,.1);
 drawCylinder(.8,5,90);
 glPopMatrix();

    //leaf1


 glPushMatrix();
glColor3f(0.3,0.5,0.2);
 glTranslatef(50,-1,-0.75);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();

   //leaf2


 glPushMatrix();
glColor3f(0.3,0.5,0.2);
 glTranslatef(50,-1,-0.65);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();

    //leaf3


 glPushMatrix();
 glColor3f(0,1,0);
 glTranslatef(49.8,-.9777777777,-0.7);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();


    //leaf4glColor3f(1,1,1);
  glPushMatrix();
 glColor3f(0,1,0);
 glTranslatef(50.2,-1,-0.7);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();

//leaf5

  glPushMatrix();
 glColor3f(0,1,0);
 glTranslatef(50,-.9,-0.7);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();

}
glPopMatrix();



//Tree6
glPushMatrix();
{
    glTranslatef(-20,0,0);
 //Body
 glColor3f(1,1,1);
 glPushMatrix();
 //glColor3f(0,0,1);
 glTranslatef(50,-1.2,-0.7);
 glRotatef(90,1,0,0);
 //glTranslatef(3,-1.1,4);
 glScalef(0.02,.03,.1);
 drawCylinder(.8,5,90);
 glPopMatrix();

    //leaf1


 glPushMatrix();
glColor3f(0.3,0.5,0.2);
 glTranslatef(50,-1,-0.75);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();

   //leaf2


 glPushMatrix();
glColor3f(0.3,0.5,0.2);
 glTranslatef(50,-1,-0.65);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();

    //leaf3


 glPushMatrix();
 glColor3f(0,1,0);
 glTranslatef(49.8,-.9777777777,-0.7);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();


    //leaf4
  glPushMatrix();
 glColor3f(0,1,0);
 glTranslatef(50.2,-1,-0.7);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();

//leaf5

  glPushMatrix();
 glColor3f(0,1,0);
 glTranslatef(50,-.9,-0.7);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();

}
glPopMatrix();


glPushMatrix();
{
    glTranslatef(0,0,1.45);

//Tree7

 //Body
 glPushMatrix();
 //glColor3f(0,0,1);
 glColor3f(1,1,1);
 glTranslatef(50,-1.2,-0.7);
 glRotatef(90,1,0,0);
 //glTranslatef(3,-1.1,4);
 glScalef(0.02,.03,.1);
 drawCylinder(.8,5,90);
 glPopMatrix();

    //leaf1


 glPushMatrix();
 glColor3f(0.3,0.5,0.2);
 glTranslatef(50,-1,-0.75);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();

   //leaf2


 glPushMatrix();
glColor3f(0.3,0.5,0.2);
 glTranslatef(50,-1,-0.65);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();

    //leaf3


 glPushMatrix();
 glColor3f(0,1,0);
 glTranslatef(49.8,-.9777777777,-0.7);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();


    //leaf4
  glPushMatrix();
 glColor3f(0,1,0);
 glTranslatef(50.2,-1,-0.7);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();

//leaf5

  glPushMatrix();
 glColor3f(0,1,0);
 glTranslatef(50,-.9,-0.7);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();




//Tree8
glPushMatrix();
{
    glTranslatef(-4,0,0);
 //Body
 glPushMatrix();
 glColor3f(1,1,1);
 //glColor3f(0,0,1);
 glTranslatef(50,-1.2,-0.7);
 glRotatef(90,1,0,0);
 //glTranslatef(3,-1.1,4);
 glScalef(0.02,.03,.1);
 drawCylinder(.8,5,90);
 glPopMatrix();

    //leaf1


 glPushMatrix();
 glColor3f(0.3,0.5,0.2);
 glTranslatef(50,-1,-0.75);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();

   //leaf2


 glPushMatrix();
 glColor3f(0.3,0.5,0.2);
 glTranslatef(50,-1,-0.65);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();

    //leaf3


 glPushMatrix();
 glColor3f(0,1,0);
 glTranslatef(49.8,-.9777777777,-0.7);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();


    //leaf4
  glPushMatrix();
 glColor3f(0,1,0);
 glTranslatef(50.2,-1,-0.7);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();

//leaf5

  glPushMatrix();
 glColor3f(0,1,0);
 glTranslatef(50,-.9,-0.7);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();

}
glPopMatrix();



//Tree9
glPushMatrix();
{
    glTranslatef(-8,0,0);
 //Body
 glPushMatrix();
 glColor3f(1,1,1);
 //glColor3f(0,0,1);
 glTranslatef(50,-1.2,-0.7);
 glRotatef(90,1,0,0);
 //glTranslatef(3,-1.1,4);
 glScalef(0.02,.03,.1);
 drawCylinder(.8,5,90);
 glPopMatrix();

    //leaf1


 glPushMatrix();
glColor3f(0.3,0.5,0.2);
 glTranslatef(50,-1,-0.75);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();

   //leaf2


 glPushMatrix();
glColor3f(0.3,0.5,0.2);
 glTranslatef(50,-1,-0.65);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();

    //leaf3


 glPushMatrix();
 glColor3f(0,1,0);
 glTranslatef(49.8,-.9777777777,-0.7);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();


    //leaf4
  glPushMatrix();
 glColor3f(0,1,0);
 glTranslatef(50.2,-1,-0.7);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();
glColor3f(0.3,0.5,0.2);
//leaf5

  glPushMatrix();
 glColor3f(0,1,0);
 glTranslatef(50,-.9,-0.7);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();

}
glPopMatrix();



glColor3f(1,1,1);
//Tree10
glPushMatrix();
{
    glTranslatef(-12,0,0);
 //Body
 glPushMatrix();
 glColor3f(1,1,1);
 //glColor3f(0,0,1);
 glTranslatef(50,-1.2,-0.7);
 glRotatef(90,1,0,0);
 //glTranslatef(3,-1.1,4);
 glScalef(0.02,.03,.1);
 drawCylinder(.8,5,90);
 glPopMatrix();

    //leaf1


 glPushMatrix();
glColor3f(0.3,0.5,0.2);
 glTranslatef(50,-1,-0.75);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();
glColor3f(1,1,1);
   //leaf2


 glPushMatrix();
glColor3f(0.3,0.5,0.2);
 glTranslatef(50,-1,-0.65);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();

    //leaf3


 glPushMatrix();
 glColor3f(0,1,0);
 glTranslatef(49.8,-.9777777777,-0.7);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();


    //leaf4
  glPushMatrix();
 glColor3f(0,1,0);
 glTranslatef(50.2,-1,-0.7);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();

//leaf5

  glPushMatrix();
 glColor3f(0,1,0);
 glTranslatef(50,-.9,-0.7);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();

}
glPopMatrix();





//Tree11
glPushMatrix();
{
    glTranslatef(-16,0,0);
 //Body
 glPushMatrix();
 glColor3f(1,1,1);
 //glColor3f(0,0,1);
 glTranslatef(50,-1.2,-0.7);
 glRotatef(90,1,0,0);
 //glTranslatef(3,-1.1,4);
 glScalef(0.02,.03,.1);
 drawCylinder(.8,5,90);
 glPopMatrix();

    //leaf1


 glPushMatrix();
glColor3f(0.3,0.5,0.2);
 glTranslatef(50,-1,-0.75);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();

   //leaf2


 glPushMatrix();
glColor3f(0.3,0.5,0.2);
 glTranslatef(50,-1,-0.65);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();

    //leaf3


 glPushMatrix();
 glColor3f(0,1,0);
 glTranslatef(49.8,-.9777777777,-0.7);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();


    //leaf4
  glPushMatrix();
 glColor3f(0,1,0);
 glTranslatef(50.2,-1,-0.7);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();

//leaf5

  glPushMatrix();
 glColor3f(0,1,0);
 glTranslatef(50,-.9,-0.7);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();

}
glPopMatrix();



//Tree12
glPushMatrix();
{
    glTranslatef(-20,0,0);
 //Body
 glPushMatrix();
 glColor3f(1,1,1);
 //glColor3f(0,0,1);
 glTranslatef(50,-1.2,-0.7);
 glRotatef(90,1,0,0);
 //glTranslatef(3,-1.1,4);
 glScalef(0.02,.03,.1);
 drawCylinder(.8,5,90);
 glPopMatrix();

    //leaf1


 glPushMatrix();
glColor3f(0.3,0.5,0.2);
 glTranslatef(50,-1,-0.75);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();

   //leaf2


 glPushMatrix();
glColor3f(0.3,0.5,0.2);
 glTranslatef(50,-1,-0.65);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();

    //leaf3


 glPushMatrix();
 glColor3f(0,1,0);
 glTranslatef(49.8,-.9777777777,-0.7);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();


    //leaf4
  glPushMatrix();
 glColor3f(0,1,0);
 glTranslatef(50.2,-1,-0.7);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();

//leaf5

  glPushMatrix();
 glColor3f(0,1,0);
 glTranslatef(50,-.9,-0.7);
 glScalef(0.045,.045,.045);
 drawSphere(2,5,90);
 glPopMatrix();

}
glPopMatrix();


}glPopMatrix();

}
glPopMatrix();

}glPopMatrix();








}




void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			drawgrid=1-drawgrid;
			break;

			case 'c':
        posx+=lx;
        posy+=ly;
        posz+=lz;
        break;

    case 'd':
        posx-=lx;
        posy-=ly;
        posz-=lz;
        break;

    case 'a':
        posx+=rx;
        posy+=ry;
        posz+=rz;
        break;

    case 'b':
        posx-=rx;
        posy-=ry;
        posz-=rz;
        break;

    case 'e':
        posx+=ux;
        posy+=uy;
        posz+=uz;
        break;

    case 'f':
        posx-=ux;
        posy-=uy;
        posz-=uz;
        break;



		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			cameraHeight -= 3.0;
			break;
		case GLUT_KEY_UP:		// up arrow key
			cameraHeight += 3.0;
			break;

		case GLUT_KEY_RIGHT:
			cameraAngle += 0.03;
			break;
		case GLUT_KEY_LEFT:
			cameraAngle -= 0.03;
			break;

		case GLUT_KEY_PAGE_UP:
			break;
		case GLUT_KEY_PAGE_DOWN:
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}



void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	//gluLookAt(10,10,10,	0,0,0,	1,1,0);
	//gluLookAt(10,0,0,	0,0,0,	0,1,0);
    gluLookAt(posx,posy,posz, posx+lx, posy+ly, posz+lz,	0,1,0);

	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	//drawAxes();
	//drawGrid();

    glColor3f(1,0,0);
    //drawSquare(10);
    //drawCube(20)




    drawSS();

    //drawCircle(30,24);

    //drawCone(20,50,24);

	//drawSphere(30,24,20);




	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){

    tx=tx+((2*6.5)/100)*.45;
    if(tx>25)
    {
        tx=25;

    }

    a=a+((2*6.5)/100)*.45;
    b=b+((2*6.5)/100)*.45;
    if(a>0.5)
    {
        a=0;
        if(tx==25)
        {
            a=25;b=25;
        }

    }
     if(a=0.5 && b>0.5)
    {
            b=0;
    }



    ty=ty-((2*6.5)/100)*.45;
    if(ty<-180)
    {

    }

    //For translation


	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=1;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
