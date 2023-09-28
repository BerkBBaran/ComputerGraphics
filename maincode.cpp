//Essential Libs
#include <windows.h>
#include <math.h>
#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <cstdlib>
#include <stdio.h>
#include <vector>
#include <string>
#include <fstream>
#include <bits/stdc++.h>

//Glut Libs
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

//Define
# define M_PI           3.14159265358979323846
using namespace std;
//Classes
class Triangle{
public:
    int a, b, c;
    Triangle(int aa, int bb, int cc){
        a = aa;
        b = bb;
        c = cc;
    }
};
class Vector3{
public:
    double x, y, z;
    Vector3(double xx, double yy, double zz){
        x = xx;
        y = yy;
        z = zz;
    }
};
//global variables
int index = 1;
string fileLocation = "C://Users//kanar//Desktop//cng477//hw2//test5//";
string extension = ".off";
vector<Triangle> triangleList;
vector<Vector3> verticeList;
//Prototypes
void LoadFile(void);
void ProcessModel (void);
void Menu();
void InitGlut(int,char**);
void KeyboardProcess(unsigned char ,int, int);
void Translate (double, double, double);
void Scale (double, double, double);
double getAvarageZCoordinateOfATriangle2(Triangle);
bool comparetriangleList(Triangle, Triangle);


vector<string> splitString(string str) {
	vector<string> splittedOutput;
	string word = "";

	int i = 0;
	while (str[i] != '\0') {
		while (str[i] == ' ' && str[i] != '\0') {
			i++;
		}
		word = "";
		while (str[i] != ' ' && str[i] != '\0') {
			word = word + str[i];
			if (str[i + 1] == ' ' || str[i + 1] == '\0') {
				splittedOutput.push_back(word);
			}
			i++;
		}
	}
	return splittedOutput;
}

void LoadFiles(){

    int face_amount,vertice_amount,edge_amount;
    int a,b,c;
    double x,y,z;
    ifstream  ifin;
    string indexAsString,line;
    vector<string> tempStringList;

    if(index == 11){
        index = 1;
    }
    indexAsString = to_string(index++);
    string fileName = fileLocation;
    fileName.append(indexAsString);
    fileName.append(extension);

    ifin.open(fileName);
    if(ifin.fail()){
        cerr << "Failed to open the file" << endl;
        exit(1);
    }
    getline(ifin,line); //get off statement
    getline(ifin,line); //get vertices,edges and faces
    tempStringList = splitString(line);
    vertice_amount = stoi(tempStringList.at(0));
    face_amount = stoi(tempStringList.at(1));
    edge_amount = stoi(tempStringList.at(2));

    for(int i = 0; i < vertice_amount; i++){
        getline(ifin,line);
        tempStringList = splitString(line);
        x = stod(tempStringList.at(0));
        y = stod(tempStringList.at(1));
        z = stod(tempStringList.at(2));
        Vector3 tempVector(x,y,z);

        verticeList.push_back(tempVector);
    }
    for(int i = 0; i < face_amount; i++){
        getline(ifin,line);
        tempStringList = splitString(line);
        a = stoi(tempStringList.at(1));
        b = stoi(tempStringList.at(2));
        c = stoi(tempStringList.at(3));

        Triangle tempTriangle(a,b,c);
        triangleList.push_back(tempTriangle);
    }
    ifin.close();

}

double getAvarageZCoordinateOfATriangle2(Triangle triangle){
    return ((verticeList[triangle.a].z + verticeList[triangle.b].z + verticeList[triangle.c].z) / 3);
}

// Compares two triangleList according to center z coordinates.
bool comparetriangleList(Triangle t1, Triangle t2)
{
    double firstTriangleZAvarage,secondTriangleZAvarage;
    firstTriangleZAvarage = getAvarageZCoordinateOfATriangle2(t1);
    secondTriangleZAvarage = getAvarageZCoordinateOfATriangle2(t2);
    return (firstTriangleZAvarage < secondTriangleZAvarage);
}


void Menu(){
    cout << " Move Keys     Rotate Around Itself Keys" << endl;
    cout << "    w                       u" << endl;
    cout << " a  s  d                 h  j  k" << endl << endl;

    cout << "Rotate Around Plane Keys" << endl;
    cout << "         c   v" << endl << endl;

    cout << "Scale Keys" << endl;
    cout << " +   -" << endl << endl;

    cout << "Next Image: x" << endl;
    cout << "---------------------------------------" << endl;
}

int main(int argc, char **argv){
    LoadFiles();
    sort(triangleList.begin(), triangleList.end(), comparetriangleList);
    Menu();
    InitGlut(argc,argv);
    return(0);
}
void InitGlut(int argc, char **argv){

    //camera distance
    int camX_0= -1;
    int camY_0= -1;
    int camX_1 = 1;
    int camY_1= 1;

    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB); //library function
    glutCreateWindow("GLWindow"); //change name of the window
    glMatrixMode(GL_PROJECTION);//library
    glLoadIdentity();//library
    gluOrtho2D(camX_0, camX_1, camY_0, camY_1);//library
    glutDisplayFunc(ProcessModel);
    glutKeyboardFunc(KeyboardProcess);
    glutMainLoop();
}
void Translate (double x, double y, double z){
    int i = verticeList.size()-1;
    while(i >= 0){
        verticeList[i].x += x;
        verticeList[i].y += y;
        verticeList[i].z += z;
        --i;
    }
}

