
#define GL_SILENCE_DEPRECATION
#include <windows.h>
#include <gl/GL.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "BmpLoader.h"


GLUquadric* quad;

using namespace std;

double Txval = 0, Tyval = 0, Tzval = 0;
double windowHeight = 1000, windowWidth = 1000;
GLfloat alpha = 0.0, theta = 0.0, roundswingAlpha = 0, roundswingTheta = 0.0, nepaliswingTheta = 0.0;
double eyeX = -10, eyeY = 5.0, eyeZ = 100, refX = 0, refY = 0, refZ = 0;
GLboolean  ferriswheelcheck = false, roundswingFlag = false,  nepaliswingFlag = false, nepaliswingCheck = false, day = true, switchOne = false, switchTwo = false;
unsigned int ID;
unsigned int ID2[40];

GLfloat yf = 0, xf = 0;
GLboolean yflag = false, xflag = false;

static GLfloat colors[4][6] =
{
    {1,0,0, 0.5,0,0}, //red
    {0,1,0, 0,0.5,0}, //green
    {0,0,1, 0,0,0.5}, //blue
    {1,1,0, 0.5,0.5,0} //yellow
};

static GLfloat v_cube[8][3] =
{
    {0.0, 0.0, 0.0}, //0
    {0.0, 0.0, 3.0}, //1
    {3.0, 0.0, 3.0}, //2
    {3.0, 0.0, 0.0}, //3
    {0.0, 3.0, 0.0}, //4
    {0.0, 3.0, 3.0}, //5
    {3.0, 3.0, 3.0}, //6
    {3.0, 3.0, 0.0}  //7
};

static GLubyte quadIndices[6][4] =
{
    {0, 1, 2, 3}, //bottom
    {4, 5, 6, 7}, //top
    {5, 1, 2, 6}, //front
    {3, 7, 4, 0}, // back
    {2, 3, 7, 6}, //right
    {0, 4, 5,1}  //left
};

void LoadTexture(const char* filename, int index)
{
    // Generate a texture ID and store it in the given index
    glGenTextures(1, &ID2[index]);

    // Bind the texture ID for configuration
    glBindTexture(GL_TEXTURE_2D, ID2[index]);

    // Set pixel storage mode (alignment) for data
    glPixelStorei(GL_UNPACK_ALIGNMENT, ID2[index]);

    // Set texture filtering options for minification and magnification
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Set texture wrapping options for S and T coordinates
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Load the texture data using a BMP loader class
    BmpLoader bl(filename);

    // Build texture mipmaps with specified parameters
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, bl.iWidth, bl.iHeight, GL_RGB, GL_UNSIGNED_BYTE, bl.textureData);
}
void materialProperty()
{
    // Set the current color to white
    glColor3f(1, 1, 1);

    // Material properties
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };       // No material property
    GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };   // Ambient reflection color
    GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };   // Diffuse reflection color
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };  // Specular reflection color
    GLfloat mat_shininess[] = { 60 };                 // Shininess value

    // Set material properties using OpenGL commands
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);     // Ambient reflection
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);     // Diffuse reflection
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);   // Specular reflection
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess); // Shininess
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);         // No emission

    // Note: GL_FRONT indicates that the material properties apply to the front face of polygons
}
void matCurve(GLfloat difX, GLfloat difY, GLfloat difZ, GLfloat ambfactor = 1.0, GLfloat specfactor = 1.0, GLfloat shine = 50)
{
    // Set the current color to white
    glColor3f(1, 1, 1);

    // Material properties
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };                             // No material property
    GLfloat mat_ambient[] = { difX * ambfactor, difY * ambfactor, difZ * ambfactor, 1.0 };  // Ambient reflection color
    GLfloat mat_diffuse[] = { difX, difY, difZ, 1.0 };                       // Diffuse reflection color
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };                        // Specular reflection color
    GLfloat mat_shininess[] = { shine };                                     // Shininess value

    // Set material properties using OpenGL commands
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);     // Ambient reflection
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);     // Diffuse reflection
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);   // Specular reflection
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess); // Shininess
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);         // No emission
}
static void getNormal3p(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2,GLfloat x3, GLfloat y3, GLfloat z3)
{
    // Calculate the vectors U and V between the points
    GLfloat Ux, Uy, Uz, Vx, Vy, Vz, Nx, Ny, Nz;

    Ux = x2 - x1; // U vector x component
    Uy = y2 - y1; // U vector y component
    Uz = z2 - z1; // U vector z component

    Vx = x3 - x1; // V vector x component
    Vy = y3 - y1; // V vector y component
    Vz = z3 - z1; // V vector z component

    // Calculate the cross product of U and V to get the normal vector
    Nx = Uy * Vz - Uz * Vy; // Normal x component
    Ny = Uz * Vx - Ux * Vz; // Normal y component
    Nz = Ux * Vy - Uy * Vx; // Normal z component

    // Set the calculated normal vector using OpenGL's glNormal3f function
    glNormal3f(Nx, Ny, Nz);
}

// Draws a solid cube with the given material properties.
void drawCube1(GLfloat difX, GLfloat difY, GLfloat difZ,GLfloat ambX = 0, GLfloat ambY = 0, GLfloat ambZ = 0,GLfloat shine = 50)
{
    // Material properties
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };           // No material property
    GLfloat mat_ambient[] = { ambX, ambY, ambZ, 1.0 };   // Ambient reflection color
    GLfloat mat_diffuse[] = { difX, difY, difZ, 1.0 };   // Diffuse reflection color
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };    // Specular reflection color
    GLfloat mat_shininess[] = { shine };                // Shininess value

    // Set material properties using OpenGL commands
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);     // Ambient reflection
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);     // Diffuse reflection
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);   // Specular reflection
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess); // Shininess
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);         // No emission

    // Begin drawing quads for the cube's faces
    glBegin(GL_QUADS);

    for (GLint i = 0; i < 6; i++)
    {
        // Calculate and set the normal for the current quad face
        getNormal3p(v_cube[quadIndices[i][0]][0], v_cube[quadIndices[i][0]][1], v_cube[quadIndices[i][0]][2],
            v_cube[quadIndices[i][1]][0], v_cube[quadIndices[i][1]][1], v_cube[quadIndices[i][1]][2],
            v_cube[quadIndices[i][2]][0], v_cube[quadIndices[i][2]][1], v_cube[quadIndices[i][2]][2]);

        // Define vertices of the quad
        glVertex3fv(&v_cube[quadIndices[i][0]][0]);
        glVertex3fv(&v_cube[quadIndices[i][1]][0]);
        glVertex3fv(&v_cube[quadIndices[i][2]][0]);
        glVertex3fv(&v_cube[quadIndices[i][3]][0]);
    }

    // End drawing quads
    glEnd();
}


