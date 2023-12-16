#include<GL/glut.h>
#include<bits/stdc++.h>
using namespace std;

struct Point3D
{
    double x_val;
    double y_val;
    double z_val;

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

    Point3D(const Point3D& point){
        x_val = point.x_val;
        y_val = point.y_val;
        z_val = point.z_val;
    }
};

Point3D addTwoPoints(Point3D point1, Point3D point2){
    Point3D point3 = Point3D(point1.x_val + point2.x_val, point1.y_val + point2.y_val, point1.z_val + point2.z_val);
    return point3;
}

Point3D subtractTwoPoints(Point3D point1, Point3D point2){
    Point3D point3 = Point3D(point1.x_val - point2.x_val, point1.y_val - point2.y_val, point1.z_val - point2.z_val);
    return point3;
}

Point3D multiplyPointWithNumber(Point3D point1, double num){
    Point3D point2 = Point3D(point1.x_val * num, point1.y_val * num, point1.z_val * num);
    return point2;
}

Point3D dividePointByNumber(Point3D point1, double num){
    Point3D point2 = Point3D(point1.x_val / num, point1.y_val / num, point1.z_val / num);
    return point2;
}

Point3D multiplyTwoPoints(Point3D point1, Point3D point2){
    Point3D point3 = Point3D(point1.y_val * point2.z_val - point1.z_val * point2.y_val, point1.z_val * point2.x_val - point1.x_val * point2.z_val, point1.x_val * point2.y_val - point1.y_val * point2.x_val);
    return point3;
}

int window_width = 800, window_height = 800, window_x_pos = 70, window_y_pos = 70;
double changeRate = 0.1;
char title[] = "Rolling Ball";
GLfloat rotateAngle = 45.0;
double length = 10.0;
double boundaryLength = 5 * length * sqrt(2);
int totalLineSquares = 40, stacks = 30, sectors = 40, MANUAL = 0, SIMULATION = 1;
double ballRadius = 5.0;

struct Camera
{
    Point3D eye_pos, lookat_dir, right_dir, up_dir;

    Camera(){
        eye_pos = Point3D(150, -20, 60);
        lookat_dir = Point3D(-1, 0, 0);
        right_dir = Point3D(0, 1, 0);
        up_dir = Point3D(0, 0, 1);
    }

    void startLooking(){
        gluLookAt(eye_pos.x_val, eye_pos.y_val, eye_pos.z_val, eye_pos.x_val + lookat_dir.x_val, eye_pos.y_val + lookat_dir.y_val, eye_pos.z_val + lookat_dir.z_val, up_dir.x_val, up_dir.y_val, up_dir.z_val);
    }

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

    void lookLeft(){
        // struct Point3D point1, point2;

        // point1 = multiplyPointWithNumber(right_dir, cos(changeRate));
        // point2 = multiplyPointWithNumber(lookat_dir, sin(changeRate));
        // right_dir = addTwoPoints(point1, point2);

        // point1 = multiplyPointWithNumber(lookat_dir, cos(changeRate));
        // point2 = multiplyPointWithNumber(right_dir, sin(changeRate));
        // lookat_dir = subtractTwoPoints(point1, point2);
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

void initGL(){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}

// void drawSquare(double a)
// {
//     //glColor3f(1.0,0.0,0.0);
// 	glBegin(GL_QUADS);{
// 		glVertex3f(a/2, a/2, 0);
// 		glVertex3f(a/2, -a/2, 0);
// 		glVertex3f(-a/2, -a/2, 0);
// 		glVertex3f(-a/2, a/2, 0);
// 	}glEnd();
// }

void drawSingleSquare(){
    glBegin(GL_QUADS);
        glVertex3f(length/2, length/2, 0);
		glVertex3f(length/2, -length/2, 0);
		glVertex3f(-length/2, -length/2, 0);
		glVertex3f(-length/2, length/2, 0);
    glEnd();
}

// void lineOfSquares(int noOfSquares, int squareLength){
// 	for(int i = 0; i < noOfSquares; i++){		
// 		glPushMatrix();
// 		glRotatef(45 , 0, 0, 1);
// 		glTranslatef(i*squareLength*2, i*squareLength*2, 0);
// 		drawSquare(squareLength);
// 		glPopMatrix();   

// 		glPushMatrix();
// 		glRotatef(45 , 0, 0, 1);
// 		glTranslatef(-i*squareLength*2, -i*squareLength*2, 0);
// 		drawSquare(squareLength);
// 		glPopMatrix();   
// 	}
// }

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

// void drawBlackAndWhiteGrid(int squareLength){
// 	int noOfSquares = 20;

// 	for(int i = 0; i < noOfSquares; i++){
// 		glPushMatrix();
// 		glTranslatef(squareLength* sqrt(2) + i* 2*squareLength*sqrt(2), 0, 0);
// 		glColor3f(1.,1.,1.);
// 		lineOfSquares(noOfSquares, squareLength);
// 		glPopMatrix();


// 		glPushMatrix();
// 		glTranslatef(-squareLength* sqrt(2) -i* 2*squareLength*sqrt(2), 0, 0);
// 		glColor3f(1.,1.,1.);
// 		lineOfSquares(noOfSquares, squareLength);
// 		glPopMatrix();
// 	}
// }

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
            drawBoundaryLine(boundaryLength / sqrt(2), boundaryHeight);
            glRotated(90, 0, 0, 1);
        }
    glPopMatrix();
}

