#include<GL/glut.h>
#include<bits/stdc++.h>
using namespace std;

// Point Structure
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


// Add Two Points
Point3D addTwoPoints(Point3D point1, Point3D point2){
    Point3D point3 = Point3D(point1.x_val + point2.x_val, point1.y_val + point2.y_val, point1.z_val + point2.z_val);
    return point3;
}


// Subtract Two Points
Point3D subtractTwoPoints(Point3D point1, Point3D point2){
    Point3D point3 = Point3D(point1.x_val - point2.x_val, point1.y_val - point2.y_val, point1.z_val - point2.z_val);
    return point3;
}


// Multiply Point with a Number
Point3D multiplyPointWithNumber(Point3D point1, double num){
    Point3D point2 = Point3D(point1.x_val * num, point1.y_val * num, point1.z_val * num);
    return point2;
}


// Divide Point with a Number
Point3D dividePointByNumber(Point3D point1, double num){
    Point3D point2 = Point3D(point1.x_val / num, point1.y_val / num, point1.z_val / num);
    return point2;
}


// Multiply Two Points
Point3D multiplyTwoPoints(Point3D point1, Point3D point2){
    Point3D point3 = Point3D(point1.y_val * point2.z_val - point1.z_val * point2.y_val, point1.z_val * point2.x_val - point1.x_val * point2.z_val, point1.x_val * point2.y_val - point1.y_val * point2.x_val);
    return point3;
}


// Global Variables
int window_width = 800, window_height = 800, window_x_pos = 70, window_y_pos = 70;
char title[] = "Magic Cube";
GLfloat rotateAngle = 45.0;
double maxLength = 1.5, curLength = 1.5, changeRate = 0.1, fixedRadius = 0.577, radius = 0;
int rowPoints = 100;


// Camera Implementation
struct Camera
{
    Point3D eye_pos, lookat_dir, right_dir, up_dir;

    // Constructor
    Camera(){
        eye_pos = Point3D(7, 0, 2);
        lookat_dir = Point3D(-1, 0, 0);
        right_dir = Point3D(0, 1, 0);
        up_dir = Point3D(0, 0, 1);
    }

    // Look Function
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

