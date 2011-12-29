/*	VNCMesh.h
	____________________________________________________________________________

	Defines classes used to build polygonal meshes.

	Class
	---------
	VertexID ... vertex
	Face ....... planar face (polygon)
	Mesh ....... polygonal mesh

	METHODS
    read ........ read mesh from a file
	draw ........ draw the mesh

	Phillip Napieralski
	November 26, 2008
	- Added getCenter() method and offset methods/Vector3
	____________________________________________________________________________
*/

#ifndef MESH
#define MESH

#include <iostream>
#include <fstream>
#include <cmath>
#include <cassert>
#include <string>
using namespace std;

#include "Vector3.h"

/*                                                           ________________
**__________________________________________________________/ VertexID Class \__
*/

class VertexID
{
public:
   int vertIndex; // index of this vert in the vertex list
   int normIndex; // index of this vertex's normal
};

/*                                                               ____________
**______________________________________________________________/ Face Class \__
*/

class Face
{
public: 
   int      nVerts;     // number of vertices in this face
   VertexID *vert;      // the list of vertex and normal indices
   float    color[3];   // RGB face color

   Face(){ nVerts = 0; vert = NULL; }    // constructor

	~Face(){ delete[] vert; nVerts = 0; } // destructor
};

/*                                                                ___________
**_______________________________________________________________/ Mesh class\__
*/

class Mesh {

};


class VNCMesh {
private:
   int numVerts;	// number of vertices in the mesh
   Point3  *pt;	    // array of 3D vertices

   int numNormals;  // number of normal vectors for the mesh
   Vector3 *norm;   // array of normals 

   int numFaces; 	  // number of faces in the mesh
   Face *face;	     // array of face data

   Vector3 offset;

public:
   VNCMesh(); 	// constructor
   ~VNCMesh(); // destructor

   void read(const string& fname);  // reads data for this mesh from a file
   void draw(bool filled = false);  // draws this mesh
   void setOffset( Vector3 off ){ offset = off; }
   void applyOffset()
   {
	   for( int i = 0; i < numVerts; i++ )
		   pt[i].set( pt[i].x+offset.x, pt[i].y+offset.y, pt[i].z+offset.z );
   }

   /* Preconditions: read() */
   Point3 getCenter();
};	

VNCMesh::VNCMesh()
{ // Construct an empty mesh.
   numVerts = numFaces = numNormals = 0; 
   pt = NULL;  norm =NULL;  face = NULL;
   offset = Vector3(0,0,0);
}

VNCMesh::~VNCMesh()
{ // Free up memory used by this VNCMesh.
   delete [] pt;    
   delete [] norm;  
   for(int f = 0; f < numFaces; f++)
	   face[f].~Face();
      //delete[] face[f].vert; 
}

void VNCMesh::read(const string& fname)
{
   fstream inStream;

   inStream.open(fname.c_str(), ios::in);
	if(inStream.fail() || inStream.eof()) 
	{
      std::cerr << "Can't open file or file is empty: " << fname << endl; 
      return;
	}

   inStream >> numVerts >> numNormals >> numFaces;
   inStream.ignore(256, '\n');

	// Make arrays for vertices, normals, and faces
   pt = new Point3[numVerts];        assert(pt != NULL);
   norm = new Vector3[numNormals];   assert(norm != NULL);
   face = new Face[numFaces];        assert(face != NULL);

   // Read the vertices.
   for(int i = 0; i < numVerts; i++)
   {
		inStream >> pt[i].x >> pt[i].y >> pt[i].z;
      inStream.ignore(256, '\n');
   }

   // Read the normals.
	for(int i = 0; i < numNormals; i++) 
   {
		inStream >> norm[i].x >> norm[i].y >> norm[i].z;
      inStream.ignore(256, '\n');
   }

   // Read face data.
   for(int f = 0; f < numFaces; f++)       
   {  
      inStream >> face[f].nVerts;
      inStream.ignore(256, '\n');
      int n = face[f].nVerts;
      face[f].vert = new VertexID[n]; 
      assert(face[f].vert != NULL);
      // Read vertex indices for this face.
		for(int k = 0; k < n; k++)  
			inStream >> face[f].vert[k].vertIndex;
      inStream.ignore(256, '\n');
      // Read normal indices for this face.
		for(int kk = 0; kk < n; kk++) 
			inStream >> face[f].vert[kk].normIndex;
      inStream.ignore(256, '\n');
      // Read RGB color for this face
		for(int j = 0; j < 3; j++) 
		  	inStream >> face[f].color[j];
      inStream.ignore(256, '\n');
	}

	inStream.close();
} 

void VNCMesh::draw(bool filled) 
{          
   for(int f = 0; f < numFaces; f++) 
	{  // Draw each face.
      if (filled) glColor3fv(face[f].color);
      glBegin(filled ? GL_POLYGON : GL_LINE_LOOP);
         for(int v = 0; v < face[f].nVerts; v++) 
         {
            int in = face[f].vert[v].normIndex ;
            int iv =  face[f].vert[v].vertIndex ;
            glNormal3f(norm[in].x, norm[in].y, norm[in].z);
            glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
         }
      glEnd();
   }
}

Point3 VNCMesh::getCenter()
{
	Point3 center(0,0,0);

	for( int i = 0; i < numVerts; i++ )
	{
		center.x += pt[i].x;
		center.y += pt[i].y;
		center.z += pt[i].z;
	}
	center.x /= numVerts;
	center.y /= numVerts;
	center.z /= numVerts;

	return center;
}

#endif
