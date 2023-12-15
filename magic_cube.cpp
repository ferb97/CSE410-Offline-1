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

struct Point3D eye_pos, lookat_dir, right_dir, up_dir;
int window_width = 840, window_height = 840, window_x_pos = 50, window_y_pos = 50;
char title[] = "Magic Cube";
GLfloat rotateAngle = 45.0;
double maxLength = 1.6, curLength = 1.6, changeRate = 0.1;
int rowPoints = 100;
double maxRadius = maxLength / sqrt(3.0), radius = 0;
double step = maxRadius / 16.0;

struct Camera
{
    Point3D eye_pos, lookat_dir, right_dir, up_dir;

    Camera(){
        eye_pos = Point3D(7, 0, 2);
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

void rotateClockwise(){
    rotateAngle += 5.0f;
}

void rotateCounterClockwise(){
    rotateAngle -= 5.0f;
}

Camera camera;

void initGL(){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}

void drawThreeAxes(){
    glLineWidth(5);
    glBegin(GL_LINES);
        glColor3f(1, 1, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(2, 0, 0);

        glColor3f(0, 1, 1);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 2, 0);

        glColor3f(1, 0, 1);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, 2);
    glEnd();    
}

void drawBaseTriangle(){
    glBegin(GL_TRIANGLES);
        glVertex3f(1, 0, 0);
        glVertex3f(0, 1, 0);
        glVertex3f(0, 0, 1);
    glEnd();    
}

void drawOctahedronUpper(){

    double translation = (maxLength - curLength) / 3.0;
    for(int i = 0; i < 4; i++){
        if(i % 2 == 0){
           glColor3d(0, 0, 1); 
        }
        else{
           glColor3d(0, 1, 0); 
        }

        glPushMatrix();
            glRotatef(90 * i, 0, 1, 0);
            glTranslatef(translation, translation, translation);
            glScaled(curLength, curLength, curLength);
            drawBaseTriangle();
        glPopMatrix();
    }
}

void drawWholeOctahedron(){
    glPushMatrix();
        drawOctahedronUpper();
    glPopMatrix();

    glPushMatrix();
        glRotatef(180, 0, 0, 1);
        drawOctahedronUpper();
    glPopMatrix();    
}

void drawSphereFace(){
    struct Point3D points[rowPoints + 1][rowPoints + 1];

    for(int i = 0; i <= rowPoints; i++){
        // double tmp2 = (acos(-1) / 180.0f) * (45.0f - 90.0f * i / (rowPoints - 1));
        // struct Point3D point1 = Point3D(-sin(tmp2), cos(tmp2), 0);

        for(int j = 0; j <= rowPoints; j++){
            // double tmp1 = (acos(-1) / 180.0f) * (-45.0f + 90.0f * j / (rowPoints - 1));
            // struct Point3D point2 = Point3D(-sin(tmp1), 0, -cos(tmp1));
            // points[i][j] = multiplyTwoPoints(point1, point2);

            double x = -1 + (double)i / rowPoints * 2;
		    double y = -1 + (double)j / rowPoints * 2;
            points[i][j] = Point3D(x, y, 1);

            double len = sqrt(points[i][j].x_val * points[i][j].x_val + points[i][j].y_val * points[i][j].y_val + points[i][j].z_val * points[i][j].z_val);
            points[i][j] = dividePointByNumber(points[i][j], len);
            points[i][j] = multiplyPointWithNumber(points[i][j], radius);
        }
    }

        for(int i =  0; i < rowPoints; i++){
            for(int j = 0; j < rowPoints; j++){
                glBegin(GL_QUADS);
                glVertex3f(points[i][j].x_val, points[i][j].y_val, points[i][j].z_val);
                glVertex3f(points[i][j + 1].x_val, points[i][j + 1].y_val, points[i][j + 1].z_val);
                glVertex3f(points[i + 1][j + 1].x_val, points[i + 1][j + 1].y_val, points[i + 1][j + 1].z_val);
                glVertex3f(points[i + 1][j].x_val, points[i + 1][j].y_val, points[i + 1][j].z_val);
                glEnd();
            }
        }
}

void drawOneSphere(){
        glTranslated(0, 0, curLength);
        // glScaled(sphereRadius, sphereRadius, sphereRadius);
        drawSphereFace();
}

void drawAllSpheres(){
    for(int i = 0; i < 4; i++){
        if(i % 2 == 0){
            glColor3f(1, 0, 1);
        }
        else{
            glColor3f(1, 1, 0);
        }

        glPushMatrix();
            glRotated(i * 90, 0, 1, 0);
            drawOneSphere();
        glPopMatrix();
    }

    glColor3f(0, 1, 1);
    glPushMatrix();
        glRotated(90, 1, 0, 0);
        drawOneSphere();
    glPopMatrix();

    glPushMatrix();
        glRotated(270, 1, 0, 0);
        drawOneSphere();
    glPopMatrix();
}

void drawCylinderFace(){
    struct Point3D points1[rowPoints + 1];
    double angleDiff = 70.5287794 * acos(-1) / 180.0;
    double height = curLength * sqrt(2);

    for(int i = 0; i <= rowPoints; i++){
        double angleTheta = -angleDiff / 2 +  i * angleDiff / rowPoints;
        points1[i] = Point3D(radius * cos(angleTheta), radius * sin(angleTheta), height / 2);
    }

    for(int i = 0; i < rowPoints; i++){
        glBegin(GL_QUADS);
            glVertex3f(points1[i].x_val, points1[i].y_val, points1[i].z_val);
            glVertex3f(points1[i].x_val, points1[i].y_val, -points1[i].z_val);
            glVertex3f(points1[i + 1].x_val, points1[i + 1].y_val, -points1[i + 1].z_val);
            glVertex3f(points1[i + 1].x_val, points1[i + 1].y_val, points1[i + 1].z_val);
        glEnd();
    }
}

void drawOneCylinder(){

    glPushMatrix();
        glRotatef(45, 0, 1, 0);
        glTranslatef(curLength / sqrt(2), 0, 0);
        drawCylinderFace();
    glPopMatrix();
}

void drawCylindersForOneAxis(){
    glPushMatrix();
    for(int i = 0; i < 4; i++){
        glRotatef(90, 0, 1, 0);
        drawOneCylinder();
    }
    glPopMatrix();
}

void drawAllCylinders(){
    glPushMatrix();
    glColor3f(1.0, 0, 0);

    drawCylindersForOneAxis();

    glRotatef(90, 1, 0, 0);
    drawCylindersForOneAxis();

    glRotatef(90, 0, 0, 1);
    drawCylindersForOneAxis();
    glPopMatrix();
}

void display() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
   glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix
 
   // Render a color-cube consisting of 6 quads with different colors
   glLoadIdentity();                 // Reset the model-view matrix

   camera.startLooking();

   glRotatef(rotateAngle, 0, 0, 1); 
   drawThreeAxes();
   drawWholeOctahedron();
   drawAllSpheres();
   drawAllCylinders();

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

        case 'a':
            rotateClockwise();
            break;

        case 'd':
            rotateCounterClockwise();
            break;

        case ',':
            curLength -= 0.1;
            radius += step;
            if( curLength < 0 ) {
                curLength = 0;
                radius = maxRadius;
            }
            break;

        case '.':
            curLength += 0.1;
            radius -= step;
            if( curLength > maxLength ) {
                curLength = maxLength;
                radius = 0;
            }
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
   glutKeyboardFunc(keyboardListener);  // Register callback handler for keyboard Event
   glutSpecialFunc(specialKeyboardListener);    // Register callback handler for special key Event
   initGL();                       // Our own OpenGL initialization
   glutMainLoop();                 // Enter the infinite event-processing loop
   return 0;
}