// Draws a solid sphere with the given material properties.
void drawSphere(GLfloat difX, GLfloat difY, GLfloat difZ, GLfloat ambX, GLfloat ambY, GLfloat ambZ, GLfloat shine = 90)
{
    // Material properties
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { ambX, ambY, ambZ, 1.0 };
    GLfloat mat_diffuse[] = { difX, difY, difZ, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { shine };

    // Set material properties using OpenGL commands
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

    // Draw the solid sphere
    glutSolidSphere(1.0, 16, 16);
}

// Draws a solid torus with the given material properties.
void drawTorus(GLfloat difX, GLfloat difY, GLfloat difZ, GLfloat ambX, GLfloat ambY, GLfloat ambZ, GLdouble innerRadius, GLdouble outerRadius, GLint nsides, GLint rings, GLfloat shine = 90)
{
    // Material properties
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { ambX, ambY, ambZ, 1.0 };
    GLfloat mat_diffuse[] = { difX, difY, difZ, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { shine };

    // Set material properties using OpenGL commands
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

    // Draw the solid torus
    glutSolidTorus(innerRadius, outerRadius, nsides, rings);
}

// Draws a solid cylinder with the given material properties.
void drawCylinder(GLfloat difX, GLfloat difY, GLfloat difZ, GLfloat ambX, GLfloat ambY, GLfloat ambZ, GLfloat shine = 90)
{
    // Material properties
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { ambX, ambY, ambZ, 1.0 };
    GLfloat mat_diffuse[] = { difX, difY, difZ, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { shine };

    // Set material properties using OpenGL commands
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

    // Draw the solid cylinder
    GLUquadricObj* quadratic;
    quadratic = gluNewQuadric();
    gluCylinder(quadratic, 1.5, 1.5, 19, 32, 32);
}

static GLfloat v_box[8][3] =
{
    {0.0, 0.0, 0.0}, //0
    {3.0, 0.0, 0.0}, //1
    {0.0, 0.0, 3.0}, //2
    {3.0, 0.0, 3.0}, //3
    {0.0, 3.0, 0.0}, //4
    {3.0, 3.0, 0.0}, //5
    {0.0, 3.0, 3.0}, //6
    {3.0, 3.0, 3.0}, //7

};

static GLubyte BoxquadIndices[6][4] =
{
    {0,2,3,1},
    {0,2,6,4},
    {2,3,7,6},
    {1,3,7,5},
    {1,5,4,0},
    {6,7,5,4}
};

// Draws a box with texture coordinates using vertex data from v_box and quad indices from BoxquadIndices.
void drawBox()
{
    // Set material properties
    materialProperty();

    // Begin drawing quads for the box's faces
    glBegin(GL_QUADS);

    for (GLint i = 0; i < 6; i++)
    {
        // Calculate and set the normal for the current quad face
        getNormal3p(v_box[BoxquadIndices[i][0]][0], v_box[BoxquadIndices[i][0]][1], v_box[BoxquadIndices[i][0]][2],
            v_box[BoxquadIndices[i][1]][0], v_box[BoxquadIndices[i][1]][1], v_box[BoxquadIndices[i][1]][2],
            v_box[BoxquadIndices[i][2]][0], v_box[BoxquadIndices[i][2]][1], v_box[BoxquadIndices[i][2]][2]);

        // Define vertices of the quad and corresponding texture coordinates
        glVertex3fv(&v_box[BoxquadIndices[i][0]][0]); glTexCoord2f(1, 1);
        glVertex3fv(&v_box[BoxquadIndices[i][1]][0]); glTexCoord2f(1, 0);
        glVertex3fv(&v_box[BoxquadIndices[i][2]][0]); glTexCoord2f(0, 0);
        glVertex3fv(&v_box[BoxquadIndices[i][3]][0]); glTexCoord2f(0, 1);
    }

    // End drawing quads
    glEnd();
}

void light()
{
    GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position[] = { 20.0, 50.0, 0.0, 1.0 };

    
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

}
void sky(float x, float y, float z, float width, float height, float length)
{
    materialProperty();
    glDisable(GL_DEPTH_TEST);
    x = x - width / 2;
    y = y - height / 2;
    z = z - length / 2;


    //front
    glEnable(GL_TEXTURE_2D);
    if (day == true)
    {
        glBindTexture(GL_TEXTURE_2D, ID2[8]);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, ID2[28]);
    }
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z + length);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
    glEnd();

    //up;
    if (day == true)
    {
        glBindTexture(GL_TEXTURE_2D, ID2[27]);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, ID2[28]);
    }
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y + height, z + length);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
    glEnd();

    //back

    if (day == true)
    {
        glBindTexture(GL_TEXTURE_2D, ID2[9]);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, ID2[28]);
    }
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z + length);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z + length);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);
    glEnd();

    //right
    if (day == true)
    {
        glBindTexture(GL_TEXTURE_2D, ID2[10]);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, ID2[28]);
    } //10
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
    glEnd();

    //left

    if (day == true)
    {
        glBindTexture(GL_TEXTURE_2D, ID2[11]);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, ID2[28]);
    } //11
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z + length);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z + length);
    glEnd();

    glDisable(GL_TEXTURE_2D);

    
}
void ground()
{
    // Enable 2D texture mapping
    glEnable(GL_TEXTURE_2D);

    // Bind a specific texture (ID2[14]) for rendering
    glBindTexture(GL_TEXTURE_2D, ID2[14]);

    // Save the current transformation matrix onto the stack
    glPushMatrix();

    // Set up material properties (not shown here)
    materialProperty();

    // Begin defining vertices for a quadrilateral
    glBegin(GL_QUADS);

    // Define the first corner of the quadrilateral
    glTexCoord2f(0.0f, 0.0f); glVertex3f(90, -20, 60);

    // Define the second corner of the quadrilateral
    glTexCoord2f(5.0f, 0.0f); glVertex3f(90, -20, -75);

    // Define the third corner of the quadrilateral
    glTexCoord2f(5.0f, 5.0f); glVertex3f(-75, -20, -75);

    // Define the fourth corner of the quadrilateral
    glTexCoord2f(0.0f, 5.0f); glVertex3f(-75, -20, 60);



    // Finish defining vertices for the quadrilateral
    glEnd();

    // Pop the transformation matrix from the stack, undoing transformations
    glPopMatrix();

    // Disable 2D texture mapping
    glDisable(GL_TEXTURE_2D);
}
void ground2()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, ID2[2]); //2
    glPushMatrix();
    materialProperty();
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(10, -19.8, 10);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(10, -19.8, -10);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-10, -19.8, -10);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-10, -19.8, 10);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}//secondary base for all
void ferrisWheelSeat()
{
    //seat
    glPushMatrix();
    glTranslatef(0, -0.5, 0);
    glScalef(0.5, 0.2, 1.5);
    drawCube1(0.804, 0.361, 0.361, 0.403, 0.1805, 0.1805);
    glPopMatrix();

    //seat belt rod
    glPushMatrix();
    glTranslatef(1.3, 0.7, 0);
    glScalef(0.02, 0.02, 1.5);
    drawCube1(0, 0, 0, 0, 0, 0.0);
    glPopMatrix();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, ID2[4]);

    //back
    glPushMatrix();
    glScalef(0.2, 0.5, 1.5);
    drawBox();
    glPopMatrix();

   // seat right side
    glPushMatrix();
    glScalef(0.5, 0.5, 0.02);
    drawBox();
    glPopMatrix();

    //seat left side
    glPushMatrix();
    glTranslatef(0, 0, 4.445);
    glScalef(0.5, 0.5, 0.02);
    drawBox();
    glPopMatrix();

    //bottom bent part
    glPushMatrix();
    glTranslatef(1.48, -0.5, 0);
    glRotatef(-45, 0, 0, 1);
    glScalef(0.15, 0.02, 1.5);
    drawBox();
    glPopMatrix();

    //bottom straight part
    glPushMatrix();
    glTranslatef(1.8, -0.8, 0);
    glRotatef(-20, 0, 0, 1);
    glScalef(0.16, 0.02, 1.5);
    drawBox();
    glPopMatrix();

    //bottom farthest part
    glPushMatrix();
    glTranslatef(2.25, -0.8, 0);
    glScalef(0.02, 0.1, 1.5);
    drawBox();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

}
void wheel()
{
    glPushMatrix();
    glScalef(1, 1, 2);
    drawSphere(1, 0, 0, 0.5, 0, 0);
    glPopMatrix();
    glPushMatrix();
    drawTorus(0.0, 0.0, 1.0, 0.0, 0.0, 0.5, 0.5, 10.0, 32, 64);
    glPopMatrix();


    //the big lines
    for (int i = 0; i < 180; i += 45)
    {
        glPushMatrix();
        glRotatef(i, 0, 0, 1);
        glScalef(6.6, 0.1, 0.5);
        glTranslatef(-1.5, -1.5, -1.5);
        drawCube1(0.5, 0.5, 0.0, 0.25, 0.25, 0.0);
        glPopMatrix();
    }

}
void ferrisWheel()
{
    //right stand on the back
    glPushMatrix();
    glTranslatef(-.2, 0, -1);
    glRotatef(-75, 0, 0, 1);
    glScalef(7, 0.28, 0.1);
   
    drawCube1(0.2, 0.1, 0.1, 0.1, 0.05, 0.05);

    glPopMatrix();

   // left stand on the back
    glPushMatrix();
    glTranslatef(-0.6, 0, -1);
    glRotatef(-105, 0, 0, 1);
    glScalef(7, 0.28, 0.1);
    
    drawCube1(0.2, 0.1, 0.1, 0.1, 0.05, 0.05);
    glPopMatrix();

    //right stand on the front
    glPushMatrix();
    glTranslatef(-.2, 0, 6);
    glRotatef(-75, 0, 0, 1);
    glScalef(7, 0.28, 0.1);
    drawCube1(0.2, 0.1, 0.1, 0.1, 0.05, 0.05);
    glPopMatrix();

    //left stand on the front
    glPushMatrix();
    glTranslatef(-0.6, 0, 6);
    glRotatef(-105, 0, 0, 1);
    glScalef(7, 0.28, 0.1);
    drawCube1(0.2, 0.1, 0.1, 0.1, 0.05, 0.05);
    glPopMatrix();

    //base stand
    glPushMatrix();
    glTranslatef(0, -19.5, 2.5);
    glScalef(8, 0.5, 6);
    glTranslatef(-1.5, -1.5, -1.5);
    drawCube1(0.545, 0.271, 0.075, 0.2725, 0.1355, 0.0375);
    glPopMatrix();

    //rotating part
    glPushMatrix();
    glRotatef(-theta, 0, 0, 1);
    glScalef(1.5, 1.5, 1);
    wheel();

    glPushMatrix();
    glTranslatef(0, 0, 5);
    wheel();
    glPopMatrix();

    //the middle orange ring
    glPushMatrix();
    glTranslatef(0, 0, 2.5);
   
    glScalef(0.5, 0.5, 4);
    drawTorus(1.0, 0.5, 0.0, 0.5, 0.25, 0.0, 0.5, 10.0, 32, 64);
    glPopMatrix();

    //the middle line between two spheres
    glPushMatrix();
    glScalef(0.1, 0.05, 1.5);
    drawCube1(0.2, 0.2, 0.2, 0.1, 0.1, 0.1);
    glPopMatrix();


    //the seats
    for (int i = 0; i < 360; i += 30)
    {
        glPushMatrix();
        glRotatef(i, 0, 0, 1);
        glTranslatef(10, 0, 0.9);
        glRotatef(-i, 0, 0, 1);
        glRotatef(theta, 0, 0, 1);
        glScalef(1, 1, 0.8);
        ferrisWheelSeat();
        glPopMatrix();
    }

    glPopMatrix();

    //ground
    glPushMatrix();
    glTranslatef(2, 0, 0);
    glScalef(2, 1, 1.5);
    ground2();
    glPopMatrix();

}
void bush()
{
    quad = gluNewQuadric();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, ID2[24]);

    glPushMatrix();
    gluQuadricTexture(quad, 1);
    gluSphere(quad, 1, 100, 100);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}