    // Position Change
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


// Rotate
void rotateClockwise(){
    rotateAngle += 5.0f;
}

void rotateCounterClockwise(){
    rotateAngle -= 5.0f;
}

Camera camera;


// Initialize OpenGL
void initGL(){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}


// Drawing Three Axes
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


// Drawing Base Triangle
void drawBaseTriangle(){
    glBegin(GL_TRIANGLES);
        glVertex3f(1, 0, 0);
        glVertex3f(0, 1, 0);
        glVertex3f(0, 0, 1);
    glEnd();    
}


// Ocathedron Upper Surface
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


// Full Octahedron
void drawWholeOctahedron(){
    glPushMatrix();
        drawOctahedronUpper();
    glPopMatrix();

    glPushMatrix();
        glRotatef(180, 0, 0, 1);
        drawOctahedronUpper();
    glPopMatrix();    
}


// Draw One Face of Sphere
void drawSphereFace(){
    struct Point3D points[rowPoints][rowPoints];

    // Generate Points
    for(int i = 0; i < rowPoints; i++){
        double tmp2 = (acos(-1) / 180.0f) * (45.0f - 90.0f * i / (rowPoints - 1));
        struct Point3D point1 = Point3D(-sin(tmp2), cos(tmp2), 0);

        for(int j = 0; j < rowPoints; j++){
            double tmp1 = (acos(-1) / 180.0f) * (-45.0f + 90.0f * j / (rowPoints - 1));
            struct Point3D point2 = Point3D(-sin(tmp1), 0, -cos(tmp1));
            points[i][j] = multiplyTwoPoints(point1, point2);

            double len = sqrt(points[i][j].x_val * points[i][j].x_val + points[i][j].y_val * points[i][j].y_val + points[i][j].z_val * points[i][j].z_val);
            points[i][j] = dividePointByNumber(points[i][j], len);
            points[i][j] = multiplyPointWithNumber(points[i][j], fixedRadius);
        }
    }

    //  Generate Sphere Face Using Quads
    for(int i =  0; i < rowPoints - 1; i++){
        for(int j = 0; j < rowPoints - 1; j++){
            glBegin(GL_QUADS);
                glVertex3f(points[i][j].x_val, points[i][j].y_val, points[i][j].z_val);
                glVertex3f(points[i][j + 1].x_val, points[i][j + 1].y_val, points[i][j + 1].z_val);
                glVertex3f(points[i + 1][j + 1].x_val, points[i + 1][j + 1].y_val, points[i + 1][j + 1].z_val);
                glVertex3f(points[i + 1][j].x_val, points[i + 1][j].y_val, points[i + 1][j].z_val);
            glEnd();
        }
    }
}


// Draw One Sphere
void drawOneSphere(){
    glPushMatrix();
        glTranslated(-curLength, 0, 0);
        glScaled(radius, radius, radius);
        drawSphereFace();
    glPopMatrix();
}


// Draw All Spheres
void drawAllSpheres(){
    glPushMatrix();
    for(int i = 0; i < 4; i++){
        if(i % 2 == 0){
            glColor3f(1, 1, 0);
        }
        else{
            glColor3f(1, 0, 1);
        }

        drawOneSphere();
        glRotated(90, 0, 1, 0);
    }
    glPopMatrix();

    glPushMatrix();
        glColor3f(0, 1, 1);
        glRotated(90, 0, 0, 1);
        drawOneSphere();
        glRotated(180, 0, 0, 1);
        drawOneSphere();
    glPopMatrix();
}


// Draw Cylinder Face
void drawCylinderFace(){
    struct Point3D points1[rowPoints + 1];
    double angleDiff = acos(-1) - acos(-1 / 3.0);
    double height = curLength * sqrt(2);

    // Generate Points
    for(int i = 0; i <= rowPoints; i++){
        double angleTheta = -angleDiff / 2 +  i * angleDiff / rowPoints;
        points1[i] = Point3D(fixedRadius * cos(angleTheta), fixedRadius * sin(angleTheta), height / 2);
    }

    // Generate Cylinder Face Using Quads
    for(int i = 0; i < rowPoints; i++){
        glBegin(GL_QUADS);
            glVertex3f(points1[i].x_val, points1[i].y_val, points1[i].z_val);
            glVertex3f(points1[i].x_val, points1[i].y_val, -points1[i].z_val);
            glVertex3f(points1[i + 1].x_val, points1[i + 1].y_val, -points1[i + 1].z_val);
            glVertex3f(points1[i + 1].x_val, points1[i + 1].y_val, points1[i + 1].z_val);
        glEnd();
    }
}


// Draw One Cylinder
void drawOneCylinder(){

    glPushMatrix();
        glRotatef(45, 0, 1, 0);
        glTranslatef(curLength / sqrt(2), 0, 0);
        glScaled(radius, radius, 1);
        drawCylinderFace();
    glPopMatrix();
}


// Draw Four Cylinders
void drawCylindersForOneAxis(){
    glPushMatrix();
    for(int i = 0; i < 4; i++){
        glRotatef(90, 0, 1, 0);
        drawOneCylinder();
    }
    glPopMatrix();
}


// Draw All Cylinders
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


// Display Function
void display() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);  
 
   glLoadIdentity();       

   camera.startLooking();

   glRotatef(rotateAngle, 0, 0, 1); 
   drawThreeAxes();
   drawWholeOctahedron();
   drawAllSpheres();
   drawAllCylinders();

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

// Keyboard Functionalities
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
            
            if( curLength > 0.0 ) {
                curLength -= 0.1;
                radius += 0.1;
            }
            break;

        case '.':
            
            if( curLength < maxLength ) {
                curLength += 0.1;
                radius -= 0.1;
            }
            break;               

        default:
            break;
    }
    glutPostRedisplay();
}


// Special Key Functionalities
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
   glutKeyboardFunc(keyboardListener); 
   glutSpecialFunc(specialKeyboardListener);  

   initGL();                    
   glutMainLoop();             
   return 0;
}