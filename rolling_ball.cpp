#include<GL/glut.h>
#include<bits/stdc++.h>
using namespace std;


// Structure for Points
struct Point3D
{
    double x_val;
    double y_val;
    double z_val;


    // Constructor
    Point3D(){
        x_val = 0;
        y_val = 0;
        z_val = 0;
    }

    Point3D(double x, double y, double z){
        x_val = x;
        y_val = y;
        z_val = z;
    }

    // Copy Constructor
    Point3D(const Point3D& point){
        x_val = point.x_val;
        y_val = point.y_val;
        z_val = point.z_val;
    }
};


// Adding Two Points
Point3D addTwoPoints(Point3D point1, Point3D point2){
    Point3D point3 = Point3D(point1.x_val + point2.x_val, point1.y_val + point2.y_val, point1.z_val + point2.z_val);
    return point3;
}


// Subtract Two Points
Point3D subtractTwoPoints(Point3D point1, Point3D point2){
    Point3D point3 = Point3D(point1.x_val - point2.x_val, point1.y_val - point2.y_val, point1.z_val - point2.z_val);
    return point3;
}


// Multiply Point With a Number
Point3D multiplyPointWithNumber(Point3D point1, double num){
    Point3D point2 = Point3D(point1.x_val * num, point1.y_val * num, point1.z_val * num);
    return point2;
}


// Divide Point By a Number
Point3D dividePointByNumber(Point3D point1, double num){
    Point3D point2 = Point3D(point1.x_val / num, point1.y_val / num, point1.z_val / num);
    return point2;
}


// Multiplying Two Points
Point3D multiplyTwoPoints(Point3D point1, Point3D point2){
    Point3D point3 = Point3D(point1.y_val * point2.z_val - point1.z_val * point2.y_val, point1.z_val * point2.x_val - point1.x_val * point2.z_val, point1.x_val * point2.y_val - point1.y_val * point2.x_val);
    return point3;
}


// Global Variables
int window_width = 800, window_height = 800, window_x_pos = 70, window_y_pos = 70;
double changeRate = 0.1;
char title[] = "Rolling Ball";
GLfloat rotateAngle = 45.0;
double length = 10.0;
double boundaryLength = 5 * length;
int totalLineSquares = 40, stacks = 32, sectors = 16, MANUAL = 0, SIMULATION = 1;
double ballRadius = 5.0;


// Camera Structure
struct Camera
{
    Point3D eye_pos, lookat_dir, right_dir, up_dir;

    // Constructor
    Camera(){
        eye_pos = Point3D(150, -20, 60);
        lookat_dir = Point3D(-1, 0, 0);
        right_dir = Point3D(0, 1, 0);
        up_dir = Point3D(0, 0, 1);
    }

    // Looking
    void startLooking(){
        gluLookAt(eye_pos.x_val, eye_pos.y_val, eye_pos.z_val, eye_pos.x_val + lookat_dir.x_val, eye_pos.y_val + lookat_dir.y_val, eye_pos.z_val + lookat_dir.z_val, up_dir.x_val, up_dir.y_val, up_dir.z_val);
    }

    // Eye Position Change
    void moveForward(){
        eye_pos = addTwoPoints(eye_pos, lookat_dir);
    }

    void moveBackward(){
        eye_pos = subtractTwoPoints(eye_pos, lookat_dir);
    }

    void moveRight(){
        eye_pos = addTwoPoints(eye_pos, right_dir);
    }

    void moveLeft(){
        eye_pos = subtractTwoPoints(eye_pos, right_dir);
    }

    void moveUp(){
        eye_pos = addTwoPoints(eye_pos, up_dir);
    }

    void moveDown(){
        eye_pos = subtractTwoPoints(eye_pos, up_dir);
    }

    // Camera Change
    void lookLeft(){
        right_dir = addTwoPoints(multiplyPointWithNumber(right_dir, cos(changeRate)), multiplyPointWithNumber(lookat_dir, sin(changeRate)));
        lookat_dir = subtractTwoPoints(multiplyPointWithNumber(lookat_dir, cos(changeRate)), multiplyPointWithNumber(right_dir, sin(changeRate)));
    }

    void lookRight(){
        right_dir = addTwoPoints(multiplyPointWithNumber(right_dir, cos(-changeRate)), multiplyPointWithNumber(lookat_dir, sin(-changeRate)));
        lookat_dir = subtractTwoPoints(multiplyPointWithNumber(lookat_dir, cos(-changeRate)), multiplyPointWithNumber(right_dir, sin(-changeRate)));
    }