void tree()
{
    quad = gluNewQuadric();
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, ID2[30]);

    glPushMatrix();
    glScalef(1, 2, 1);
    gluQuadricTexture(quad, 1);
    gluSphere(quad, 4, 100, 100);
    glPopMatrix();



    glBindTexture(GL_TEXTURE_2D, ID2[29]);
    glPushMatrix();
    glTranslatef(0, -7, 0);
    glRotatef(90, 1, 0, 0);
    gluQuadricTexture(quad, 1);
    gluCylinder(quad, 1, 1, 10, 32, 32);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}
void body()
{
    //head
    drawSphere(0.5, 0.5, 0.0, 0.15, 0.15, 0.05);

    //neck
    glPushMatrix();
    glRotatef(90, 1, 0, 0);
    glScalef(0.2, 0.2, 0.1);
    drawCylinder(0.396, 0.263, 0.129, 0.198, 0.1315, 0.0645);

    glPopMatrix();

    //body

    glPushMatrix();
    glTranslatef(-0.7, -7.5, -0.5);
    glScalef(0.5, 2, 0.3);
    drawCube1(0.0, 0.0, 0.5, 0.0, 0.0, 0.25);

    glPopMatrix();

    //legs
    glPushMatrix();
    glTranslatef(-0.4, -7, 0);
    glRotatef(90, 1, 0, 0);
    glScalef(0.15, 0.15, 0.25);
    drawCylinder(0.396, 0.263, 0.129, 0.198, 0.1315, 0.0645);

    glPopMatrix();
    //legs
    glPushMatrix();
    glTranslatef(0.4, -7, 0);
    glRotatef(90, 1, 0, 0);
    glScalef(0.15, 0.15, 0.25);
    drawCylinder(0.396, 0.263, 0.129, 0.198, 0.1315, 0.0645);

    glPopMatrix();

    //arms
    glPushMatrix();
    glTranslatef(0, -2.5, -0.1);
    glRotatef(90, 0, 1, 0);
    glScalef(0.25, 0.25, 0.15);
    drawCylinder(0.396, 0.263, 0.129, 0.198, 0.1315, 0.0645);

    glPopMatrix();

    //arms
    glPushMatrix();
    glTranslatef(0, -2.5, -0.1);
    glRotatef(-90, 0, 1, 0);
    glScalef(0.25, 0.25, 0.15);
    drawCylinder(0.396, 0.263, 0.129, 0.198, 0.1315, 0.0645);
    glPopMatrix();

    //shoes
    glPushMatrix();
    glTranslatef(0.145, -12, -0.3);
    glScalef(0.2, 0.25, 0.4);
    drawCube1(0.2, 0.2, 0.2, 0.1, 0.1, 0.1);

    glPopMatrix();

    //shoes
    glPushMatrix();
    glTranslatef(-0.69, -12, -0.3);
    glScalef(0.2, 0.25, 0.4);
    drawCube1(0.2, 0.2, 0.2, 0.1, 0.1, 0.1);

    glPopMatrix();


    //hands
    glPushMatrix();
    glTranslatef(3, -2.5, -0.1);
    glRotatef(90, 0, 1, 0);
    glScalef(0.5, 0.5, 0.5);
    drawSphere(0.2, 0.2, 0.2, 0.1, 0.1, 0.1);

    glPopMatrix();

    //hands
    glPushMatrix();
    glTranslatef(-3, -2.5, -0.1);
    glRotatef(-90, 0, 1, 0);
    glScalef(0.5, 0.5, 0.5);
    drawSphere(0.2, 0.2, 0.2, 0.1, 0.1, 0.1);

    glPopMatrix();

    //legs pant
    glPushMatrix();
    glTranslatef(-0.4, -7, 0);
    glRotatef(90, 1, 0, 0);
    glScalef(0.2, 0.2, 0.15);
    drawCylinder(0.8, 0.8, 0.0, 0.5, 0.5, 0.0);
    glPopMatrix();

    //legs pant
    glPushMatrix();
    glTranslatef(0.4, -7, 0);
    glRotatef(90, 1, 0, 0);
    glScalef(0.2, 0.2, 0.15);
    drawCylinder(0.8, 0.8, 0.0, 0.5, 0.5, 0.0);
    glPopMatrix();

    //arms sheeve
    glPushMatrix();
    glTranslatef(0, -2.5, -0.1);
    glRotatef(90, 0, 1, 0);
    glScalef(0.3, 0.3, 0.1);
    drawCylinder(1.0, 0.5, 0.0, 0.5, 0.25, 0.0);
    glPopMatrix();

    //arms sleeve
    glPushMatrix();
    glTranslatef(0, -2.5, -0.1);
    glRotatef(-90, 0, 1, 0);
    glScalef(0.3, 0.3, 0.1);
    drawCylinder(1.0, 0.5, 0.0, 0.5, 0.25, 0.0);


    glPopMatrix();
}
void round_swing()
{
    //the grass base
    glPushMatrix();
    glScalef(2, 1, 2);
    ground2();
    glPopMatrix();

    //the base
    glPushMatrix();
    glTranslatef(0, -19.5, 0);
    glScalef(10, 0.5, 10);
    glTranslatef(-1.5, -1.5, -1.5);
    drawCube1(0.545, 0.271, 0.075, 0.2725, 0.1355, 0.0375);
    glPopMatrix();

    //default rotation for this function
    glPushMatrix();
    glRotatef(90, 1, 0, 0);

    //center cylinder
    glPushMatrix();
    glTranslatef(0.0, 0, -12);
    glScalef(0.75, 0.75, 1.7);
    drawCylinder(0.2, 0.1, 0.1, 0.1, 0.05, 0.05);
    glPopMatrix();

    //cube in the bottom
    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    glTranslatef(-1.5, -20, -1.5);
    glScalef(1, 1.5, 1);
    drawCube1(0.4, 0.4, 0.4, 0.25, 0.25, 0.25);
    glPopMatrix();


    //for the rotation part
    glPushMatrix();
    glRotatef(roundswingTheta, 0, 0, 1);

    
    //sphere on top
    glPushMatrix();
    glTranslatef(0.0, 0, -12);
    glScalef(2, 2, 2);
    drawSphere(0, 0, 1, 0, 0, 0.5);
    glPopMatrix();


    //toros
    glPushMatrix();
    glTranslatef(0, 0, -12);
    glScalef(2, 2, 2);
    drawTorus(0.8, 0, 0, 0.5, 0, 0, 0.6, 7, 32, 64);
    glPopMatrix();

    //torus connector flaps
    for (int i = 0; i < 360; i += 45)
    {
        glPushMatrix();
        glRotatef(i, 0, 0, 1);
        glTranslatef(0, 0, -13);
        glScalef(4.8, 0.4, 0.4);
        drawCube1(0.3, 0.3, 0.3, 0.1, 0.1, 0.1);
        glPopMatrix();
    }

    //torus strings and seat and bearing sphere
    for (int i = 0; i < 360; i += 45)
    {
        glPushMatrix();

        glRotatef(i, 0, 0, 1);
        glTranslatef(-13.5, 0, -12);


        glPushMatrix();//for the vertical rotation of the swing
        glRotatef(-roundswingAlpha, 0, 1, 0);
        //the rope
        glPushMatrix();
        glScalef(0.15, 0.15, 1.2);
        drawCylinder(0.4, 0.4, 0, 0.2, 0.2, 0);
        glPopMatrix();

        //the bearning sphere
        glPushMatrix();
        glScalef(0.8, 0.8, 0.8);
        glTranslatef(0, 0, 1);
        drawSphere(0.8, 0.8, 0.0, 0.4, 0.4, 0.0);
        glPopMatrix();


        //seats
        glPushMatrix();
        glRotatef(-90, 1, 0, 0);
        glRotatef(180, 0, 1, 0);
        glTranslatef(0, -23, -2);
        ferrisWheelSeat();
        glPopMatrix();
        glPopMatrix();

        glPopMatrix();// for the vertical rotation of swing

    }

    glPopMatrix();// for the rotation part

    glPopMatrix();//main
}
void nepali_swing()
{

    //base
    glPushMatrix();
    glTranslatef(1, 0, 0);
    glScalef(1.4, 1, 1.4);
    ground2();
    glPopMatrix();


    //base stand
    glPushMatrix();
    glTranslatef(-11.5, -19.5, -10);
    glScalef(8, 0.2, 6);
    drawCube1(0.545, 0.271, 0.075, 0.2725, 0.1355, 0.0375);
    glPopMatrix();



    //base stand at the front
    glPushMatrix();
    glTranslatef(0, 20, 5);


    //sphere

    glPushMatrix();
    glTranslatef(0, 0, 0); //
    drawSphere(0, 0, 1, 0, 0, 0.5);
    glPopMatrix();



    //left stand
    glPushMatrix();
    glRotatef(-75, 0, 0, 1);
    glScalef(15, 0.28, 0.1);
    // drawCube1(0.2, 0.1, 0.1, 0.1, 0.05, 0.05);
    drawCube1(0.0, 0.2, 0.0, 0.0, 0.1, 0.0);

    glPopMatrix();

    //right stand
    glPushMatrix();
    glRotatef(-105, 0, 0, 1);
    glScalef(15, 0.28, 0.1);
    //drawCube1(0.2, 0.1, 0.1, 0.1, 0.05, 0.055);
    drawCube1(0.0, 0.2, 0.0, 0.0, 0.1, 0.0);

    glPopMatrix();

    glPopMatrix();


    //upper part

    glPushMatrix();
    glTranslatef(0, 20, 5);


    //left stand up
    glPushMatrix();

    glTranslatef(1, 0, 0);

    glRotatef(-75, 0, 0, 1);
    glRotatef(180, 0, 1, 0);
    glRotatef(180, 1, 0, 0);
    glScalef(5, 0.28, 0.1);
    //drawCube1(0.2, 0.1, 0.1, 0.1, 0.05, 0.05);
    drawCube1(0.0, 0.2, 0.0, 0.0, 0.1, 0.0);

    glPopMatrix();

    //right stand up
    glPushMatrix();

    glTranslatef(1, 0, 0);

    glRotatef(-105, 0, 0, 1);
    glRotatef(180, 0, 1, 0);
    glRotatef(180, 1, 0, 0);
    glScalef(5, 0.28, 0.1);
    //drawCube1(0.2, 0.1, 0.1, 0.1, 0.05, 0.055);
    drawCube1(0.0, 0.2, 0.0, 0.0, 0.1, 0.0);

    glPopMatrix();

    glPopMatrix();


    //base stand at the back
    glPushMatrix();
    glTranslatef(0, 20, -5);

    //sphere
    glPushMatrix();
    glTranslatef(0.0, 0, 0);//
    drawSphere(0, 0, 1, 0, 0, 0.5);
    glPopMatrix();


    //left stand 
    glPushMatrix();
    glRotatef(-75, 0, 0, 1);
    glScalef(15, 0.28, 0.1);
    //drawCube1(0.2, 0.1, 0.1, 0.1, 0.05, 0.05);
    drawCube1(0.0, 0.2, 0.0, 0.0, 0.1, 0.0);

    glPopMatrix();

    //right stand 
    glPushMatrix();
    glRotatef(-105, 0, 0, 1);
    glScalef(15, 0.28, 0.1);
    //drawCube1(0.2, 0.1, 0.1, 0.1, 0.05, 0.05);
    drawCube1(0.0, 0.2, 0.0, 0.0, 0.1, 0.0);

    glPopMatrix();

    glPopMatrix();
    //upper part

    glPushMatrix();
    glTranslatef(0, 20, -5);


    //left stand up
    glPushMatrix();

    glTranslatef(1, 0, 0);

    glRotatef(-75, 0, 0, 1);
    glRotatef(180, 0, 1, 0);
    glRotatef(180, 1, 0, 0);
    glScalef(5, 0.28, 0.1);
    //drawCube1(0.2, 0.1, 0.1, 0.1, 0.05, 0.05);
    drawCube1(0.0, 0.2, 0.0, 0.0, 0.1, 0.0);

    glPopMatrix();

    //right stand up
    glPushMatrix();

    glTranslatef(1, 0, 0);

    glRotatef(-105, 0, 0, 1);
    glRotatef(180, 0, 1, 0);
    glRotatef(180, 1, 0, 0);
    glScalef(5, 0.28, 0.1);
    drawCube1(0.0, 0.2, 0.0, 0.0, 0.1, 0.0);

    glPopMatrix();

    glPopMatrix();


    //middle wood in the swing
    glPushMatrix();
    glTranslatef(0, 20, -10);//

    glPushMatrix();
    glScalef(0.35, 0.35, 1);
    drawCylinder(0.2, 0.1, 0.1, 0.1, 0.05, 0.05);
    glPopMatrix();

    glPopMatrix();


    //FOR THE ROTATION OF THE SWING

    glPushMatrix();

    glTranslatef(0, 20, -10);
    glRotatef(nepaliswingTheta, 0, 0, 1);


    //torus in the swing
    glPushMatrix();
    glTranslatef(0, 0, 7.5);//
    drawTorus(1, 0, 0, 0.5, 0, 0, 0.2, 0.5, 32, 64);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0, 12.5);//
    drawTorus(1, 0, 0, 0.5, 0, 0, 0.2, 0.5, 32, 64);
    glPopMatrix();

    //rope in the swing

    glPushMatrix();
    glTranslatef(0, 0, 7.5);//
    glRotatef(90, 1, 0, 0);
    glScalef(0.075, 0.075, 1.8);
    //drawCylinder(0.2, 0.1, 0.1, 0.1, 0.05, 0.05);
    //drawCylinder(0.5, 0.5, 0.0, 0.25, 0.125, 0.125);
    drawCylinder(0.2, 0.1, 0.0, 0.1, 0.05, 0.05);


    glPopMatrix();

    //rope in the swing
    glPushMatrix();
    glTranslatef(0, 0, 12.5);//
    glRotatef(90, 1, 0, 0);
    glScalef(0.075, 0.075, 1.8);
    //drawCylinder(0.2, 0.1, 0.1, 0.1, 0.05, 0.05);
   // drawCylinder(0.5, 0.5, 0.0, 0.25, 0.125, 0.125);
    drawCylinder(0.2, 0.1, 0.0, 0.1, 0.05, 0.05);

    glPopMatrix();



    //swing seat


    glPushMatrix();
    glTranslatef(-0.5, -20 - 14, 13);
    glScalef(0.3, 0.15, 2);
    glRotatef(90, 0, 1, 0);
    drawCube1(0.6, 0.4, 0.2, 0.3, 0.2, 0.1);
    glPopMatrix();



    //body
    glPushMatrix();
    glTranslatef(0, -21.5, 10);
    glScalef(1, 1, 0.85);
    glRotatef(90, 0, 1, 0);
    body();
    glPopMatrix();


    glPopMatrix();//for the rotation of swing
}
void trees()
{
   
    glPushMatrix();
    glTranslatef(45, -2, -30);
    tree();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(45, -2, -10);
    tree();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(55, -2, -30);
    tree();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(55, -2, -10);
    tree();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(-35, -2, -30);
    tree();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(-35, -2, -10);
    tree();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(-45, -2, -30);
    tree();
    glPopMatrix();

   
    glPushMatrix();
    glTranslatef(-45, -2, -10);
    tree();
    glPopMatrix();












}
void streetLampbody()
{
    glPushMatrix();
    glRotatef(90, 1, 0, 0);
    glScalef(0.5, 0.5, 1);
    drawCylinder(0.1, 0.1, 0.1, 0.05, 0.05, 0.05);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, -19, 0);
    glRotatef(90, 1, 0, 0);
    glScalef(0.2, 0.2, 0.2);
    drawTorus(0.1, 0.1, 0.1, 0.05, 0.05, 0.05, 2, 5, 32, 64);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0, 0);
    glRotatef(90, 1, 0, 0);
    glScalef(0.2, 0.2, 0.2);
    drawTorus(0.1, 0.1, 0.1, 0.05, 0.05, 0.05, 2, 5, 32, 64);
    glPopMatrix();
}
void spotLight1()
{
    glPushMatrix();

    GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position[] = { -25, 20, -30, 1.0 };

    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);

    glLightfv(GL_LIGHT1, GL_POSITION, light_position);
    GLfloat spot_direction[] = { -0.5, -1, 0 };
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 70.0);
    glPopMatrix();
}
void streetLight1()
{
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
    GLfloat mat_ambient_color[] = { 0.8, 0.8, 0.2, 1.0 };
    GLfloat mat_diffuse[] = { 1.0, 0.0, 0.0, 1.0 };  // Green color when light is off
    GLfloat high_shininess[] = { 100.0 };
    GLfloat mat_emission[] = { 1, 1, 1, 1.0 };

    glPushMatrix();
    glTranslatef(0, 2, 0);
    glScalef(2, 2, 2);
    glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    if (switchOne == true) {
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
    }
    else {
        glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
    }

    glutSolidSphere(1.0, 16, 16);
    glPopMatrix();

    streetLampbody();
}
void spotLight2()
{
    glPushMatrix();

    GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
 
    GLfloat light_position[] = { 30.0, 20, -30.0, 1.0 };

   
    glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular);

    glLightfv(GL_LIGHT2, GL_POSITION, light_position);
    GLfloat spot_direction[] = { 1.0, 0, 0.5 };

    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 70.0);
    glPopMatrix();
}
void streetLight2()
{
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
    GLfloat mat_ambient_color[] = { 0.8, 0.8, 0.2, 1.0 };
    GLfloat mat_diffuse[] = { 0.0, 1.0, 0.0, 1.0 };  // Green color when light is off
    GLfloat high_shininess[] = { 100.0 };
    GLfloat mat_emission[] = { 1,1,1, 1.0 };

    glPushMatrix();
    glTranslatef(0, 2, 0);
    glScalef(2, 2, 2);
    glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
    if (switchTwo == true) {
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
    }
    else {
        glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
    }
    glutSolidSphere(1.0, 16, 16);
    glPopMatrix();

    streetLampbody();
}
void display(void)
{
    // Clear the color and depth buffers to prepare for rendering
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set up the projection matrix using perspective projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1, 1, 300);  // Field of view, aspect ratio, near clipping plane, far clipping plane

    // Set up the modelview matrix using gluLookAt to position the camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eyeX, eyeY, eyeZ, refX, refY, refZ, 0, 1, 0);  // Camera position, reference point, up vector

    // Enable lighting calculations
    glEnable(GL_LIGHTING);

    // Draw the sky with specified color and position
    glPushMatrix();
    sky(eyeX + (0.05 * refX), eyeY + (0.05 * refY), eyeZ + (0.05 * refZ), 250, 250, 250);
    glPopMatrix();

    // Enable depth testing for accurate rendering
    glEnable(GL_DEPTH_TEST);

    // Call various functions to draw spotlights, ground, and other objects
    spotLight1();
    spotLight2();
    ground();
    trees();

    // ... More function calls to draw various objects ...
    //lamp1
    glPushMatrix();
    glTranslatef(-25, 0, -30);
    streetLight1();
    glPopMatrix();

    //lamp
    glPushMatrix();
    glTranslatef(30, 0, -30);
    streetLight2();
    glPopMatrix();

    glPushMatrix();
   
    //ferris wheel
    glTranslatef(0, 0, -30);
    ferrisWheel();
    glPopMatrix();

    glPushMatrix();
    
    glTranslatef(50, 0, 20);
    round_swing();
    glPopMatrix();

    //nepali swing
    glPushMatrix();
   glTranslatef(-40, 0, 20);
    nepali_swing();
    glPopMatrix();



    //grass
    glPushMatrix();
    glTranslatef(0, 0.11, 20);
    glScalef(4, 1, 0.8);
    ground2();
    glPopMatrix();


    //grass
   glPushMatrix();
    glTranslatef(0, 0.1, 20);
    glScalef(0.6, 1, 4);
    ground2();
    glPopMatrix();

  

    //grass below tree
    glPushMatrix();
    glTranslatef(-40, 0, -20);
    glScalef(1, 1, 1.4);
    ground2();
    glPopMatrix();

    //grass below tree
    glPushMatrix();
    glTranslatef(50, 0, -20);
    glScalef(1, 1, 1.4);
    ground2();
    glPopMatrix();


    glDisable(GL_LIGHTING);  // Disable lighting for drawing non-lit objects

    glFlush();   // Flush the OpenGL pipeline to execute pending commands
    
    glutSwapBuffers();  // Swap buffers in a double-buffered context
}
void myKeyboardFunc(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'w': // move eye point upwards along Y axis
        eyeY += 1.0;
        break;
    case 's': // move eye point downwards along Y axis
        eyeY -= 1.0;
        break;
    case 'a': // move eye point left along X axis
        eyeX -= 1.0;
        break;
    case 'd': // move eye point right along X axis
        eyeX += 1.0;
        break;
    case 'o':  //zoom out
        eyeZ += 1;
        break;
    case 'i': //zoom in
        eyeZ -= 1;
        break;
    case 'q': //back to default eye point and ref point
        eyeX = 0.0; eyeY = 2.0; eyeZ = 30.0;
        refX = 0.0; refY = 0.0; refZ = 0.0;
        break;
    case 'j': // move ref point upwards along Y axis
        refY += 1.0;
        break;
    case 'n': // move ref point downwards along Y axis
        refY -= 1.0;
        break;
    case 'b': // move ref point left along X axis
        refX -= 1.0;
        break;
    case 'm': // move eye point right along X axis
        refX += 1.0;
        break;
    case 'k':  //move ref point away from screen/ along z axis
        refZ += 1;
        break;
    case 'l': //move ref point towards screen/ along z axis
        refZ -= 1;
        break;
    case '1': //round swing
        if (roundswingFlag == false) {
            roundswingFlag = true; break;
        }
        else if (roundswingFlag == true) {
            roundswingFlag = false; break;
        }
    case '2': //ferriswheel
        if (ferriswheelcheck == false) {
            ferriswheelcheck = true; break;
        }
        else {
            ferriswheelcheck = false; break;
        }
    case '3': //nepali swing
        if (nepaliswingFlag == false) {
            nepaliswingFlag = true; break;
        }
        else if (nepaliswingFlag == true) {
            nepaliswingFlag = false; break;
        }
    case '4': //spot light 1
        if (switchOne == false) {
            switchOne = true; glEnable(GL_LIGHT1); break;
        }
        else if (switchOne == true) {
            switchOne = false; glDisable(GL_LIGHT1); break;
        }
    case '5': //spot light 2
        if (switchTwo == false) {
            switchTwo = true; glEnable(GL_LIGHT2); break;
        }
        else if (switchTwo == true) {
            switchTwo = false; glDisable(GL_LIGHT2); break;
        }
    case '0': //day/night
        if (day == false) {
            day = true; glEnable(GL_LIGHT0); break;
        }
        else if (day == true) {
            day = false; glDisable(GL_LIGHT0); break;
        }
    case 27:    // Escape key
        exit(1);
    }

    glutPostRedisplay();
}
void animate()
{

    if (nepaliswingFlag == true)
    {
        if (nepaliswingCheck == true)
        {
            nepaliswingTheta -= 1;
            if (nepaliswingTheta == -45)
            {
                nepaliswingCheck = false;
            }
        }
        else
        {
            nepaliswingTheta += 1;
            if (nepaliswingTheta == 45)
            {
                nepaliswingCheck = true;
            }
        }
    }
    else
    {
        if (nepaliswingTheta < 0)
        {
            nepaliswingTheta += 1;
            if (nepaliswingTheta == 0)
            {
                nepaliswingTheta = 0;
            }
        }
        else if (nepaliswingTheta > 0)
        {
            nepaliswingTheta -= 1;
            if (nepaliswingTheta == 0)
            {
                nepaliswingTheta = 0;
            }
        }
    }

    if (ferriswheelcheck == true)
    {
        theta += 1.5;
        if (theta > 360.0)
            theta -= 360.0 * floor(theta / 360.0);
    }

    if (roundswingFlag == true)
    {
        roundswingTheta += 1;
        if (roundswingTheta > 360.0)
            roundswingTheta -= 360.0 * floor(theta / 360.0);

        roundswingAlpha += 0.5;
        if (roundswingAlpha >= 30)
            roundswingAlpha = 30;
    }
    else
    {
        roundswingAlpha -= 0.1;
        if (roundswingAlpha <= 0)
            roundswingAlpha = 0;

        roundswingTheta += 1;
        if (roundswingAlpha == 0)
            roundswingTheta = 0;
    }

    glutPostRedisplay();

}
void fullScreen(int w, int h)
{
    //Prevent a divide by zero, when window is too short;you cant make a window of zero width.
    if (h == 0)
        h = 1;
    float ratio = (GLfloat)w / (GLfloat)h;         //Calculate aspect ratio of the window

    //Set the perspective coordinate system
    glMatrixMode(GL_PROJECTION);                   //Use the Projection Matrix
    glLoadIdentity();                              //Reset Matrix

    glViewport(0, 0, w, h);                        //Set the viewport to be the entire window
    gluPerspective(60, ratio, 1, 500);             //Set the correct perspective.
    glMatrixMode(GL_MODELVIEW);                    //Get Back to the Modelview
}
int main(int argc, char** argv)
{
    // Initialize GLUT with command-line arguments
    glutInit(&argc, argv);

    // Set up display mode with double buffering, RGB color, and depth buffering
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // Set initial window position and size, then create the window
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("GRAPHICS PROJECT USING OPENGL");

    // Set shading model to smooth, enable depth testing and normal vector normalization
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    light();   // Call the function to set up lighting

    // Print instructions for controlling the application
    std::cout << "To move Eye point:" << std::endl;
    cout << "To move Eye point:" << endl;
    cout << "w: up" << endl;
    cout << "s: down" << endl;
    cout << "a: left" << endl;
    cout << "d: right" << endl;
    cout << "i: zoom in" << endl;
    cout << "o: zoom out" << endl;
    cout << "      " << endl;
    cout << "To move Camera point:" << endl;
    cout << "j: up" << endl;
    cout << "n: down" << endl;
    cout << "b: left" << endl;
    cout << "m: right" << endl;
    cout << "l: move nearer" << endl;
    cout << "k: move far" << endl;
    cout << "      " << endl;
    cout << "Press q to move to default position" << endl;
    cout << "      " << endl;
    cout << "To control Rides: " << endl;
    cout << "1: Round swing" << endl;
    cout << "2: Ferris Wheel" << endl;
    cout << "3: Nepali swing" << endl;
    cout << "To control lights: " << endl;
    cout << "4: Spotlight 1" << endl;
    cout << "5: Spotlight 2" << endl;
    cout << "      " << endl;
    cout << "Other controls: " << endl;
    cout << "0: Day/Night" << endl;
    
    LoadTexture("bmp images/whiteground.bmp", 2);
    LoadTexture("bmp images/red-metal.bmp", 4);
    LoadTexture("bmp images/front.bmp", 8);
    LoadTexture("bmp images/back.bmp", 9);
    LoadTexture("bmp images/right.bmp", 10);
    LoadTexture("bmp images/left.bmp", 11);
    LoadTexture("bmp images/grass.bmp", 14);
    LoadTexture("bmp images/up.bmp", 27);
    LoadTexture("bmp images/nightsky.bmp", 28);
    LoadTexture("bmp images/treebark.bmp", 29);
    LoadTexture("bmp images/bush.bmp", 30);       
    

    // Register callback functions for events
    glutReshapeFunc(fullScreen);
    glutKeyboardFunc(myKeyboardFunc);
    glutDisplayFunc(display);
    glutIdleFunc(animate);

    
    glutMainLoop();  // Enter the main event processing loop

    return 0; // Return 0 to indicate successful execution
}