struct BallWithArrow{
    Point3D ballPos;
	int totalRotations;
    double v;
	int angle;
	int opMode;

    BallWithArrow(){
        ballPos = Point3D(0, 0, 0);
        totalRotations = 0;
        v = 0.6;
        angle = 45;
        opMode = MANUAL;
    }

    void drawWholeSphere(){
        struct Point3D points[stacks + 1][sectors + 1];

		for(int i = 0; i <= stacks; i++)
		{
			// h=radius*sin(((double)i/(double)stacks)*(acos(-1)/2));
			// r=radius*cos(((double)i/(double)stacks)*(acos(-1)/2));

            double height = ballRadius * sin(((i * 1.0) / stacks) * acos(-1) / 2);
            double r = ballRadius * cos(((i * 1.0) / stacks) * acos(-1) / 2);

			for(int j = 0; j <= sectors; j++)
			{
				// points[i][j].x_val=r*cos(((double)j/(double)slices)*2*acos(-1));
				// points[i][j].y_val=r*sin(((double)j/(double)slices)*2*acos(-1));
				// points[i][j].z_val=h;

                points[i][j] = Point3D(r * cos(((j * 1.0) / sectors) * 2 * acos(-1)), r * sin(((j * 1.0) / sectors) * 2 * acos(-1)), height);

			}
		}

        for(int i = 0; i < stacks; i++){
			int blueColor = 0;
			for(int j = 0; j < sectors; j++)
			{
				if(blueColor){
					glColor3f(0, 1, 0);
				}else{
					glColor3f(0, 0, 1);
				}
				glBegin(GL_QUADS);
					glVertex3f(points[i][j].x_val, points[i][j].y_val, points[i][j].z_val);
					glVertex3f(points[i][j + 1].x_val, points[i][j + 1].y_val, points[i][j + 1].z_val);
					glVertex3f(points[i + 1][j + 1].x_val, points[i + 1][j + 1].y_val, points[i + 1][j + 1].z_val);
					glVertex3f(points[i + 1][j].x_val, points[i + 1][j].y_val, points[i + 1][j].z_val);
				glEnd();

                if(j % (sectors / 20) == 0){
                   blueColor = 1 - blueColor; 
                }
            }
        }

        for(int i = 0; i < stacks; i++){
            int blueColor = 1;

            for(int j = 0; j < sectors; j++){
                if(blueColor){
					glColor3f(0, 1, 0);
				}else{
					glColor3f(0, 0, 1);
				}
				glBegin(GL_QUADS);
					glVertex3f(points[i][j].x_val, points[i][j].y_val, -points[i][j].z_val);
					glVertex3f(points[i][j + 1].x_val, points[i][j + 1].y_val, -points[i][j + 1].z_val);
					glVertex3f(points[i + 1][j + 1].x_val, points[i+1][j + 1].y_val, -points[i + 1][j + 1].z_val);
					glVertex3f(points[i + 1][j].x_val, points[i + 1][j].y_val, -points[i + 1][j].z_val);
				glEnd();

                if(j % (sectors / 20) == 0){
                   blueColor = 1 - blueColor; 
                }
            }
        }        

    }

    void drawBall(){

        int rotationAmount = totalRotations % 360;
        double radAngle = angle * acos(-1) / 180.0;
        glPushMatrix();
        glTranslatef(ballPos.x_val, ballPos.y_val, ballPos.z_val);
        glRotatef(-rotationAmount, sin(radAngle), -cos(radAngle), 1);
        drawWholeSphere();
        glPopMatrix();
    }