    void lookUp(){
        lookat_dir = addTwoPoints(multiplyPointWithNumber(lookat_dir, cos(changeRate)), multiplyPointWithNumber(up_dir, sin(changeRate)));
        up_dir = subtractTwoPoints(multiplyPointWithNumber(up_dir, cos(changeRate)), multiplyPointWithNumber(lookat_dir, sin(changeRate)));
    }

    void lookDown(){
        lookat_dir = addTwoPoints(multiplyPointWithNumber(lookat_dir, cos(-changeRate)), multiplyPointWithNumber(up_dir, sin(-changeRate)));
        up_dir = subtractTwoPoints(multiplyPointWithNumber(up_dir, cos(-changeRate)), multiplyPointWithNumber(lookat_dir, sin(-changeRate)));
    }

    void tiltCounterClockwise(){
        up_dir = addTwoPoints(multiplyPointWithNumber(up_dir, cos(changeRate)), multiplyPointWithNumber(right_dir, sin(changeRate)));
        right_dir = subtractTwoPoints(multiplyPointWithNumber(right_dir, cos(changeRate)), multiplyPointWithNumber(up_dir, sin(changeRate)));
    }

    void tiltClockwise(){
        up_dir = addTwoPoints(multiplyPointWithNumber(up_dir, cos(-changeRate)), multiplyPointWithNumber(right_dir, sin(-changeRate)));
        right_dir = subtractTwoPoints(multiplyPointWithNumber(right_dir, cos(-changeRate)), multiplyPointWithNumber(up_dir, sin(-changeRate)));
    }

    // Bonus Task
    void moveUpWithoutChangingReferencePoint(){
        eye_pos.z_val += 0.1;
        lookat_dir.z_val -= 0.1;
    }

    void moveDownWithoutChangingReferencePoint(){
        eye_pos.z_val -= 0.1;
        lookat_dir.z_val += 0.1;
    }

};

Camera camera;


// Initialize OpenGL
void initGL(){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}


// Draw a Single Square
void drawSingleSquare(){
    glBegin(GL_QUADS);
        glVertex3f(length/2, length/2, 0);
		glVertex3f(length/2, -length/2, 0);
		glVertex3f(-length/2, -length/2, 0);
		glVertex3f(-length/2, length/2, 0);
    glEnd();
}


// Draw a Row of Squares
void drawRowOfSquares(){
	for(int i = 0; i < totalLineSquares; i++){		
		glPushMatrix();
		// glRotatef(45 , 0, 0, 1);
		glTranslatef(i * length * 2, 0, 0);
		drawSingleSquare();
		glPopMatrix();   

		glPushMatrix();
		// glRotatef(45 , 0, 0, 1);
		glTranslatef(-i * length * 2, 0, 0);
		drawSingleSquare();
		glPopMatrix();   
	}
}


// Drawing a Checkerboard
void drawCheckerboard(){

	for(int i = 0; i < totalLineSquares; i++){
		glPushMatrix();
        if(i % 2 == 0){
           glTranslatef(0, i * length, 0); 
        }
        else{
            glTranslatef(length, i * length, 0);
        }
		glColor3f(1.0, 1.0, 1.0);
		drawRowOfSquares();
		glPopMatrix();


		glPushMatrix();
		if(i % 2 == 0){
           glTranslatef(0, -i * length, 0); 
        }
        else{
            glTranslatef(length, -i * length, 0);
        }
		glColor3f(1.0, 1.0, 1.0);
		drawRowOfSquares();
		glPopMatrix();
	}
}


// Drawing Boundary
void drawBoundaryLine(double boundary, double boundaryHeight){
    glBegin(GL_QUADS);
		glVertex3f(boundary, boundary, 0);
		glVertex3f(boundary, -boundary, 0);
		glVertex3f(boundary, -boundary, boundaryHeight);
		glVertex3f(boundary, boundary, boundaryHeight);
	glEnd();
}

void drawWholeBoundary(){
    glColor3f(1.0, 1.0, 0.0);
    double boundaryHeight = 5.0;

    glPushMatrix();
        for(int i = 0; i < 4; i++){
            drawBoundaryLine(boundaryLength, boundaryHeight);
            glRotated(90, 0, 0, 1);
        }
    glPopMatrix();
}


// Ball and Arrow Structure
struct BallWithArrow{
    Point3D ballPos;
	int totalRotations;
    double v;
	double angle;
	int opMode;

    // Constructor
    BallWithArrow(){
        ballPos = Point3D(0, 0, 0);
        totalRotations = 0;
        v = 0.6;
        angle = 0;
        opMode = MANUAL;
    }