void Scale (double x, double y, double z){
    int i = verticeList.size()-1;
    while(i >= 0){
        verticeList[i].x *= x;
        verticeList[i].y *= y;
        verticeList[i].z *= z;
        --i;
    }

}

void ProcessModel(void){

    int state = 1;
    double myCol = 0.0,myCol2 = 1.0, myCol3 = 0.0;

    glClearColor(0.5, 0.5, 0.5, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_TRIANGLES);

    for(int i = 0; i < triangleList.size(); i++){
      if(i % 1000 == 0){
        if(state == 4){
            state = 1;
        }
        if(state == 1){
            myCol = 1.0,myCol2 = 0.0, myCol3 = 0.0;
        }
        else if(state == 2){
            myCol = 0.0,myCol2 = 1.0, myCol3 = 0.0;
        }
        else if(state == 3){
            myCol = 0.0,myCol2 = 0.0, myCol3 = 1.0;
        }
        state++;
      }
      if(state == 1){
        myCol -= 0.001;
        myCol2 += 0.001;
        myCol3 += 0.001;
      }
      else if(state == 2){
        myCol += 0.001;
        myCol2 -= 0.001;
        myCol3 += 0.001;
      }
      else{
        myCol += 0.001;
        myCol2 += 0.001;
        myCol3 -= 0.001;
      }

      glColor3f(myCol,myCol2,myCol3);

      glVertex3f(verticeList[triangleList[i].a].x, verticeList[triangleList[i].a].y, verticeList[triangleList[i].a].z);
      glVertex3f(verticeList[triangleList[i].b].x, verticeList[triangleList[i].b].y, verticeList[triangleList[i].b].z);
      glVertex3f(verticeList[triangleList[i].c].x, verticeList[triangleList[i].c].y, verticeList[triangleList[i].c].z);
    }
    glEnd();
    glutSwapBuffers();
}


void Rotate(double a, double x, double y, double z){
    double rotation_matrix[9];
    double sina,cosa;

    a *= M_PI / 180.0;
    sina = sinf(a);
    cosa = cosf(a);

    rotation_matrix[0] = x*x*(1 - cosa) + cosa;
    rotation_matrix[1] = x*y*(1 - cosa) - z*sina;
    rotation_matrix[2] = x*z*(1 - cosa) + y*sina;
    rotation_matrix[3] = y*x*(1 - cosa) + z*sina;
    rotation_matrix[4] = y*y*(1 - cosa) + cosa;
    rotation_matrix[5] = y*z*(1 - cosa) - x*sina;
    rotation_matrix[6] = z*x*(1 - cosa) - y*sina;
    rotation_matrix[7] = z*y*(1 - cosa) + x*sina;
    rotation_matrix[8] = z*z*(1 - cosa) + cosa;

    int i = verticeList.size()-1;
    while(i >= 0){
        double x = rotation_matrix[0] * verticeList[i].x + rotation_matrix[1] * verticeList[i].y + rotation_matrix[2] * verticeList[i].z;
        double y = rotation_matrix[3] * verticeList[i].x + rotation_matrix[4] * verticeList[i].y + rotation_matrix[5] * verticeList[i].z;
        double z = rotation_matrix[6] * verticeList[i].x + rotation_matrix[7] * verticeList[i].y + rotation_matrix[8] * verticeList[i].z;

        verticeList[i].x = x;
        verticeList[i].y = y;
        verticeList[i].z = z;
        --i;
    }


}

void KeyboardProcess (unsigned char key, int x, int y){

    switch(key){
        //Move
        case 'a':
            Translate(-0.1, 0,  0);
            break;
        case 's':
            Translate(0, -0.1,  0);
            break;
        case 'd':
            Translate(0.1, 0,  0);
            break;
        case 'w':
            Translate(0, 0.1,  0);
            break;
        //Rotate around itself
         case 'h':
            Rotate(10, 0.0, 1.0, 0.0);
            break;
        case 'j':
            Rotate(10,1.0, 0.0, 0.0);
            break;
        case 'k':
            Rotate(10, 0.0, -1.0, 0.0);
            break;
        case 'u':
            Rotate(10,1.0, 0.0, 0.0);
            break;
        //Rotate Around Plane
        case 'c':
            Rotate(10, 0.0, 0.0, 1.0);
            break;
        case 'v':
            Rotate(10, 0.0, 0.0, -1.0);
            break;
        //Scale
        case '+':
            Scale(1.2, 1.2, 1.2);
            break;
        case '-':
            Scale(0.8, 0.8, 0.8);
            break;
        //Next Mesh
        case 'x':
            verticeList.clear();
            triangleList.clear();
            LoadFiles();
            sort(triangleList.begin(), triangleList.end(), comparetriangleList);
            break;
    }

    ProcessModel();
}