    void drawArrowLine(){
        glLineWidth(8);
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

    bool ballWithinBoundary(){
        double tmp1, tmp2, tmp3, tmp4;
        tmp1 = (ballPos.x_val + ballRadius) / boundaryLength + ballPos.y_val / boundaryLength - 1;
        tmp2 = (ballPos.x_val + ballRadius) / boundaryLength - ballPos.y_val / boundaryLength - 1;
        tmp3 = -(ballPos.x_val + ballRadius) / boundaryLength - ballPos.y_val / boundaryLength - 1;
        tmp4 = -(ballPos.x_val + ballRadius) / boundaryLength + ballPos.y_val / boundaryLength - 1;

        if(tmp1 > 0 || tmp2 > 0 || tmp3 > 0 || tmp4 > 0){
           return false; 
        }

        tmp1 = (ballPos.x_val - ballRadius) / boundaryLength + ballPos.y_val / boundaryLength - 1;
        tmp2 = (ballPos.x_val - ballRadius) / boundaryLength - ballPos.y_val / boundaryLength - 1;
        tmp3 = -(ballPos.x_val - ballRadius) / boundaryLength - ballPos.y_val / boundaryLength - 1;
        tmp4 = -(ballPos.x_val - ballRadius) / boundaryLength + ballPos.y_val / boundaryLength - 1;

        if(tmp1 > 0 || tmp2 > 0 || tmp3 > 0 || tmp4 > 0){
           return false; 
        }

        tmp1 = ballPos.x_val / boundaryLength + (ballPos.y_val + ballRadius) / boundaryLength - 1;
        tmp2 = ballPos.x_val / boundaryLength - (ballPos.y_val + ballRadius) / boundaryLength - 1;
        tmp3 = -ballPos.x_val / boundaryLength - (ballPos.y_val + ballRadius) / boundaryLength - 1;
        tmp4 = -ballPos.x_val / boundaryLength + (ballPos.y_val + ballRadius) / boundaryLength - 1;

        if(tmp1 > 0 || tmp2 > 0 || tmp3 > 0 || tmp4 > 0){
           return false; 
        }

        tmp1 = ballPos.x_val / boundaryLength + (ballPos.y_val - ballRadius) / boundaryLength - 1;
        tmp2 = ballPos.x_val / boundaryLength - (ballPos.y_val - ballRadius) / boundaryLength - 1;
        tmp3 = -ballPos.x_val / boundaryLength - (ballPos.y_val - ballRadius) / boundaryLength - 1;
        tmp4 = -ballPos.x_val / boundaryLength + (ballPos.y_val - ballRadius) / boundaryLength - 1;

        if(tmp1 > 0 || tmp2 > 0 || tmp3 > 0 || tmp4 > 0){
           return false; 
        }

        return true;

    }

    void goForward(){
        if(!ballWithinBoundary()){
           angle = (angle + 90) % 360; 
        }

        ballPos = addTwoPoints(ballPos, Point3D(v * cos(angle * acos(-1) / 180), v * sin(angle * acos(-1) / 180), 0));
        totalRotations += 10;
    }

    void goBackward(){
        if(!ballWithinBoundary()){
           angle = (angle + 180) % 360; 
        }

        ballPos = subtractTwoPoints(ballPos, Point3D(v * cos(angle * acos(-1) / 180), v * sin(angle * acos(-1) / 180), 0));
        totalRotations -= 5;
    }

    void rotateArrowCounterClockwise(){
        angle = (angle + 10) % 360;
    }

    void rotateArrowClockwise(){
        angle = (angle - 10) % 360;
    }
};

BallWithArrow ballWithArrow;



void display() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
   glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix
 
   // Render a color-cube consisting of 6 quads with different colors
   glLoadIdentity();                 // Reset the model-view matrix

   camera.startLooking();

//    glRotatef(rotateAngle, 0, 0, 1);

   glPushMatrix();
   glRotatef(45 , 0, 0, 1);
   glTranslatef(0, 0, -ballRadius); 
   drawCheckerboard();
//    lineOfSquares(30, 10);
   glPopMatrix(); 

   glPushMatrix();
   glRotatef(45 , 0, 0, 1);
   glTranslatef(0, 0, -ballRadius);
   drawWholeBoundary();
   glPopMatrix();

//    ball.setBoundary(boundaryLength);
//    ball.draw(); 
    ballWithArrow.drawBallAndArrow();

   glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)
}

void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
   // Compute aspect ratio of the new window
   if (height == 0) height = 1;                // To prevent divide by 0
   GLfloat aspect = (GLfloat)width / (GLfloat)height;
 
   // Set the viewport to cover the new window
   glViewport(0, 0, width, height);
 
   // Set the aspect ratio of the clipping volume to match the viewport
   glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
   glLoadIdentity();             // Reset
   // Enable perspective projection with fovy, aspect, zNear and zFar
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
            break;    

        case 'l':
            ballWithArrow.rotateArrowClockwise();
            break;

        case 'i':
            ballWithArrow.goForward();
            break;

        case 'k':
            ballWithArrow.goBackward();
            break;

        case ' ':
            ballWithArrow.opMode = 1 - ballWithArrow.opMode;
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
   glutInit(&argc, argv);            // Initialize GLUT
   glutInitWindowSize(window_width, window_height);   // Set the window's initial width & height
   glutInitWindowPosition(window_x_pos, window_y_pos); // Position the window's initial top-left corner
   glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB); // Enable double buffered mode
   glutCreateWindow(title);          // Create window with the given title
   glutDisplayFunc(display);       // Register callback handler for window re-paint event
   glutReshapeFunc(reshape);       // Register callback handler for window re-size event
   glutIdleFunc(runSimulationMode); // Simulation mode running
   glutKeyboardFunc(keyboardListener);  // Register callback handler for keyboard Event
   glutSpecialFunc(specialKeyboardListener);    // Register callback handler for special key Event
   initGL();                       // Our own OpenGL initialization
   glutMainLoop();                 // Enter the infinite event-processing loop
   return 0;
}