    // Upper Sphere
    void drawSphereUpper(){
        struct Point3D spherePoints[stacks + 1][sectors + 1];

        // Generate Points
		for(int i = 0; i <= stacks; i++)
		{
            double height = ballRadius * sin(((i * 1.0) / stacks) * acos(-1) / 2);
            double r = ballRadius * cos(((i * 1.0) / stacks) * acos(-1) / 2);

			for(int j = 0; j <= sectors; j++)
			{
                spherePoints[i][j] = Point3D(r * cos(((j * 1.0) / sectors) * 2 * acos(-1)), r * sin(((j * 1.0) / sectors) * 2 * acos(-1)), height);

			}
		}

        // Drawing Upper Sphere Using Quads
        for(int i = 0; i < stacks; i++){
			for(int j = 0; j < sectors; j++)
			{
				if(j % 2){
					glColor3f(0, 1, 0);
				}else{
					glColor3f(0, 0, 1);
				}
				glBegin(GL_QUADS);
					glVertex3f(spherePoints[i][j].x_val, spherePoints[i][j].y_val, spherePoints[i][j].z_val);
					glVertex3f(spherePoints[i][j + 1].x_val, spherePoints[i][j + 1].y_val, spherePoints[i][j + 1].z_val);
					glVertex3f(spherePoints[i + 1][j + 1].x_val, spherePoints[i + 1][j + 1].y_val, spherePoints[i + 1][j + 1].z_val);
					glVertex3f(spherePoints[i + 1][j].x_val, spherePoints[i + 1][j].y_val, spherePoints[i + 1][j].z_val);
				glEnd();
            }
        }     
    }

    void drawBall(){

        int rotationAmount = totalRotations % 360;
        double radAngle = angle * acos(-1) / 180.0;

        glPushMatrix();
        glTranslatef(ballPos.x_val, ballPos.y_val, ballPos.z_val);
        glRotatef(rotationAmount, sin(radAngle), -cos(radAngle), 0);
        // Upper Sphere
        drawSphereUpper();

        // Lower Sphere
        glRotatef(180, 0, 1, 0);
        drawSphereUpper();
        glPopMatrix();
    }

    // Drawing an Arrow
    void drawArrowLine(){
        glLineWidth(5);
		glBegin(GL_LINES);
			glVertex3f(0,0,0);
			glVertex3f(20, 0,0);
		glEnd();
    }

    void drawArrowTriangle(){
        glBegin(GL_TRIANGLES);
			glVertex3f(20,0,0);
			glVertex3f(16, 4, 0);
			glVertex3f(16, -4, 0);
		glEnd();
    }

    void drawArrow(){
        glColor3f(1, 0, 0);
		glPushMatrix();
		glTranslatef(ballPos.x_val, ballPos.y_val, ballPos.z_val);
		glRotatef(angle, 0, 0, 1);

        drawArrowLine();
		drawArrowTriangle();
	
		glPopMatrix();
    }

    void drawBallAndArrow(){
        drawBall();
        drawArrow();
    }

    // Checking for collision
    void checkCollisionAndChangeAngle(){

        double x_part = cos(angle * acos(-1) / 180);
        double y_part = sin(angle * acos(-1) / 180);
        bool isCollision = false;

        if(ballPos.x_val + ballRadius >= boundaryLength || ballPos.x_val - ballRadius <= -boundaryLength){
            x_part *= -1;
            isCollision = true;
        }

        if(ballPos.y_val + ballRadius >= boundaryLength || ballPos.y_val - ballRadius <= -boundaryLength){
            y_part *= -1;
            isCollision = true;
        }

        if(isCollision){
            angle = atan(y_part / x_part) * 180.0 / acos(-1);
            if(x_part <= 0){
                angle += 180;
            }
        }

    }

    // Go Forward
    void goForward(){
        checkCollisionAndChangeAngle();

        ballPos = addTwoPoints(ballPos, Point3D(v * cos(angle * acos(-1) / 180), v * sin(angle * acos(-1) / 180), 0));
        totalRotations -= 10;
    }

    // Go Backward
    void goBackward(){
        checkCollisionAndChangeAngle();

        ballPos = subtractTwoPoints(ballPos, Point3D(v * cos(angle * acos(-1) / 180), v * sin(angle * acos(-1) / 180), 0));
        totalRotations += 10;
    }

    // Rotating Arrow Counter Clockwise
    void rotateArrowCounterClockwise(){
        angle = (int)(angle + 10) % 360;
    }

    // Rotating Arrow Clockwise
    void rotateArrowClockwise(){
        angle = (int)(angle - 10) % 360;
    }


