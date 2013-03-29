#include "GameObject.h"
#include "ngl/Obj.h"
#include "iostream"
#include "ngl/VertexArrayObject.h"
#include "vector"

void GameObject::loadMesh( const std::string& _meshName )
{
	std::cout << "loading mesh\n";
	// load the obj
	ngl::Obj mesh( _meshName );
	std::cout << "checking triangular\n";
	
	// this is only going to work for tri meshes so check
	if( ! mesh.isTriangular() ) {
		std::cout << "only works for tri meshes\n";
		exit( EXIT_FAILURE );
	}
	
	std::cout << "getting lists\n";
	
	// get the obj data so we can process it locally
	std::vector <ngl::Vector> verts = mesh.getVertexList();
	std::vector <ngl::Face> faces = mesh.getFaceList();
	std::vector <ngl::Vector> tex = mesh.getTextureCordList();
	std::vector <ngl::Vector> normals = mesh.getNormalList();
	
	std::cout << "got mesh data\n";
	// now we are going to process and pack the mesh into an ngl::VertexArrayObject
	std::vector <vertData> vboMesh;
	vertData d;
	unsigned int nFaces = faces.size();
	unsigned int nNorm = normals.size();
	unsigned int nTex = tex.size();
	
	// loop for each of the faces
	for( unsigned int i = 0; i < nFaces; ++i ) {
		// now for each triangle in the face (remember we ensured tri above)
		for( int j = 0; j < 3; ++j ) {
			// pack in the vertex data first
			d.x = verts[faces[i].m_vert[j]].m_x;
			d.y = verts[faces[i].m_vert[j]].m_y;
			d.z = verts[faces[i].m_vert[j]].m_z;
			
			// now if we have norms of tex (possibly could not) pack them as well
			if( nNorm > 0 && nTex > 0 ) {
				d.nx = normals[faces[i].m_norm[j]].m_x;
				d.ny = normals[faces[i].m_norm[j]].m_y;
				d.nz = normals[faces[i].m_norm[j]].m_z;
				
				d.u = tex[faces[i].m_tex[j]].m_x;
				d.v = tex[faces[i].m_tex[j]].m_y;
				
			}
			// now if neither are present (only verts like Zbrush models)
			else if( nNorm == 0 && nTex == 0 ) {
				d.nx = 0;
				d.ny = 0;
				d.nz = 0;
				d.u = 0;
				d.v = 0;
			}
			// here we've got norms but not tex
			else if( nNorm > 0 && nTex == 0 ) {
				d.nx = normals[faces[i].m_norm[j]].m_x;
				d.ny = normals[faces[i].m_norm[j]].m_y;
				d.nz = normals[faces[i].m_norm[j]].m_z;
				d.u = 0;
				d.v = 0;
				
			}
			// here we've got tex but not norm least common
			else if( nNorm == 0 && nTex > 0 ) {
				d.nx = 0;
				d.ny = 0;
				d.nz = 0;
				d.u = tex[faces[i].m_tex[j]].m_x;
				d.v = tex[faces[i].m_tex[j]].m_y;
			}
			
			// now we calculate the tangent / bi-normal (tangent) based on the article here
			// http://www.terathon.com/code/tangent.html
			
			ngl::Vector c1 = normals[faces[i].m_norm[j]].cross( ngl::Vector( 0.0, 0.0, 1.0 ) );
			ngl::Vector c2 = normals[faces[i].m_norm[j]].cross( ngl::Vector( 0.0, 1.0, 0.0 ) );
			ngl::Vector tangent;
			ngl::Vector binormal;
			
			if( c1.length() > c2.length() ) {
				tangent = c1;
			}
			else {
				tangent = c2;
			}
			
			// now we normalize the tangent so we don't need to do it in the shader
			tangent.normalize();
			// now we calculate the binormal using the model normal and tangent (cross)
			binormal = normals[faces[i].m_norm[j]].cross( tangent );
			// normalize again so we don't need to in the shader
			binormal.normalize();
			d.tx = tangent.m_x;
			d.ty = tangent.m_y;
			d.tz = tangent.m_z;
			d.bx = binormal.m_x;
			d.by = binormal.m_y;
			d.bz = binormal.m_z;
			// finally add it to our mesh VAO structure
			vboMesh.push_back( d );
		}
	}
	
	// first we grab an instance of our VOA class as a TRIANGLE_STRIP
	vao = ngl::VertexArrayObject::createVOA( GL_TRIANGLES );
	// next we bind it so it's active for setting data
	vao->bind();
	
	unsigned int meshSize = vboMesh.size();
	// now we have our data add it to the VAO, we need to tell the VAO the following
	// how much (in bytes) data we are copying
	// a pointer to the first element of data (in this case the address of the first element of the
	// std::vector
	
	vao->setData( meshSize * sizeof( vertData ), vboMesh[0].u );
	// in this case we have packed our data in interleaved format as follows
	// u,v,nx,ny,nz,x,y,z
	// If you look at the shader we have the following attributes being used
	// attribute vec3 inVert; attribute 0
	// attribute vec2 inUV; attribute 1
	// attribute vec3 inNormal; attribure 2
	// so we need to set the vertexAttributePointer so the correct size and type as follows
	// vertex is attribute 0 with x,y,z(3) parts of type GL_FLOAT, our complete packed data is
	// sizeof(vertData) and the offset into the data structure for the first x component is 5 (u,v,nx,ny,nz)..x
	
	vao->setVertexAttributePointer( 0, 3, GL_FLOAT, sizeof( vertData ), 5 );
	// uv same as above but starts at 0 and is attrib 1 and only u,v so 2
	
	vao->setVertexAttributePointer( 1, 2, GL_FLOAT, sizeof( vertData ), 0 );
	// normal same as vertex only starts at position 2 (u,v)-> nx
	vao->setVertexAttributePointer( 2, 3, GL_FLOAT, sizeof( vertData ), 2 );
	
	// tangent same as vertex only starts at position 8 (u,v)-> nx
	vao->setVertexAttributePointer( 3, 3, GL_FLOAT, sizeof( vertData ), 8 );
	
	// bi-tangent (or Binormal) same as vertex only starts at position 11 (u,v)-> nx
	vao->setVertexAttributePointer( 4, 3, GL_FLOAT, sizeof( vertData ), 11 );
	
	// now we have set the vertex attributes we tell the VAO class how many indices to draw when
	// glDrawArrays is called, in this case we use buffSize (but if we wished less of the sphere to be drawn we could
	// specify less (in steps of 3))
	vao->setNumIndices( meshSize );
	
	// finally we have finished for now so time to unbind the VAO
	vao->unbind();
}
