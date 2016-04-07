#include "Mesh2.h"
#include "GameFunctions.h"

void Mesh::Init(std::vector<Vertex> vertices, std::vector<GLuint> indices){
	this->vertices = vertices;
	this->indices = indices;
	//this->textures = textures;

	this->setupMesh();
}

void Mesh::Init(const std::string& fileName){
	if (!FileExists(fileName)){
		std::cout << "ERROR: Mesh: File \"" << fileName << "\" does not exist. Mesh load cancelled" << std::endl;
		return;
	}
	IndexedModel m_model = OBJModel(fileName).ToIndexedModel();

	indices = m_model.indices;
	for (GLuint i = 0; i < m_model.positions.size(); i++){
		Vertex v;
		v.Position = m_model.positions[i];
		v.Normal = m_model.normals[i];
		v.TexCoords = m_model.texCoords[i];
		vertices.push_back(v);
	}

	SetBounds();
	setupMesh();
}

void Mesh::setupMesh(){
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));

	glBindVertexArray(0);
}

void Mesh::Draw(){
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Mesh::SetBounds(){
	smallvec = glm::vec3(NULL, NULL, NULL);
	bigvec = glm::vec3(NULL, NULL, NULL);
	for (GLuint i = 0; i < vertices.size(); i++){
		for (GLuint u = 0; u < 3; u++){
			if (vertices[i].Position[u] < smallvec[u] || smallvec[u] == NULL){
				smallvec[u] = vertices[i].Position[u];
			}
			if (vertices[i].Position[u] > bigvec[u] || bigvec[u] == NULL){
				bigvec[u] = vertices[i].Position[u];
			}
		}
	}
}