    // Calculating Collision time for event driven simulation
    double calculateTimeForCollision(){
        double v_x_part = v * cos(angle * acos(-1) / 180.0);
        double v_y_part = v * sin(angle * acos(-1) / 180.0);

		double t1 = (boundaryLength - ballPos.x_val - ballRadius) / v_x_part;
		double t2 = (-boundaryLength - ballPos.x_val + ballRadius) / v_x_part;
		double t3 = (boundaryLength - ballPos.y_val - ballRadius) / v_y_part;
		double t4 = (-boundaryLength - ballPos.y_val + ballRadius) / v_y_part;

		t1 = abs(t1);
        t2 = abs(t2);
        t3 = abs(t3);
        t4 = abs(t4);

        if(t1 <= t2 || t1 <= t3 || t1 <= t4){
           return t1; 
        }

        else if(t2 <= t3 || t2 <= t4){
           return t2; 
        }

        else if(t3 <= t4){
           return t3; 
        }

        else{
           return t4; 
        }
	}
};

BallWithArrow ballWithArrow;


// Scheduled Function Calling
void timeScheduler(int t){
	double nextCollisionTime = 0;

	if(ballWithArrow.opMode == SIMULATION){
		ballWithArrow.checkCollisionAndChangeAngle();
		nextCollisionTime = ballWithArrow.calculateTimeForCollision();
	}

	glutTimerFunc(nextCollisionTime, timeScheduler, 0);
	glutPostRedisplay();
}    



void display() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW); 
 
   glLoadIdentity();      

   camera.startLooking();

   glPushMatrix();
   glRotatef(45, 0, 0, 1);
   glTranslatef(0, 0, -ballRadius); 
   drawCheckerboard();
   glPopMatrix(); 

   glPushMatrix();
   glRotatef(45 , 0, 0, 1);
   glTranslatef(0, 0, -ballRadius);
   drawWholeBoundary();
   glPopMatrix();

   glPushMatrix();
   glRotatef(45, 0, 0, 1);
   ballWithArrow.drawBallAndArrow();
   glPopMatrix();

//    if(ballWithArrow.opMode == SIMULATION){
//       ballWithArrow.goForward();  
//    }

   glutSwapBuffers();
}


void reshape(GLsizei width, GLsizei height) { 

   if (height == 0) 
    height = 1;    
   GLfloat aspect = (GLfloat)width / (GLfloat)height;

   glViewport(0, 0, width, height);
 
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity(); 
   
   gluPerspective(80.0f, aspect, 1.0f, 1000.0f);
}


void runSimulationMode(){
    if(ballWithArrow.opMode == SIMULATION){
       ballWithArrow.goForward(); 
    }

    glutPostRedisplay();
}


void keyboardListener(unsigned char key, int x, int y){
    switch(key){


        case '1':
            camera.lookLeft();
            break;

        case '2':
            camera.lookRight();
            break;

        case '3':
            camera.lookUp();
            break;

        case '4':
            camera.lookDown();
            break;

        case '5':
            camera.tiltCounterClockwise();
            break;

        case '6':
            camera.tiltClockwise();
            break; 

        case 'w':
            camera.moveUpWithoutChangingReferencePoint();
            break;

        case 's':
            camera.moveDownWithoutChangingReferencePoint();
            break;

        case 'j':
            ballWithArrow.rotateArrowCounterClockwise();
            // timeScheduler(0);
            break;    

        case 'l':
            ballWithArrow.rotateArrowClockwise();
            // timeScheduler(0);
            break;

        case 'i':
            if(ballWithArrow.opMode == MANUAL){
                ballWithArrow.goForward();
            }
            break;

        case 'k':
            if(ballWithArrow.opMode == MANUAL){
                ballWithArrow.goBackward();
            }
            break;

        case ' ':
            ballWithArrow.opMode = 1 - ballWithArrow.opMode;
            // timeScheduler(0);
            break;               

        default:
            break;
    }
    glutPostRedisplay();
}

void specialKeyboardListener(int key, int x, int y){
    switch(key){
        case GLUT_KEY_UP:
            camera.moveForward();
            break;

        case GLUT_KEY_DOWN:
            camera.moveBackward();
            break;

        case GLUT_KEY_RIGHT:
            camera.moveRight();
            break;

        case GLUT_KEY_LEFT:
            camera.moveLeft();
            break;

        case GLUT_KEY_PAGE_UP:
            camera.moveUp();
            break;

        case GLUT_KEY_PAGE_DOWN:
            camera.moveDown();
            break;

        default:
            break;            
    }
    glutPostRedisplay();
}


int main(int argc, char** argv){
   glutInit(&argc, argv);  
   glutInitWindowSize(window_width, window_height); 
   glutInitWindowPosition(window_x_pos, window_y_pos);
   glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
   glutCreateWindow(title);

   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);   
   glutIdleFunc(runSimulationMode);

//    glutTimerFunc(0, timeScheduler, 0);

   glutKeyboardFunc(keyboardListener);
   glutSpecialFunc(specialKeyboardListener);  
   
   initGL();  
   glutMainLoop();  
   return 0;
}